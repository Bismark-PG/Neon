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
using namespace DirectX;
using namespace PALETTE;

#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD    30

// Player Physics Constants
static XMFLOAT3 Player_Pos = {};
static XMFLOAT3 Player_Start_POS = {};
static XMFLOAT3 Player_Front = { 0.0f, 0.0f, 1.0f };
static float Player_Speed = 35.0f;

// Player resource
static int Player_HP = 100;
static int Player_MaxHP = 100;

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
	Player_Start_POS = First_POS; 
	XMStoreFloat3(&Player_Front, XMVector3Normalize(XMLoadFloat3(&First_Front)));
}

void Player_Finalize()
{
}

void Player_Update(double elapsed_time)
{
	XMFLOAT3 camFront = Player_Camera_Get_Front();
	XMVECTOR frontVec = XMVectorSet(camFront.x, 0.0f, camFront.z, 0.0f);

	if (XMVectorGetX(XMVector3LengthSq(frontVec)) > 0.0001f)
	{
		frontVec = XMVector3Normalize(frontVec);
		XMStoreFloat3(&Player_Front, frontVec);
	}

	XMVECTOR currentMoveDir = XMVectorZero();
	currentMoveDir = Player_Update_Is_Moved(currentMoveDir);

	if (Is_Input_Moving)
	{
		XMVECTOR posVec = XMLoadFloat3(&Player_Pos);
		posVec += currentMoveDir * Player_Speed * static_cast<float>(elapsed_time);
		posVec = XMVectorSetY(posVec, 0.5f);

		XMStoreFloat3(&Player_Pos, posVec);
	}
}

void Player_Draw()
{
	float yaw = atan2f(Player_Front.x, Player_Front.z);

	XMMATRIX scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	XMMATRIX rot = XMMatrixRotationY(yaw);
	XMMATRIX trans = XMMatrixTranslation(Player_Pos.x, Player_Pos.y, Player_Pos.z);

	XMMATRIX worldMatrix = scale * rot * trans;

	Cube_Draw(worldMatrix, Shader_Filter::ANISOTROPIC, Cube_Type::BOX);
}

void Player_Reset()
{
	// 1. Movement Reset
	Player_Pos = Player_Start_POS;

	// 2. Resource Reset
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
}

int Player_Get_HP()
{
	return Player_HP;
}

int Player_Get_MaxHP()
{
	return Player_MaxHP;
}

void Player_LevelUp()
{
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

	// Get Front Vector From Camera (Remove Y Axis And Normalize)
	XMFLOAT3 camFront = Player_Camera_Get_Front();
	XMVECTOR Flat_Front = XMVectorSet(camFront.x, 0.0f, camFront.z, 0.0f);
	Flat_Front = XMVector3Normalize(Flat_Front);

	// Get Right Vector From Camera (Remove Y Axis And Normalize)
	XMVECTOR UpVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Flat_Right = XMVector3Cross(UpVector, Flat_Front);
	Flat_Right = XMVector3Normalize(Flat_Right);

	XMVECTOR InputDir = XMVectorZero();

	// Get Keyboard Input
	if (KeyLogger_IsPressed(KK_W)) InputDir += Flat_Front;
	if (KeyLogger_IsPressed(KK_S)) InputDir -= Flat_Front;
	if (KeyLogger_IsPressed(KK_D)) InputDir += Flat_Right;
	if (KeyLogger_IsPressed(KK_A)) InputDir -= Flat_Right;

	// Get D-Pad Input
	if (XKeyLogger_IsPadPressed(XINPUT_GAMEPAD_DPAD_UP))    InputDir += Flat_Front;
	if (XKeyLogger_IsPadPressed(XINPUT_GAMEPAD_DPAD_DOWN))  InputDir -= Flat_Front;
	if (XKeyLogger_IsPadPressed(XINPUT_GAMEPAD_DPAD_RIGHT)) InputDir += Flat_Right;
	if (XKeyLogger_IsPadPressed(XINPUT_GAMEPAD_DPAD_LEFT))  InputDir -= Flat_Right;

	// Get Left Stick Input
	XMFLOAT2 Stick = XKeyLogger_GetLeftStick();
	float Stick_Magnitude = sqrtf(Stick.x * Stick.x + Stick.y * Stick.y);

	if (Stick_Magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		XMVECTOR WorldStickDir = (Flat_Front * (Stick.y / 32767.0f)) + (Flat_Right * (Stick.x / 32767.0f));
		InputDir += WorldStickDir;
	}

	// Normalize Input Direction And Check If Moving
	XMVECTOR LengthSq = XMVector3LengthSq(InputDir);
	float lenSq = XMVectorGetX(LengthSq);

	if (lenSq > 0.0001f)
	{
		Is_Input_Moving = true;
		return Vel + XMVector3Normalize(InputDir);
	}

	return Vel;
}

// ----------------------------------------------------------------------------------------------------------------
//											--- Parameter Geter  ---
// ----------------------------------------------------------------------------------------------------------------