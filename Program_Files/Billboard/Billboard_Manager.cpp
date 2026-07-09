/*==============================================================================

	Managed Billboard System [Billboard_Manager.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Project_Header.h"
#include "Billboard_Manager.h"
#include "Texture_Manager.h"
#include "debug_ostream.h"
using namespace DirectX;

int Billboard_Effect::Explosion_Pattern_ID = -1;
int Billboard_Effect::Smoke_Pattern_ID = -1;

void Billboard_Manager::Init()
{
    Billboard_Initialize();

    m_EffectPool.clear();

    Billboard_Effect::Initialize_Resource();

	// Billboard_Effect Pool
    for (int i = 0; i < MAX_EFFECT_POOL; ++i)
    {
        m_EffectPool.push_back(new Billboard_Effect());
    }
}

void Billboard_Manager::Final()
{
    for (auto* effect : m_EffectPool) delete effect;

    m_EffectPool.clear();

    Billboard_Finalize();
}

void Billboard_Manager::Reset()
{
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
    // Draw All Effects
    for (auto* effect : m_EffectPool)
    {
        if (effect->IsActive())
        {
            effect->Draw();
        }
    }
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