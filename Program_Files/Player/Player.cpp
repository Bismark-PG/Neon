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
#include "Cube.h"
#include "Heapler_Logic.h"
#include "Game_Window.h"
#include "Bullet_Manager.h"
#include "Enemy_Manager.h"
#include "Weapon_System.h"
using namespace DirectX;
using namespace PALETTE;

#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD    30

constexpr XMFLOAT3 FIRST_POS = { 0, 0, 0 };

// Player Physics Constants
static XMFLOAT3 Player_Pos = {};
static XMFLOAT3 Aim_Pos = {};
static float Player_Move_Speed = 25.0f;
static float Aim_Move_Speed = 40.0f;

static float Player_Tension_Speed = 6.0f;
static float Safe_Zone_Ratio = 0.7f;

// --- Combat System ---
static WeaponType Current_Weapon = WeaponType::MACHINE_GUN;
static float Fire_Cooldown = 0.0f;

// Player resource
static int Player_TexID = -1;
static XMFLOAT2 Player_Size = { 2.0f, 2.0f };
static float Player_HP = 100;
static float Player_MaxHP = 100;
static float Player_ATK = 10;
static float Player_DEF = 10;
static bool Is_Input_Moving = false;

// ----------------------------------------------------------
//				static Player Update Logic
// ----------------------------------------------------------
// --- Movement And Physics System ---

// --- Input ---
static XMVECTOR Player_Update_Movement_Input();
static void Player_Update_Aim_Input(float dt);
static void Player_Update_Weapon_Logic(float dt);
// ----------------------------------------------------------

void Player_Initialize()
{
	Player_Pos	= FIRST_POS;
	Aim_Pos		= FIRST_POS;

	Player_TexID = Texture_Manager::GetInstance()->GetID("Player");
}

void Player_Finalize()
{
}

void Player_Update(float elapsed_time)
{
	// Aim Update
	Player_Update_Aim_Input(elapsed_time);

	// Movement Update
	XMVECTOR Move_Dir = Player_Update_Movement_Input();

	XMVECTOR Current_Pos = XMLoadFloat3(&Player_Pos);
	XMVECTOR Target_Aim = XMLoadFloat3(&Aim_Pos);

	// Clamped Player Movement Limit
	float Limit_X = Get_Player_Limit_X();
	float Limit_Y_Min = Get_Player_Limit_Y_Min();
	float Limit_Y_Max = Get_Player_Limit_Y_Max();

	if (Is_Input_Moving)
	{
		// If Input, Move In Limit
		Current_Pos += Move_Dir * Player_Move_Speed * elapsed_time;

		float Clamped_X = ClampFloat(XMVectorGetX(Current_Pos), -Limit_X, Limit_X);
		float Clamped_Y = ClampFloat(XMVectorGetY(Current_Pos), Limit_Y_Min, Limit_Y_Max);
		Current_Pos = XMVectorSet(Clamped_X, Clamped_Y, 0.0f, 0.0f);
	}
	else
	{
		// If Not Input, Move Safe Zone
		float Safe_X = Limit_X * Safe_Zone_Ratio;
		float Safe_Y_Min = Limit_Y_Min * Safe_Zone_Ratio;
		float Safe_Y_Max = Limit_Y_Max * Safe_Zone_Ratio;

		float Target_X = ClampFloat(XMVectorGetX(Current_Pos), -Safe_X, Safe_X);
		float Target_Y = ClampFloat(XMVectorGetY(Current_Pos), Safe_Y_Min, Safe_Y_Max);

		XMVECTOR Target_Pos = XMVectorSet(Target_X, Target_Y, 0.0f, 0.0f);
		Current_Pos = XMVectorLerp(Current_Pos, Target_Pos, Player_Tension_Speed * elapsed_time);
	}

	XMStoreFloat3(&Player_Pos, Current_Pos);

	Fire_Cooldown -= elapsed_time;
	Player_Update_Weapon_Logic(elapsed_time);
}

void Player_Reset() 
{
	// 1. Movement Reset
	Player_Pos	= FIRST_POS;
	Aim_Pos		= FIRST_POS;

	// 2. Resource Reset
}

