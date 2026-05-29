/*==============================================================================

	Managed Billboard System [Billboard_Manager.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef BILLBOARD_MANAGER_H
#define BILLBOARD_MANAGER_H
#include "Project_Header.h"
#include "Collision.h"
#include "Billboard_Object.h"
#include "Billboard_Player.h"
#include "Billboard_Enemy.h"
#include "Billboard_Bullet.h"
#include "Billboard_Effect.h"

class Billboard_Object;

class Billboard_Manager
{
public:
    static Billboard_Manager& Instance()
    {
        static Billboard_Manager instance;
        return instance;
    }

    void Init();
    void Final();
    void Reset();

    void Update(double dt);
    void Draw();

    void Create_Enemy(const DirectX::XMFLOAT3& pos);
    void Create_Bullet(const DirectX::XMFLOAT3& pos);
    void Create_Effect(const DirectX::XMFLOAT3& pos, float scale, Effect_Type Type);

    Billboard_Enemy* Check_Enemy_Collision(const AABB& box);
    Billboard_Bullet* Check_Bullet_Collision(const AABB& box);

private:
    Billboard_Manager() = default;
    ~Billboard_Manager() = default;

	// Pool for Effects (e.g., explosion, smoke)
    static constexpr int MAX_EFFECT_POOL = 200;
    std::vector<class Billboard_Effect*> m_EffectPool;

	// Pool for Enemy Billboards
    static constexpr int MAX_ENEMY_POOL = 100;
    std::vector<class Billboard_Enemy*> m_EnemyPool;

	// Pool for bullets (e.g., player shots, enemy shots)
    static constexpr int MAX_BULLET_POOL = 300;
    std::vector<class Billboard_Bullet*> m_BulletPool;
};

#endif // BILLBOARD_MANAGER_H