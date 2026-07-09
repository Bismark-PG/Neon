/*==============================================================================

    Manage Bullet Spawn System [Bullet_Manager.cpp]

    Author : Choi HyungJoon

==============================================================================*/
#include "Project_Header.h"
#include "Bullet_Manager.h"
#include "Bullet_Ray.h"
#include "Bullet_Missile.h"

void Bullet_Manager::Init()
{
    m_Ray_Pool.clear();
    m_Missile_Pool.clear();

    m_Active_List.clear();

    for (int i = 0; i < MAX_RAY_POOL; ++i)
    {
        m_Ray_Pool.push_back(new Bullet_Ray());
    }
    for (int i = 0; i < MAX_MISSILE_POOL; ++i)
    {
        m_Missile_Pool.push_back(new Bullet_Missile());
    }
}

void Bullet_Manager::Final()
{
    for (Bullet* B : m_Ray_Pool) delete B;
    for (Bullet* B : m_Missile_Pool) delete B;

    m_Ray_Pool.clear();
    m_Missile_Pool.clear();
    m_Active_List.clear();
}

void Bullet_Manager::Reset()
{
    for (Bullet* B : m_Ray_Pool)
    {
        if (B->IsActive()) B->Deactivate();
    }

    for (Bullet* B : m_Missile_Pool)
    {
        if (B->IsActive()) B->Deactivate();
    }

    m_Active_List.clear();
}

void Bullet_Manager::Update(float Elapsed_Time)
{
    m_Active_List.clear();

    for (Bullet* B : m_Ray_Pool)
    {
        if (B->IsActive())
        {
            B->Update(Elapsed_Time);
            m_Active_List.push_back(B);
        }
    }

    for (Bullet* B : m_Missile_Pool)
    {
        if (B->IsActive())
        {
            B->Update(Elapsed_Time);
            m_Active_List.push_back(B);
        }
    }
}

void Bullet_Manager::Draw()
{
    for (Bullet* B : m_Active_List)
    {
        B->Draw();
    }
}

void Bullet_Manager::Fire_Ray(const XMFLOAT3& Start_Pos, const XMFLOAT3& Dir, int Damage)
{
    for (Bullet* B : m_Ray_Pool)
    {
        if (!B->IsActive())
        {
            B->Activate(Start_Pos, Dir, BulletOwner::PLAYER, Damage);
            return;
        }
    }
}

void Bullet_Manager::Fire_Missile(const XMFLOAT3& Start_Pos, const XMFLOAT3& Dir, int Damage, Enemy* Target)
{
    for (Bullet* B : m_Missile_Pool)
    {
        if (!B->IsActive())
        {
            static_cast<Bullet_Missile*>(B)->Activate_Missile(Start_Pos, Dir, Damage, Target);
            return;
        }
    }
}