void Player_Draw()
{
	if (Player_TexID != -1)
	{
		Billboard_Draw(Player_TexID, Player_Pos, Player_Size.x, Player_Size.y, { 0.5f, 0.5f });

		XMFLOAT4 Aim_Color = (Current_Weapon == WeaponType::MACHINE_GUN) ?
			XMFLOAT4(1.0f, 1.0f, 0.0f, 0.5f) : XMFLOAT4(0.0f, 0.5f, 1.0f, 0.5f);
		Billboard_Draw(Player_TexID, Aim_Pos, 0.5f, 0.5f, { 0.5f, 0.5f }, Aim_Color);

		for (const LockOn_Data& L : Weapon_Manager::GetInstance().Return_Lock_On_List())
		{
			if (L.Target_Ptr->IsActive() && L.Target_Ptr->GetUniqueID() == L.Target_ID)
			{
				Billboard_Draw(Player_TexID, L.Target_Ptr->GetPosition(), 1.5f, 1.5f, { 0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f, 0.8f }, Billboard_Facing::ALL_AXIS);
			}
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------
//											   Player Parameter Setter 
// ----------------------------------------------------------------------------------------------------------------
void Player_Set_POS(XMFLOAT3& POS)
{
	Player_Pos = POS;
}

void Player_Damaged(int damage)
{
	Player_HP -= damage;
	if (Player_HP < 0) Player_HP = 0;
}

void Player_Fire_Interval(float Interval)
{
	Fire_Cooldown = Interval;
}

// ----------------------------------------------------------------------------------------------------------------
//											    Player Parameter Geter 
// ----------------------------------------------------------------------------------------------------------------
const XMFLOAT3& Player_Get_POS()
{ 
	return Player_Pos; 
}

const XMFLOAT3& Player_Get_Aim_POS() 
{
	return Aim_Pos; 
}

float Player_Get_HP()
{
	return Player_HP;
}

float Player_Get_MaxHP()
{
	return Player_MaxHP;
}

void GUI_Set_Player_POS(float x, float y, float z)
{
	Player_Pos = { x, y, z };
}

// ----------------------------------------------------------------------------------------------------------------
//								        		 Player Debug Logic
// ----------------------------------------------------------------------------------------------------------------
void GUI_Set_Player_Health(float hp, float max)
{
	Player_HP = hp;
	Player_MaxHP = max;
}

void GUI_Set_Player_Stats(float atk, float def)
{
	Player_ATK = atk;
	Player_DEF = def;
}


// ----------------------------------------------------------------------------------------------------------------
//												Player Update Logic
//											  --- Movement System ---
// ----------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------
//												--- Input ---
// ----------------------------------------------------------------------------------------------------------------
XMVECTOR Player_Update_Movement_Input()
{
	Is_Input_Moving = false;

	// Get Input Directions (Based on Camera Front)
	XMVECTOR Flat_Front = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // Map W/S to Y Axis
	XMVECTOR Flat_Right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f); // Map A/D to X Axis
	XMVECTOR Input_Dir = XMVectorZero();

	// Get Keyboard, D-Pad Input
	if (KeyLogger_IsPressed(KK_W) || XKeyLogger_IsPadPressed(XINPUT_GAMEPAD_DPAD_UP))    
		Input_Dir += Flat_Front;
	if (KeyLogger_IsPressed(KK_S) || XKeyLogger_IsPadPressed(XINPUT_GAMEPAD_DPAD_DOWN)) 
		Input_Dir -= Flat_Front;
	if (KeyLogger_IsPressed(KK_D) || XKeyLogger_IsPadPressed(XINPUT_GAMEPAD_DPAD_RIGHT))
		Input_Dir += Flat_Right;
	if (KeyLogger_IsPressed(KK_A) || XKeyLogger_IsPadPressed(XINPUT_GAMEPAD_DPAD_LEFT))  
		Input_Dir -= Flat_Right;

	// Get Left Stick Input
	XMFLOAT2 Stick = XKeyLogger_GetLeftStick();
	float Stick_Magnitude = sqrtf(Stick.x * Stick.x + Stick.y * Stick.y);

	if (Stick_Magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		XMVECTOR Direction = (Flat_Front * (Stick.y / 32767.0f)) + (Flat_Right * (Stick.x / 32767.0f));
		Input_Dir += Direction;
	}

	// Normalize Input Direction And Check If Moving
	XMVECTOR Length_SQ = XMVector3LengthSq(Input_Dir);
	float Sqrt = XMVectorGetX(Length_SQ);

	if (Sqrt > 0.0001f)
	{
		Is_Input_Moving = true;
		return XMVector3Normalize(Input_Dir);
	}

	return Input_Dir;
}

void Player_Update_Aim_Input(float dt)
{
	float Limit_X = Get_Player_Limit_X();
	float Limit_Y_Min = Get_Player_Limit_Y_Min();
	float Limit_Y_Max = Get_Player_Limit_Y_Max();

	// Gamepad R-Stick
	XMFLOAT2 RStick = XKeyLogger_GetRightStick();
	float Stick_Mag = sqrtf(RStick.x * RStick.x + RStick.y * RStick.y);

	if (Stick_Mag > 8689.0f) // Gamepad R-Stick Aiming
	{
		Aim_Pos.x += (RStick.x / 32767.0f) * Aim_Move_Speed * dt;
		Aim_Pos.y += (RStick.y / 32767.0f) * Aim_Move_Speed * dt;

		float Pad_Limit_X = Get_Player_Limit_X() * 1.5f;
		Aim_Pos.x = ClampFloat(Aim_Pos.x, -Pad_Limit_X, Pad_Limit_X);
	}
	else // Mouse Aiming
	{
		int Mouse_X = KeyLogger_GetMouse_MoveX();
		int Mouse_Y = KeyLogger_GetMouse_MoveY();

		float w = static_cast<float>(Window_Manager::GetInstance()->GetWidth());
		float h = static_cast<float>(Window_Manager::GetInstance()->GetHeight());

		if (w > 0 && h > 0)
		{
			// Change 2D Pixel To 3D Ray
			XMMATRIX View = XMLoadFloat4x4(&Player_Camera_Get_View_Matrix());
			XMMATRIX Proj = XMLoadFloat4x4(&Player_Camera_Get_Proj_Matrix());
			XMMATRIX World = XMMatrixIdentity();

			XMVECTOR V_Near = XMVectorSet(static_cast<float>(Mouse_X), static_cast<float>(Mouse_Y), 0.0f, 1.0f);
			XMVECTOR V_Far = XMVectorSet(static_cast<float>(Mouse_X), static_cast<float>(Mouse_Y), 1.0f, 1.0f);

			XMVECTOR V_Origin = XMVector3Unproject(V_Near, 0, 0, w, h, 0.0f, 1.0f, Proj, View, World);
			XMVECTOR V_Dest = XMVector3Unproject(V_Far, 0, 0, w, h, 0.0f, 1.0f, Proj, View, World);

			XMVECTOR V_Dir = XMVector3Normalize(V_Dest - V_Origin);

			if (abs(XMVectorGetZ(V_Dir)) > 0.0001f)
			{
				float T_Hit = -XMVectorGetZ(V_Origin) / XMVectorGetZ(V_Dir);
				XMVECTOR V_Hit = V_Origin + V_Dir * T_Hit;

				Aim_Pos.x = XMVectorGetX(V_Hit);
				Aim_Pos.y = XMVectorGetY(V_Hit);
				Aim_Pos.z = 0.0f; 
			}
		}
	}
}

void Player_Update_Weapon_Logic(float dt)
{
	// Weapon Change
	if (KeyLogger_IsTrigger(KK_E) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_Y))
	{
		Current_Weapon = (Current_Weapon == WeaponType::MACHINE_GUN) ? WeaponType::MISSILE : WeaponType::MACHINE_GUN;
		Weapon_Manager::GetInstance().Missile_Lock_On_List_Clear();	// If Weapon Changed, Cancel Lock On 
	}

	// 1. Lock On Logic
	// IF Current Weapon Is Missile, Lock On Logic
	if (Current_Weapon == WeaponType::MISSILE)
	{
		Weapon_Manager::GetInstance().Missile_Lock_On();
	}
	else
	{
		// Not Missile, Clear Lock On List
		Weapon_Manager::GetInstance().Missile_Lock_On_List_Clear();
	}

	// 2. Fire Logic
	bool Is_Firing = KeyLogger_IsMousePressed(LEFT) || (XKeyLogger_GetRightTrigger() > XINPUT_GAMEPAD_TRIGGER_THRESHOLD);

	if (Is_Firing && Fire_Cooldown <= 0.0f)
	{
		XMVECTOR V_Player = XMLoadFloat3(&Player_Pos);

		if (Current_Weapon == WeaponType::MACHINE_GUN)
		{
			Weapon_Manager::GetInstance().Machine_Gun_Fire(V_Player, Player_ATK);
		}
		else if (Current_Weapon == WeaponType::MISSILE)
		{
			Weapon_Manager::GetInstance().Missile_Fire(Player_ATK);
		}
	}
}
