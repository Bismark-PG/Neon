/*==============================================================================

	Manage Enemy Type [Enemy_Type.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef ENEMY_TYPE_H
#define ENEMY_TYPE_H
enum class EnemyType
{
	ENEMY_NORMAL,
};

struct Enemy_Info
{
	EnemyType Type;
	int HP, MaxHP;
	float Speed;
	float Scale;
	float Bullet_Size, Bullet_Speed;

	int Collision_Damage;
	int Bullet_Damage;   
};

// --- Normal Type--- 
static const Enemy_Info Enemy_Normal_Info
{	// Enemy Type
	EnemyType::ENEMY_NORMAL,
	// HP, Max_HP, Speed, Scale, Bullet_Size, Bullet_Speed
	10, 10, 10.0f, 1.0f, 1.0f, 10.0f,
	//Collision_Damage, Bullet_Damage
	1, 2
};

#endif // ENEMY_TYPE_H