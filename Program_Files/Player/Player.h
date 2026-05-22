/*==============================================================================

	Manage Player Logic [Player.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef PLAYER_H
#define PLAYER_H
#include "Collision.h"

enum class E_PlayerState
{
	IDLE,
	WALK,
	JUMP
};

void Player_Initialize(const DirectX::XMFLOAT3& First_POS, const DirectX::XMFLOAT3& First_Front);
void Player_Finalize();

void Player_Update(double elapsed_time);

void Player_Draw();

void Player_Reset();

void Player_Set_POS(DirectX::XMFLOAT3& POS);
const DirectX::XMFLOAT3& Player_Get_POS();
const DirectX::XMFLOAT3& Player_Get_Front();

AABB Player_Get_AABB();

void Player_OnDamage(int damage);
int Player_Get_HP();
int Player_Get_MaxHP();

void Player_LevelUp();

// --- Parameter Geter  ---

#endif // PLAYER_H