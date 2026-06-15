/*==============================================================================

	Manage Weapon System Logic [Weapon_System.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef WEAPON_MANAGER_H
#define WEAPON_MANAGER_H
#include <vector>
#include "Enemy.h"

enum class WeaponType
{
	MACHINE_GUN,
	MISSILE
};

struct LockOn_Data
{
	Enemy* Target_Ptr;
	int Target_ID;
};

class Weapon_Manager
{
public:
	static Weapon_Manager& GetInstance()
	{
		static Weapon_Manager instance;
		return instance;
	}
	
	void Init();

	void Missile_Lock_On();
	void Missile_Lock_On_List_Clear();

	void Missile_Fire(float Damage);
	void Machine_Gun_Fire(DirectX::XMVECTOR V_Player, float Damage);

	void Set_Missile_Random_POS(float X, float Y_Min, float Y_Max, float Z);

	const std::vector<LockOn_Data> Return_Lock_On_List();

private:
	static std::vector<LockOn_Data> Locked_Targets;
	static constexpr size_t MAX_LOCK_ON = 8;

	float Random_X_Ratio = 2.0f;
	float Random_Y_Range_MIN = 0.5f;
	float Random_Y_Range_MAX = 2.0f;
	float Missile_Z_Start_POS = 1.0f;
};
#endif // WEAPON_MANAGER_H