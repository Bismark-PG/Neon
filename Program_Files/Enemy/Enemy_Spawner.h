/*==============================================================================
    
    Manage Enemy Spawning Logic [Enemy_Spawner.h]

    Author : Choi HyungJoon

==============================================================================*/
#ifndef ENEMY_SPAWNER_H
#define ENEMY_SPAWNER_H

#include <vector>
#include <DirectXMath.h>
#include "Enemy_Type.h"

class Enemy_Spawner
{
public:
    static Enemy_Spawner& GetInstance()
    {
        static Enemy_Spawner instance;
        return instance;
    }

    void Init();
    void Update(float dt);
    void Reset();

    float GetCurrentGameTime() const { return m_GameTime; }
    
    void Set_Z_Depth(float depth);

private:
    Enemy_Spawner() = default;
    ~Enemy_Spawner() = default;
    Enemy_Spawner(const Enemy_Spawner&) = delete;
    Enemy_Spawner& operator=(const Enemy_Spawner&) = delete;

    DirectX::XMFLOAT3 Get_Spawn_Position(EnemyType type, float ratio_X, int batchIndex);

    float m_GameTime = 0.0f;
    std::vector<float> m_WaveTimers;
};
#endif // ENEMY_SPAWNER_H