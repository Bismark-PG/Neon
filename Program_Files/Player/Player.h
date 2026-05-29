/*==============================================================================

	Manage Player Logic [Player.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef PLAYER_H
#define PLAYER_H
#include "Collision.h"

void Player_Initialize(const DirectX::XMFLOAT3& First_POS, const DirectX::XMFLOAT3& First_Front);
void Player_Finalize();

void Player_Update(float elapsed_time);

void Player_Reset();
void Player_Draw();

// --- Parameter Setter  ---
void Player_Set_POS(DirectX::XMFLOAT3& POS);
void Player_Damaged(int damage);
void Player_LevelUp();

// --- Parameter Geter  ---
const DirectX::XMFLOAT3& Player_Get_POS();
const DirectX::XMFLOAT3& Player_Get_Front();
int Player_Get_HP();
int Player_Get_MaxHP();

// --- Debug Function  ---
void GUI_Set_Player_Speed(float speed);
void GUI_Set_Player_Health(float hp, float max);
void GUI_Set_Player_Stats(float atk, float def);

#endif // PLAYER_H