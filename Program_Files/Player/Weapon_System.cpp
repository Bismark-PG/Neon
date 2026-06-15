
/*==============================================================================

	Manage Weapon System Logic [Weapon_System.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Project_Header.h"
#include "Weapon_System.h"
#include "Player_Camera.h"
#include "Player.h"
#include "Enemy_Manager.h"
#include "Bullet_Manager.h"
#include "Heapler_Logic.h"

using namespace DirectX;

std::vector<LockOn_Data> Weapon_Manager::Locked_Targets;

void Weapon_Manager::Init()
{
}

void Weapon_Manager::Missile_Lock_On()
{
	// If Enemy Is Dead, Clear In Target List
	auto it = std::remove_if(Locked_Targets.begin(), Locked_Targets.end(),
		[](const LockOn_Data& L) { return !L.Target_Ptr->IsActive() || L.Target_Ptr->GetUniqueID() != L.Target_ID; });
	Locked_Targets.erase(it, Locked_Targets.end());

	// Get Ray For Missile Target Pos
	XMFLOAT3 Cam_Pos = Player_Camera_Get_POS();
	XMVECTOR V_Cam = XMLoadFloat3(&Cam_Pos);
	XMVECTOR V_Aim = XMLoadFloat3(&Player_Get_Aim_POS());
	XMVECTOR V_RayDir = XMVector3Normalize(V_Aim - V_Cam);

	XMFLOAT3 RayDir;
	XMStoreFloat3(&RayDir, V_RayDir);

	if (RayDir.z > 0.0001f) // If Look Forward
	{
		for (Enemy* E : Enemy_Manager::GetInstance().Get_Active_List())
		{
			if (Locked_Targets.size() >= MAX_LOCK_ON) break; // Limit Max Size

			// Already Lock On, Do Pass
			bool already_locked = false;
			for (const LockOn_Data& L : Locked_Targets) {
				if (L.Target_ID == E->GetUniqueID()) { already_locked = true; break; }
			}
			if (already_locked) continue;

			// When Z Is Same To Enemy, Get Aim POS
			XMFLOAT3 E_Pos = E->GetPosition();
			float Hit_T = (E_Pos.z - Cam_Pos.z) / RayDir.z;
			float Hit_X = Cam_Pos.x + RayDir.x * Hit_T;
			float Hit_Y = Cam_Pos.y + RayDir.y * Hit_T;

			// If In Radius, Lock On
			float Lock_Radius = 4.0f; // Can Change Lock On Radius
			if (abs(Hit_X - E_Pos.x) < Lock_Radius && abs(Hit_Y - E_Pos.y) < Lock_Radius)
			{
				// Save Lock On Data With Enemy Pointer And Enemy ID
				Locked_Targets.push_back({ E, E->GetUniqueID() });
			}
		}
	}
}

void Weapon_Manager::Missile_Lock_On_List_Clear()
{
	Locked_Targets.clear();
}

void Weapon_Manager::Missile_Fire(float Damage)
{
	if (!Locked_Targets.empty())
	{
		// Shoot Lock On Target
		for (const LockOn_Data& L : Locked_Targets)
		{
			// Random Direction For Visual Effect
			float Random_X = RandomFloatMinus1To1() * Random_X_Ratio;
			float Random_Y = RandomFloatRange(Random_Y_Range_MIN, Random_Y_Range_MAX);
			XMFLOAT3 Start_Dir = { Random_X, Random_Y, Missile_Z_Start_POS };

			Bullet_Manager::Instance().Fire_Missile(Player_Get_POS(), Start_Dir, Damage * 3, L.Target_Ptr);
		}
		// End Shoot, Clear List
		Locked_Targets.clear();
		Player_Fire_Interval(1.0f);	// Shoot For 1/Sec
	}
}

void Weapon_Manager::Machine_Gun_Fire(XMVECTOR V_Player, float Damage)
{
	// Shoot Ray In Aim POS
	XMVECTOR V_Aim = XMLoadFloat3(&Player_Get_Aim_POS());
	XMVECTOR V_Dir = XMVector3Normalize(V_Aim - V_Player);
	XMFLOAT3 Dir;
	XMStoreFloat3(&Dir, V_Dir);

	Bullet_Manager::Instance().Fire_Ray(Player_Get_POS(), Dir, Damage);
	Player_Fire_Interval(0.25f); // Shoot For 4/Sec
}

void Weapon_Manager::Set_Missile_Random_POS(float X, float Y_Min, float Y_Max, float Z)
{
	Random_X_Ratio		= X;
	Random_Y_Range_MIN	= Y_Min;
	Random_Y_Range_MAX	= Y_Max;
	Missile_Z_Start_POS = Z;
}

const std::vector<LockOn_Data> Weapon_Manager::Return_Lock_On_List()
{
	return Locked_Targets;
}
