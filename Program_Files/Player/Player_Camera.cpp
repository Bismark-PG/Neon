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

using namespace DirectX;

// Controller Constants
constexpr float CONTROLLER_STICK_MAX = 32767.0f; // Max Value of Short
constexpr float CONTROLLER_STICK_WEIGHT = 800.0f;   // Pad Input Multiplier (Higher = Faster)
constexpr float CAMERA_ROTATION_SCALE = 0.005f;

#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689

// Player Camera Parameter
static float Camera_Yaw = 0.0f, Camera_Pitch = 0.3f;
static float Camera_Near_Z = 0.1f, Camera_Far_z = 1000.0f;

// Sensitivity
static float Mouse_Sensitivity = 0.005f; // Base Setting (From Option)
static float Apply_Sensitivity = 0.005f; // Real used value

// Matrices & Pos
static DirectX::XMFLOAT4X4 Camera_View_mtx;
static XMFLOAT3 Current_Camera_Pos = { 0.0f, 5.0f, -15.0f };
static XMFLOAT3 Camera_Front = { 0.0f, 0.0f, 1.0f };
static XMFLOAT3 Camera_POS = {};

void Player_Camera_Initialize()
{
}

void Player_Camera_Finalize()
{
}

void Player_Camera_Reset()
{
    Camera_Yaw = 0.0f;
    Camera_Pitch = 0.0f;

    Apply_Sensitivity = Mouse_Sensitivity;
}

void Player_Camera_Update(double elapsed_time)
{
    float dt = static_cast<float>(elapsed_time);

    // --- Rotation Logic ---
    // Get Mouse Movement
    Camera_Yaw = KeyLogger_GetMouse_MoveX() * Apply_Sensitivity;
    Camera_Pitch = KeyLogger_GetMouse_MoveY() * Apply_Sensitivity;

    // Limited Pitch 
    if (Camera_Pitch > 1.2f) Camera_Pitch = 1.2f;
    if (Camera_Pitch < -0.2f) Camera_Pitch = -0.2f;

    // --- Position Calculation ---
    // Get Player POS
    XMFLOAT3 Player_POS = Player_Get_POS();
    XMVECTOR vPlayer_POS = XMLoadFloat3(&Player_POS);

    // Get Camera Forward
    XMMATRIX Rotation = XMMatrixRotationRollPitchYaw(Camera_Pitch, Camera_Yaw, 0.0f);
    XMVECTOR vForward = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), Rotation);
    XMStoreFloat3(&Camera_Front, vForward);

    // Get Target Camera POS
    float camDist = 15.0f;
    XMVECTOR vDesired_POS = vPlayer_POS - (vForward * camDist);

    // Camera Lerp
    XMVECTOR vCurrent_POS = XMLoadFloat3(&Current_Camera_Pos);
    float Lerp_Speed = 10.0f;
    vCurrent_POS = XMVectorLerp(vCurrent_POS, vDesired_POS, static_cast<float>(elapsed_time) * Lerp_Speed);
    XMStoreFloat3(&Current_Camera_Pos, vCurrent_POS);

    // --- Matrix Update ---
    // Make View Matrix
    XMMATRIX mtxView = XMMatrixLookAtLH(vCurrent_POS, vCurrent_POS + vForward, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
    Shader_Manager::GetInstance()->SetViewMatrix3D(mtxView);

    //--- Projection Matrix ---
    float Ratio = static_cast<float>(Direct3D_GetBackBufferWidth()) / Direct3D_GetBackBufferHeight();
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
float Player_Camera_Get_Yaw()
{
    return Camera_Yaw;
}
float Player_Camera_Get_Pitch()
{
    return Camera_Pitch;
}

float Player_Camera_Get_Far_Z()
{
    return Camera_Far_z;
}

DirectX::XMFLOAT4X4& Player_Camera_Get_View_Matrix()
{
    return Camera_View_mtx;
}

