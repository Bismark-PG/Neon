/*==============================================================================

	Managed Billboard System [Billboard_Manager.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Project_Header.h"
#include "Billboard_Manager.h"
#include "Texture_Manager.h"
#include "debug_ostream.h"
using namespace DirectX;

int Billboard_Enemy::Enemy_Normal_ID = -1;
int Billboard_Bullet::Bullet_Normal_ID = -1;
int Billboard_Effect::Explosion_Pattern_ID = -1;
int Billboard_Effect::Smoke_Pattern_ID = -1;

void Billboard_Manager::Init()
{
    m_EnemyPool.clear();
    m_BulletPool.clear();
    m_EffectPool.clear();

    Billboard_Enemy::Initialize_Resource();
    Billboard_Bullet::Initialize_Resource();
    Billboard_Effect::Initialize_Resource();

	// Billboard Object Pool
    for (int i = 0; i < MAX_ENEMY_POOL; ++i)
    {
        m_EnemyPool.push_back(new Billboard_Enemy(-1, { 0,0,0 }, 1.0f, 1.0f));
    }

    for (int i = 0; i < MAX_BULLET_POOL; ++i)
    {
        m_BulletPool.push_back(new Billboard_Bullet(-1, { 0,0,0 }, 1.0f, 1.0f));
    }

	// Billboard_Effect Pool
    for (int i = 0; i < MAX_EFFECT_POOL; ++i)
    {
        m_EffectPool.push_back(new Billboard_Effect());
    }
}

void Billboard_Manager::Final()
{
    for (auto* effect : m_EnemyPool) delete effect;
    m_EnemyPool.clear();
    
    for (auto* effect : m_BulletPool) delete effect;
    m_BulletPool.clear();

    for (auto* effect : m_EffectPool) delete effect;
    m_EffectPool.clear();
}

void Billboard_Manager::Reset()
{
    // Reset All Objects
    for (auto* obj : m_EnemyPool)
    {
        if (obj->IsActive())
        {
            obj->Deactivate();
        }
    }
    for (auto* obj : m_BulletPool)
    {
        if (obj->IsActive())
        {
            obj->Deactivate();
        }
    }

    // Reset All Effects
    for (auto* effect : m_EffectPool)
    {
        if (effect->IsActive())
        {
            effect->Deactivate();
        }
    }

    Debug::D_Out << "[Billboard] All Objects Reset to Pool." << std::endl;
}

void Billboard_Manager::Update(double dt)
{
    // Update All Objects
    for (auto* obj : m_EnemyPool)
    {
        if (obj->IsActive())
        {
            obj->Update(dt);
        }
    }
    for (auto* obj : m_BulletPool)
    {
        if (obj->IsActive())
        {
            obj->Update(dt);
        }
    }

    // Update All Effects
    for (auto* effect : m_EffectPool)
    {
        if (effect->IsActive())
        {
            effect->Update(dt);
        }
    }
}

void Billboard_Manager::Draw()
{
    // Draw All Objects
    for (auto* obj : m_EnemyPool)
    {
        if (obj->IsActive())
        {
            obj->Draw();
        }
    }
    for (auto* obj : m_BulletPool)
    {
        if (obj->IsActive())
        {
            obj->Draw();
        }
    }

    // Draw All Effects
    for (auto* effect : m_EffectPool)
    {
        if (effect->IsActive())
        {
            effect->Draw();
        }
    }
}

void Billboard_Manager::Create_Enemy(const XMFLOAT3& pos)
{
    for (auto* obj : m_EnemyPool)
    {
        if (!obj->IsActive())
        {
            obj->Reset_State(Billboard_Enemy::Enemy_Normal_ID, pos, 1.0f, 1.0f);
            obj->Activate(pos);
            return;
        }
    }
	Debug::D_Out << "Enemy Pool Full!" << std::endl;
}

void Billboard_Manager::Create_Bullet(const DirectX::XMFLOAT3& pos)
{
	for (auto* obj : m_BulletPool)
	{
		if (!obj->IsActive())
		{
			obj->Reset_State(Billboard_Bullet::Bullet_Normal_ID, pos, 0.5f, 0.5f);
			obj->Activate(pos);
			return;
		}
	}
	Debug::D_Out << "Bullet Pool Full!" << std::endl;
}

void Billboard_Manager::Create_Effect(const XMFLOAT3& pos, float scale, Effect_Type Type)
{
    float finalScale = scale * 0.8f;

    for (auto* effect : m_EffectPool)
    {
        if (!effect->IsActive())
        {
            effect->Reset(Type, pos, finalScale);
            return;
        }
    }
    Debug::D_Out << "Effect Pool Full!" << std::endl;
}

Billboard_Enemy* Billboard_Manager::Check_Enemy_Collision(const AABB& box)
{
    for (auto* obj : m_EnemyPool)
    {
        if (obj->IsActive() && Collision_Is_Hit_AABB(box, obj->GetAABB()).Is_Hit)
            return obj;
    }
    return nullptr;
}

Billboard_Bullet* Billboard_Manager::Check_Bullet_Collision(const AABB& box)
{
	for (auto* obj : m_BulletPool)
	{
		if (obj->IsActive() && Collision_Is_Hit_AABB(box, obj->GetAABB()).Is_Hit)
			return obj;
	}
	return nullptr;
}