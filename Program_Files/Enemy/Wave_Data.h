/*==============================================================================
    
    Define Enemy Spawn Waves [Wave_Data.h]
    
    Author : Choi HyungJoon

==============================================================================*/
#ifndef WAVE_DATA_H
#define WAVE_DATA_H
#include "Enemy.h"
// For Prototype, Delete After
constexpr float Wave_End_Time = 240;

/*==============================================================================
// --- Wave Time Line ---
==============================================================================*/
struct WaveInfo
{
    float       StartTime;     
    float       EndTime;       
    float       SpawnInterval; 
    EnemyType   Type;          
    int         BatchCount;    
	float       X_Ratio;
};

static const WaveInfo Wave_Data[] =
{
    // Start ~ 10 Sec
    { 0.0f,  10.0f, 2.0f, EnemyType::ENEMY_NORMAL, 2, 0.5f },

    // 10 ~ 40 Sec
    { 10.0f, 40.0f, 5.0f, EnemyType::ENEMY_NORMAL, 1, 0.8f },

    // 30 ~ 60 Sec
    { 30.0f, 60.0f, 2.0f, EnemyType::ENEMY_NORMAL, 2, 0.2f },
    { 30.0f, 60.0f, 5.0f, EnemyType::ENEMY_NORMAL, 1, 0.9f },
};

static const int Wave_Data_Count = sizeof(Wave_Data) / sizeof(WaveInfo);

#endif // WAVE_DATA_H