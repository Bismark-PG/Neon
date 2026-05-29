/*==============================================================================

    Managed Billboard Target Type [Billboard_Target.h]

    Author : Choi HyungJoon

==============================================================================*/
#ifndef BILLBOARD_EFFECT_H
#define BILLBOARD_EFFECT_H
#include "Billboard_Object.h"
#include "Sprite_Animation.h"
#include "Texture_Manager.h"

enum class Effect_Type
{
    SMOKE,
    EXPLOSION
};

class Billboard_Effect : public Billboard_Object
{
public:

    static int Explosion_Pattern_ID;
    static int Smoke_Pattern_ID;

    static void Initialize_Resource()
    {
        int Explosion = Texture_Manager::GetInstance()->GetID("Effect_Explosion");
        if (Explosion != -1)
        {
            DirectX::XMUINT2 patternSize = { 64, 64 };
            Explosion_Pattern_ID = SpriteAni_Get_Pattern_Info(Explosion, 16, 4, 0.05, patternSize, { 0, 0 }, false);
        }

        int Smoke = Texture_Manager::GetInstance()->GetID("Effect_Smoke");
        if (Smoke != -1)
        {
            DirectX::XMUINT2 patternSize = { 64, 64 };
            Smoke_Pattern_ID = SpriteAni_Get_Pattern_Info(Smoke, 16, 4, 0.05, patternSize, { 0, 0 }, false);
        }
    }

    Billboard_Effect()
        : Billboard_Object(-1, { 0,0,0 }, 1.0f, 1.0f)
    {
        m_IsActive = false;
        m_PlayID = -1;
    }

    virtual ~Billboard_Effect()
    {
        if (m_PlayID != -1)
        {
            SpriteAni_DestroyPlayer(m_PlayID);
        }
    }

    void Reset(Effect_Type type, const DirectX::XMFLOAT3& pos, float scale)
    {
        int patternID = -1;
        switch (type)
        {
        case Effect_Type::SMOKE:
            patternID = Explosion_Pattern_ID;
            break;
        case Effect_Type::EXPLOSION:
            patternID = Smoke_Pattern_ID;
            break;
        }

        if (patternID == -1) return;

        m_Position = pos;
        m_ScaleX = scale;
        m_ScaleY = scale;

        if (m_PlayID != -1) SpriteAni_DestroyPlayer(m_PlayID);
        m_PlayID = SpriteAni_CreatePlayer(patternID);
        Activate(pos);
    }

    virtual void Update(double elapsed_time) override
    {
        // float dt = static_cast<float>(elapsed_time);

        if (!m_IsActive) return;

        if (m_PlayID != -1)
        {
            if (SpriteAni_IsStopped(m_PlayID))
            {
                SpriteAni_DestroyPlayer(m_PlayID);
                m_PlayID = -1;
                Deactivate();
            }
        }
    }

    virtual void Draw() override
    {
        if (!m_IsActive || m_PlayID == -1)
        {
            return;
        }

        if (SpriteAni_IsStopped(m_PlayID))
        {
            return;
        }

        Billboard_Draw_Animation(m_PlayID, m_Position, m_ScaleX, m_ScaleY);
    }

private:
    int m_PlayID = -1;
};

#endif // BILLBOARD_EFFECT_H