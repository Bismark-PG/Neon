/*==============================================================================

	Manage Player Logic [Player.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Project_Header.h"
#include "Player.h"
#include "KeyLogger.h"
#include "Shader_Manager.h"
#include "Player_Camera.h" 
#include "Debug_Collision.h"
#include "Palette.h"
#include "Billboard_Manager.h"
using namespace DirectX;
using namespace PALETTE;

#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD    30

// Player Physics Constants
constexpr float PLAYER_SCALE = 0.01f;
constexpr float PLAYER_DRAG = 5.0f;

static XMFLOAT3 Player_Pos	 = {};
static XMFLOAT3 Player_Start_POS = {};
static XMFLOAT3 Player_Front = { 0.0f, 0.0f, 1.0f };
static XMFLOAT3 Player_Vel	 = {};
static XMVECTOR Player_Speed = {};

static XMVECTOR Gravity		= { 0.0f, -15.0f, 0.0f };
static XMVECTOR Walk_Speed	= { 35.f, 0.0, 35.f };
static XMVECTOR Run_Speed	= { 75.f, 0.0, 75.f };

static MODEL* Player_Model{nullptr};
static bool Is_Input_Moving = false;
static bool Is_Jump = false;
static bool Is_Run = false;
static bool Is_Run_Toggle = false;

// Player resource
static const int Player_Base_MaxHP = 100;
static int Player_HP = 100;
static int Player_MaxHP = 100;
static float Player_Damage = 1.0f;

static bool Player_Is_Dead = false;
static bool Is_Shoot = false;
static bool Is_Aiming_Mode = false;
static bool Is_Game_Over_Sequence = false;

static bool  Is_Invincible = false;     
static float Invincible_Timer = 0.0f;   
constexpr float INVINCIBLE_DURATION = 1.0f;

static float Walk_Timer = 0.0f;

// Weapon Drop System
static float Interact_Timer = 0.0f;
static bool Has_Dropped_Weapon = false;
static bool Prev_Interact_Input = false;

// Level Bonus Ratios
static float Bonus_Damage_Ratio = 0.0f;
static float Bonus_Speed_Ratio = 0.0f;
static float Bonus_HP_Ratio = 0.0f;
constexpr float Bonus_HP_Heal = 0.1f;

static E_PlayerState Current_State = E_PlayerState::IDLE;

// ----------------------------------------------------------
//				static Player Update Logic
// ----------------------------------------------------------
// --- Movement And Physics System ---

// --- Input ---
static XMVECTOR Player_Update_Is_Jumped(XMVECTOR Vel);
static XMVECTOR Player_Update_Is_Moved(XMVECTOR Dir, XMVECTOR Flat_Front, XMVECTOR Flat_Right);
// ----------------------------------------------------------

void Player_Initialize(const XMFLOAT3& First_POS, const XMFLOAT3& First_Front)
{
	Player_Pos = First_POS;
	Player_Start_POS = First_POS; 
	Player_Vel = { 0.0f, 0.0f, 0.0f };
	XMStoreFloat3(&Player_Front, XMVector3Normalize(XMLoadFloat3(&First_Front)));
}

void Player_Finalize()
{
	Player_Model = nullptr;
}

void Player_Update(double elapsed_time)
{
	float dt = static_cast<float>(elapsed_time);

	// ---------------------------------------------
	//	 --- 1. Game Over And Invincible Logic ---
	// ---------------------------------------------
	if (Player_Is_Dead || Is_Game_Over_Sequence) return;

	if (Is_Invincible)
	{
		Invincible_Timer -= dt;

		if (Invincible_Timer <= 0.0f)
		{
			Is_Invincible = false;
			Invincible_Timer = 0.0f;
		}
	}

	// ------------------------
	//	--- 2. Input Check ---
	// ------------------------

	// ---------------------------------
	//	--- 3. Physics Calculation ---
	// ---------------------------------
	XMVECTOR POS = XMLoadFloat3(&Player_Pos);
	XMVECTOR Vel = XMLoadFloat3(&Player_Vel);

	// Jump Logic
	Vel = Player_Update_Is_Jumped(Vel);

	// Gravity Apply
	Vel += Gravity * dt;

	// Movement Logic
	XMVECTOR Dir{};
	XMVECTOR Front = XMLoadFloat3(&Player_Camera_Get_Front());
	XMVECTOR Flat_Front = XMVector3Normalize(XMVectorSetY(Front, 0.0f));
	XMVECTOR Flat_Right = XMVector3Cross({ 0.0f,1.0f,0.0f }, Flat_Front);
	Is_Input_Moving = false;

	Dir = Player_Update_Is_Moved(Dir, Flat_Front, Flat_Right);
	Dir = XMVector3Normalize(Dir);

	if (Is_Player_Jump())
	{
		Dir = XMVectorZero();
	}

	// Set Direction And Speed
	Vel += Dir * Player_Speed * dt;

	// Friction

	XMVECTOR Horizontal_Vel = XMVectorMultiply(Vel, XMVectorSet(1.0f, 0.0f, 1.0f, 0.0f));
	if (!Is_Player_Jump())
	{
		Horizontal_Vel -= Horizontal_Vel * PLAYER_DRAG * dt;
	}
	else
	{
		Horizontal_Vel -= Horizontal_Vel * (PLAYER_DRAG * 0.05f) * dt;
	}
	Vel = XMVectorMultiply(Vel, XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f)) + Horizontal_Vel;

	// Apply Velocity
	POS += Vel * dt;
	XMStoreFloat3(&Player_Pos, POS);

	// Reset Delta Time For Clamping
	if (dt > 0.05f)
	{
		dt = 0.05f;
	}

	// ------------------------------------------------------
	//	--- 4. Collision & Respawn (Physics Correction) ---
	// ------------------------------------------------------
}

void Player_Draw()
{
	if (!Player_Model) return;

	Shader_Manager::GetInstance()->SetLightSpecular(Player_Camera_Get_POS(), 164.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

	// Set Player Model Scale
	XMMATRIX S = XMMatrixScaling(PLAYER_SCALE, PLAYER_SCALE, PLAYER_SCALE);

	// Get Movement
	XMMATRIX T = XMMatrixTranslation(Player_Pos.x, Player_Pos.y, Player_Pos.z);

	// Get Yaw
	float Yaw = atan2f(Player_Front.x, Player_Front.z);
	float Model_Yaw = Yaw + XM_PI;
	XMMATRIX R = XMMatrixRotationY(Model_Yaw);

	// Reverse Player
	// Scale >> Rotate >> Translate
	XMMATRIX world = S * R * T;
}

void Player_Reset()
{
	// 1. Movement Reset
	Player_Pos = Player_Start_POS;
	Player_Vel = { 0.0f, 0.0f, 0.0f };
	Player_Speed = { 0.0f, 0.0f, 0.0f, 0.0f };

	// 2. Resource Reset
	Player_HP = Player_Base_MaxHP;
	Player_MaxHP = Player_Base_MaxHP;
	Player_Damage = 1.0f;

	Player_Is_Dead = false;
	Is_Shoot = false;
	Is_Aiming_Mode = false;
	Is_Game_Over_Sequence = false;

	// 3. Invincible Reset
	Bonus_Damage_Ratio = 0.0f;
	Bonus_Speed_Ratio = 0.0f;
	Bonus_HP_Ratio = 0.0f;

	// 4. Drop Weapon Reset
	Interact_Timer = 0.0f;
	Has_Dropped_Weapon = false;
	Prev_Interact_Input = false;
}


void Player_Set_Sprint_Toggle_Mode(bool isToggle)
{
	Is_Run_Toggle = isToggle;
}

void Player_Set_POS(XMFLOAT3& POS)
{
	Player_Pos = POS;
}

const XMFLOAT3& Player_Get_POS()
{
	return Player_Pos;
}

const XMFLOAT3& Player_Get_Front()
{
	return Player_Front;
}

AABB Player_Get_AABB()
{
	float w = 0.25f;
	float h = 1.75f;
	float d = 0.25f;

	return AABB
	{
		{ Player_Pos.x + w, Player_Pos.y + h, Player_Pos.z + d }, // Max
		{ Player_Pos.x - w, Player_Pos.y	, Player_Pos.z - d }  // Min
	};
}

void Player_OnDamage(int damage)
{
	if (Player_Is_Dead || Is_Game_Over_Sequence) return;
	if (Is_Invincible) return;

	Player_HP -= damage;

	Is_Invincible = true;
	Invincible_Timer = INVINCIBLE_DURATION;

	if (Player_HP <= 0)
	{
		Player_Is_Dead = true;
		Is_Shoot = false;
		Is_Aiming_Mode = false;
		Is_Game_Over_Sequence = true;
	}
}

void Player_Heal(int amount)
{
	if (Player_Is_Dead) return;
	Player_HP += amount;
	if (Player_HP > Player_MaxHP) Player_HP = Player_MaxHP;
}

int Player_Get_HP()
{
	return Player_HP;
}

int Player_Get_MaxHP()
{
	return Player_MaxHP;
}

bool Player_Check_Is_Dead()
{
	return Is_Game_Over_Sequence;
}

bool Player_Is_Aiming_Now()
{
	return Is_Aiming_Mode;
}

void Player_LevelUp()
{

}

float Player_Get_Damage_Coefficient()
{
	return Player_Damage * (1.0f + Bonus_Damage_Ratio);
}

void Player_Add_Damage_Bonus(float amount)
{
	Bonus_Damage_Ratio += amount;
}

void Player_Add_Speed_Bonus(float amount)
{
	Bonus_Speed_Ratio += amount;
}

void Player_Add_HP_Bonus(float amount)
{
	Bonus_HP_Ratio += amount;

	int addedHP = static_cast<int>(Player_Base_MaxHP * amount);

	Player_MaxHP += addedHP;
	Player_HP += addedHP;
}

// ----------------------------------------------------------------------------------------------------------------
//												Player Update Logic
//											  --- Movement System ---
// ----------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------
//												--- Input ---
// ----------------------------------------------------------------------------------------------------------------
XMVECTOR Player_Update_Is_Jumped(XMVECTOR Vel)
{
	static BYTE Prev_LT = 0;
	BYTE Curr_LT = XKeyLogger_GetLeftTrigger();
	bool Is_LT_Triggered = (Curr_LT > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) && (Prev_LT <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
	Prev_LT = Curr_LT;

	// Jump Logic
	if ((KeyLogger_IsTrigger(KK_SPACE) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_A) || Is_LT_Triggered) && !Is_Jump)
	{
		Is_Jump = true;
		Vel = XMVectorSetY(Vel, 0.0f);
		Vel += XMVECTOR{ 0.0f, 10.0f, 0.0f };
		// Audio_Manager::GetInstance()->Play_SFX("Player_Jump");
	}

	return Vel;
}

XMVECTOR Player_Update_Is_Moved(XMVECTOR Dir, XMVECTOR Flat_Front, XMVECTOR Flat_Right)
{
	// 1. Vector Init
	XMVECTOR InputDir = XMVectorZero();

	// 2-1. Get Vector From Keyboard Input
	if (KeyLogger_IsPressed(KK_W)) InputDir += Flat_Front;
	if (KeyLogger_IsPressed(KK_S)) InputDir -= Flat_Front;
	if (KeyLogger_IsPressed(KK_D)) InputDir += Flat_Right;
	if (KeyLogger_IsPressed(KK_A)) InputDir -= Flat_Right;

	// 2-2. Get Vector From Gamepad D-Pad Input
	if (XKeyLogger_IsPadPressed(XINPUT_GAMEPAD_DPAD_UP))    InputDir += Flat_Front;
	if (XKeyLogger_IsPadPressed(XINPUT_GAMEPAD_DPAD_DOWN))  InputDir -= Flat_Front;
	if (XKeyLogger_IsPadPressed(XINPUT_GAMEPAD_DPAD_RIGHT)) InputDir += Flat_Right;
	if (XKeyLogger_IsPadPressed(XINPUT_GAMEPAD_DPAD_LEFT))  InputDir -= Flat_Right;

	// 2-3. Get Vector From Gamepad Stick Input
	XMFLOAT2 Stick = XKeyLogger_GetLeftStick();
	float Stick_Magnitude = sqrtf(Stick.x * Stick.x + Stick.y * Stick.y); // Pythagorean Theorem

	if (Stick_Magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		// Normalize Stick Vector
		XMVECTOR StickDir = XMVectorSet(Stick.x, 0.0f, Stick.y, 0.0f);
		StickDir = XMVector3Normalize(StickDir);

		// Transform Stick Vector to World Space (Relative to Camera)
		// Stick.y (Up) -> Flat_Front
		// Stick.x (Right) -> Flat_Right
		XMVECTOR WorldStickDir = (Flat_Front * (Stick.y / 32767.0f)) + (Flat_Right * (Stick.x / 32767.0f));
		InputDir += WorldStickDir;
	}

	// 3. Get Length Squared
	XMVECTOR LengthSq = XMVector3LengthSq(InputDir);
	float lenSq = XMVectorGetX(LengthSq);

	// 4. If Moved, Normalize And Return
	if (lenSq > 0.0001f)
	{
		Is_Input_Moving = true;
		return Dir + XMVector3Normalize(InputDir);
	}
	else
	{
		Is_Input_Moving = false;
		return Dir;
	}
}

// ----------------------------------------------------------------------------------------------------------------
//											--- Parameter Geter  ---
// ----------------------------------------------------------------------------------------------------------------
bool Is_Player_Move()
{
	return Is_Input_Moving;
}

bool Is_Player_Jump()
{
	return Is_Jump;
}

bool Is_Player_Shoot()
{
	return Is_Shoot;
}

bool Is_Plyer_Run()
{
	return Is_Run;
}