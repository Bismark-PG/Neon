/*==============================================================================
   
   Manage Enemy Spawning Logic [Enemy_Spawner.cpp]
   
   Author : Choi HyungJoon

==============================================================================*/
#include "Project_Header.h"
#include "Enemy_Spawner.h"
#include "Enemy_Manager.h"
#include "Player.h"
#include "debug_ostream.h"
#include "Heapler_Logic.h"
#include "Wave_Data.h"
#include "Player_Camera.h"
#include "Palette.h"
using namespace DirectX;
using namespace PALETTE;
static float SPAWN_Z_DEPTH = 150.0f;

void Enemy_Spawner::Init()
{
    m_GameTime = 0.0f;
    m_WaveTimers.clear();
    m_WaveTimers.resize(Wave_Data_Count, 0.0f);
}

void Enemy_Spawner::Reset()
{
    Init();
}

void Enemy_Spawner::Set_Z_Depth(float depth)
{
    SPAWN_Z_DEPTH = depth;
}

void Enemy_Spawner::Update(float dt)
{
    m_GameTime += dt;

    for (int i = 0; i < Wave_Data_Count; ++i)
    {
        const WaveInfo& wave = Wave_Data[i];

        if (m_GameTime >= wave.StartTime && m_GameTime < wave.EndTime)
        {
            m_WaveTimers[i] -= dt;

            if (m_WaveTimers[i] <= 0.0f)
            {
                m_WaveTimers[i] = wave.SpawnInterval;

                for (int count = 0; count < wave.BatchCount; ++count)
                {
                    XMFLOAT3 finalPos = Get_Spawn_Position(wave.Type, wave.X_Ratio, count);
                    Enemy_Manager::GetInstance().Spawn(wave.Type, finalPos);
                }
            }
        }
    }
}

XMFLOAT3 Enemy_Spawner::Get_Spawn_Position(EnemyType type, float ratio_X, int batchIndex)
{
	// 1. Get X Axis : Range Is Based On World_Limit_X, But Can Be Adjusted By Ratio_X (0.0 ~ 1.0)
    float limitX = Get_Player_Limit_X();
    float minX = -limitX;
    float maxX = limitX;
    float totalWidth = maxX - minX;

    float spawnX = minX + (ratio_X * totalWidth);

    // Add Spacing For Each Batch
    spawnX += (batchIndex * 2.5f);  // 2.5f Is An Arbitrary Walue, Can Be Adjusted

	// 2. Get Y Axis: Dynamically Assign Height Based On Separated World_Limit_Y For Each Type
    float limitY_Min = Get_Player_Limit_Y_Min();
    float limitY_Max = A_Half * Get_Player_Limit_Y_Max();
    float spawnY = 0.0f;

    switch (type)
    {
    case EnemyType::ENEMY_NORMAL:
        spawnY = limitY_Max * A_F_Fifths;
        break;
    }

    float spawnZ = SPAWN_Z_DEPTH;

    return XMFLOAT3(spawnX, spawnY, spawnZ);
}