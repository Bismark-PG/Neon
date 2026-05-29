/*==============================================================================

	Make Camera [Player_Camera.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef PLAYER_CAMERA_H
#define PLAYER_CAMERA_H

void Player_Camera_Initialize();
void Player_Camera_Finalize();

void Player_Camera_Reset();
void Player_Camera_Update(float elapsed_time);

const DirectX::XMFLOAT3& Player_Camera_Get_POS();
const DirectX::XMFLOAT3& Player_Camera_Get_Front();

void Set_Mouse_Sensitivity(float Sensitivity);
float Get_Mouse_Sensitivity();

const DirectX::XMFLOAT3& Player_Camera_Get_Current_POS();
float Player_Camera_Get_Far_Z();

const DirectX::XMFLOAT4X4& Player_Camera_Get_View_Matrix();

// --- Debug Function  ---
void GUI_Set_Camera_Offset(float offsetX, float offsetY, float offsetZ);
void GUI_Set_Camera_Pitch(float pitch);
void GUI_Set_Camera_Lerp_Speed(float speed);
void GUI_Set_Camera_Size_Base(float size);

#endif //PLAYER_CAMERA_H
