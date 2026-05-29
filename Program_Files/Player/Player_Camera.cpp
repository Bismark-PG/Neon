/*==============================================================================

	Make Camera [Player_Camera.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Project_Header.h"
#include "Player_Camera.h"
#include "Player.h"
#include "Shader_Manager.h"
#include "direct3d.h"
#include "KeyLogger.h"
#include "debug_ostream.h"
#include "Game_Window.h"
#include "Heapler_Logic.h"
using namespace DirectX;

// Controller Constants
constexpr float CONTROLLER_STICK_MAX = 32767.0f; // Max Value of Short
constexpr float CONTROLLER_STICK_WEIGHT = 800.0f;   // Pad Input Multiplier (Higher = Faster)
constexpr float CAMERA_ROTATION_SCALE = 0.005f;
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689

// --- System Constraints (Based on Your Rules) ---
static constexpr float CAMERA_SIZE_BASE = 3.0f;     // Camera rect size
static constexpr float PLAYER_SIZE_BASE = 1.0f;     // Player rect size
static constexpr float WORLD_LIMIT_SCALE = 2.0f;    // Action range multiplier
static constexpr float COORD_SCALE = 10.0f;         // Real 3D Scale Multiplier

// Calculated Max Limits
static constexpr float MAX_WORLD_LIMIT = (CAMERA_SIZE_BASE * WORLD_LIMIT_SCALE * 0.5f) * COORD_SCALE;
static constexpr float MAX_CAM_OFFSET = ((CAMERA_SIZE_BASE - PLAYER_SIZE_BASE) * 0.5f) * COORD_SCALE;

// Player Camera Parameter
static float Camera_Near_Z = 0.1f, Camera_Far_z = 1000.0f;
static float Camera_Z_Dist = -15.0f;

// Sensitivity
static float Mouse_Sensitivity = 0.005f; // Base Setting (From Option)
static float Apply_Sensitivity = 0.005f; // Real used value

// Matrices & Pos
static DirectX::XMFLOAT4X4 Camera_View_mtx;
static XMFLOAT3 Current_Camera_Pos = { 0.0f, 5.0f, -15.0f };
static XMFLOAT3 Camera_Front = { 0.0f, 0.0f, 1.0f };
static XMFLOAT3 Camera_POS = {};

// Camera Parameters (Debug Adjustable)
static float Cam_Offset_X;
static float Cam_Offset_Y;
static float Cam_Offset_Z;
static float Cam_Pitch;
static float Cam_Lerp_Speed;
static float Camera_Size_Base;

void Player_Camera_Initialize()
{
    Current_Camera_Pos = { 0.0f, 0.0f, Camera_Z_Dist };
    Camera_Front = { 0.0f, 0.0f, 1.0f }; // Always look straight
    Camera_POS = Current_Camera_Pos;
}

void Player_Camera_Finalize()
{
}

void Player_Camera_Reset()
{
    Apply_Sensitivity = Mouse_Sensitivity;
}

void Player_Camera_Update(float elapsed_time)
{
	// Get Window Size
    float w = static_cast<float>(Window_Manager::GetInstance()->GetWidth());
    float h = static_cast<float>(Window_Manager::GetInstance()->GetHeight());

	// Get Mouse State
    Mouse_State mState;
    Mouse_GetState(&mState);

    // Map mouse position to screen center (-1.0 to 1.0)
    // Invert Y because 3D Up is +Y, but Screen Down is +Y
    float Mouse_X = (static_cast<float>(mState.x) / w) * 2.0f - 1.0f;
    float Mouse_Y = -(static_cast<float>(mState.y) / h) * 2.0f + 1.0f;

    Mouse_X = ClampFloat(Mouse_X, -1.0f, 1.0f);
    Mouse_Y = ClampFloat(Mouse_Y, -1.0f, 1.0f);

    float current_max_world_limit = (Camera_Size_Base * WORLD_LIMIT_SCALE * 0.5f) * COORD_SCALE;
    float current_max_cam_offset = ((Camera_Size_Base - PLAYER_SIZE_BASE) * 0.5f) * COORD_SCALE;

    // --- Position Calculation ---
    // Get Player POS
    XMFLOAT3 Player_POS = Player_Get_POS();

    float Target_X = Player_POS.x + (Mouse_X * current_max_cam_offset) + Cam_Offset_X;
    float Target_Y = Player_POS.y + (Mouse_Y * current_max_cam_offset) + Cam_Offset_Y;
    float Target_Z = Player_POS.z + Cam_Offset_Z;

    // Clamp Camera Position within World Limits
    float camWorldLimit = MAX_WORLD_LIMIT - (CAMERA_SIZE_BASE * 0.5f * COORD_SCALE);
    if (camWorldLimit < 0.0f) camWorldLimit = 0.0f;

    Target_X = ClampFloat(Target_X, -camWorldLimit, camWorldLimit);
    Target_Y = ClampFloat(Target_Y, -camWorldLimit, camWorldLimit);

    // Lerp Camera Position
    XMVECTOR vCurrentPos = XMLoadFloat3(&Current_Camera_Pos);
    XMVECTOR vDesiredPos = XMVectorSet(Target_X, Target_Y, Camera_Z_Dist, 0.0f);

    vCurrentPos = XMVectorLerp(vCurrentPos, vDesiredPos, elapsed_time * Cam_Lerp_Speed);
    XMStoreFloat3(&Current_Camera_Pos, vCurrentPos);
    Camera_POS = Current_Camera_Pos;

    // Update View Matrix
    XMMATRIX rotMatrix = XMMatrixRotationX(Cam_Pitch);
    XMVECTOR vForward = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotMatrix);
    XMVECTOR vUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    XMMATRIX mtxView = XMMatrixLookAtLH(vCurrentPos, vCurrentPos + vForward, vUp);

    XMStoreFloat4x4(&Camera_View_mtx, mtxView);
    Shader_Manager::GetInstance()->SetViewMatrix3D(mtxView);

    // Update Projection Matrix
    float Ratio = w / h;
    XMMATRIX mtxPerspective = XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), Ratio, Camera_Near_Z, Camera_Far_z);
    Shader_Manager::GetInstance()->SetProjectionMatrix3D(mtxPerspective);
}

const XMFLOAT3& Player_Camera_Get_POS()
{
    return Camera_POS;
}

const XMFLOAT3& Player_Camera_Get_Front()
{
    return Camera_Front;
}

void Set_Mouse_Sensitivity(float Sensitivity)
{
    Mouse_Sensitivity = Sensitivity;
}

float Get_Mouse_Sensitivity()
{
    return Mouse_Sensitivity;
}

const XMFLOAT3& Player_Camera_Get_Current_POS()
{
    return Current_Camera_Pos;
}

float Player_Camera_Get_Far_Z()
{
    return Camera_Far_z;
}

const XMFLOAT4X4& Player_Camera_Get_View_Matrix()
{
    return Camera_View_mtx;
}

// ----------------------------------------------------------------------------------------------------------------
//								        	Player Camera Debug Logic
// ----------------------------------------------------------------------------------------------------------------
void GUI_Set_Camera_Offset(float offsetX, float offsetY, float offsetZ)
{
	Cam_Offset_X = offsetX;
	Cam_Offset_Y = offsetY;
	Cam_Offset_Z = offsetZ;
}

void GUI_Set_Camera_Pitch(float pitch)
{
	Cam_Pitch = pitch;
}

void GUI_Set_Camera_Lerp_Speed(float speed)
{
	Cam_Lerp_Speed = speed;
}

void GUI_Set_Camera_Size_Base(float size)
{
	Camera_Size_Base = size;
}