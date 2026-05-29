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
using namespace DirectX;
using namespace PALETTE;

#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD    30

// --- System Constraints ---
static constexpr float CAMERA_SIZE_BASE = 3.0f;
static constexpr float WORLD_LIMIT_SCALE = 2.0f;
static constexpr float COORD_SCALE = 10.0f;

static constexpr float MAX_WORLD_LIMIT = (CAMERA_SIZE_BASE * WORLD_LIMIT_SCALE * 0.5f) * COORD_SCALE;

// Player Physics Constants
static XMFLOAT3 Player_Pos = {};
static XMFLOAT3 Player_Start_POS = {};
static XMFLOAT3 Player_Front = { 0.0f, 0.0f, 1.0f };
static float Player_Speed = 35.0f;

// Player resource
int Billboard_Player::Player_ID = -1;
static Billboard_Player* Player_OBJ = nullptr;
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
static XMVECTOR Player_Update_Is_Moved(XMVECTOR Vel);
// ----------------------------------------------------------

void Player_Initialize(const XMFLOAT3& First_POS, const XMFLOAT3& First_Front)
{
	Player_Pos = First_POS;
	Player_Front = First_Front;
	Player_Start_POS = First_POS;

	Billboard_Player::Initialize_Resource();

	if (Player_OBJ == nullptr)
	{
		Player_OBJ = new Billboard_Player(Billboard_Player::Player_ID, First_POS, 2.0f, 2.0f);
	}

	Player_OBJ->Activate(First_POS);
}

void Player_Finalize()
{
	if (Player_OBJ)
	{
		delete Player_OBJ;
		Player_OBJ = nullptr;
	}
}

void Player_Update(float elapsed_time)
{
	Player_Front = { 0.0f, 0.0f, 1.0f };

	XMVECTOR Current_Direction = XMVectorZero();
	Current_Direction = Player_Update_Is_Moved(Current_Direction);

	if (Is_Input_Moving)
	{
		XMVECTOR POS_Vector = XMLoadFloat3(&Player_Pos);
		POS_Vector += Current_Direction * Player_Speed * static_cast<float>(elapsed_time);

		// š Limit Player Movement inside the World Limit Box š
		float Player_Limit = MAX_WORLD_LIMIT;

		float Clamped_X = ClampFloat(XMVectorGetX(POS_Vector), -Player_Limit, Player_Limit);
		float Clamped_Y = ClampFloat(XMVectorGetY(POS_Vector), -Player_Limit, Player_Limit);

		POS_Vector = XMVectorSetX(POS_Vector, Clamped_X);
		POS_Vector = XMVectorSetY(POS_Vector, Clamped_Y);

		// š Condition: Player Z is rigidly fixed at 0.0f š
		POS_Vector = XMVectorSetZ(POS_Vector, 0.0f);

		XMStoreFloat3(&Player_Pos, POS_Vector);

		if (Player_OBJ)
		{
			Player_OBJ->Reset_State(Billboard_Player::Player_ID, Player_Pos, 2.0f, 2.0f);
			Player_OBJ->Activate(Player_Pos);
			Player_OBJ->Update(elapsed_time);
		}
	}
}

void Player_Reset()
{
	// 1. Movement Reset
	Player_Pos = Player_Start_POS;

	// 2. Resource Reset
}

void Player_Draw()
{
	float yaw = atan2f(Player_Front.x, Player_Front.z);

	XMMATRIX scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	XMMATRIX rot = XMMatrixRotationY(yaw);
	XMMATRIX trans = XMMatrixTranslation(Player_Pos.x, Player_Pos.y, Player_Pos.z);

	XMMATRIX worldMatrix = scale * rot * trans;

	if (Player_OBJ)
	{
		Player_OBJ->Draw();
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

void Player_LevelUp()
{
}

// ----------------------------------------------------------------------------------------------------------------
//											    Player Parameter Geter 
// ----------------------------------------------------------------------------------------------------------------
const XMFLOAT3& Player_Get_POS()
{
	return Player_Pos;
}

const XMFLOAT3& Player_Get_Front()
{
	return Player_Front;
}

int Player_Get_HP()
{
	return Player_HP;
}

int Player_Get_MaxHP()
{
	return Player_MaxHP;
}

// ----------------------------------------------------------------------------------------------------------------
//								        		 Player Debug Logic
// ----------------------------------------------------------------------------------------------------------------
void GUI_Set_Player_Speed(float speed)
{
	Player_Speed = speed;
}

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
XMVECTOR Player_Update_Is_Moved(XMVECTOR Vel)
{
	Is_Input_Moving = false;

	// Get Input Directions (Based on Camera Front)
	XMVECTOR Flat_Front = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // Map W/S to Y Axis
	XMVECTOR Flat_Right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f); // Map A/D to X Axis
	XMVECTOR Input_Dir = XMVectorZero();

	// Get Keyboard Input
	if (KeyLogger_IsPressed(KK_W)) Input_Dir += Flat_Front;
	if (KeyLogger_IsPressed(KK_S)) Input_Dir -= Flat_Front;
	if (KeyLogger_IsPressed(KK_D)) Input_Dir += Flat_Right;
	if (KeyLogger_IsPressed(KK_A)) Input_Dir -= Flat_Right;

	// Get D-Pad Input
	if (XKeyLogger_IsPadPressed(XINPUT_GAMEPAD_DPAD_UP))    Input_Dir += Flat_Front;
	if (XKeyLogger_IsPadPressed(XINPUT_GAMEPAD_DPAD_DOWN))  Input_Dir -= Flat_Front;
	if (XKeyLogger_IsPadPressed(XINPUT_GAMEPAD_DPAD_RIGHT)) Input_Dir += Flat_Right;
	if (XKeyLogger_IsPadPressed(XINPUT_GAMEPAD_DPAD_LEFT))  Input_Dir -= Flat_Right;

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
		return Vel + XMVector3Normalize(Input_Dir);
	}

	return Vel;
}