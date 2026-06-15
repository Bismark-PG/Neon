/*==============================================================================

    Managed Billboard Target Type [Billboard_Effect.h]

    Author : Choi HyungJoon

==============================================================================*/
#ifndef BILLBOARD_EFFECT_H
#define BILLBOARD_EFFECT_H
#include "Billboard.h"
#include "Sprite_Animation.h"
#include "Texture_Manager.h"

enum class Effect_Type
{
    SMOKE,
    EXPLOSION
};

class Billboard_Effect
{
public:
    static int Explosion_Pattern_ID;
    static int Smoke_Pattern_ID;

    static void Initialize_Resource()
    {
        int exID = Texture_Manager::GetInstance()->GetID("Effect_Explosion");
        if (exID != -1) 
            Explosion_Pattern_ID = SpriteAni_Get_Pattern_Info(exID, 16, 4, 0.05, { 64, 64 }, { 0, 0 }, false);

        //int smID = Texture_Manager::GetInstance()->GetID("Effect_Smoke");
        //if (smID != -1) 
        //    Smoke_Pattern_ID = SpriteAni_Get_Pattern_Info(smID, 16, 4, 0.05, { 64, 64 }, { 0, 0 }, false);
    }

    Billboard_Effect() : m_IsActive(false), m_PlayID(-1), m_Scale(1.0f) {}
    ~Billboard_Effect() { if (m_PlayID != -1) SpriteAni_DestroyPlayer(m_PlayID); }

    bool IsActive() const { return m_IsActive; }
    void Deactivate() { m_IsActive = false; }

    void Reset(Effect_Type type, const DirectX::XMFLOAT3& pos, float scale)
    {
        int patternID = (type == Effect_Type::EXPLOSION) ? Explosion_Pattern_ID : Smoke_Pattern_ID;
        if (patternID == -1) return;

        m_Position = pos;
        m_Scale = scale;

        if (m_PlayID != -1) SpriteAni_DestroyPlayer(m_PlayID);

        m_PlayID = SpriteAni_CreatePlayer(patternID);
        m_IsActive = true;
    }

    void Update(double elapsed_time)
    {
        if (!m_IsActive) return;

        if (m_PlayID != -1 && SpriteAni_IsStopped(m_PlayID))
        {
            SpriteAni_DestroyPlayer(m_PlayID);
            m_PlayID = -1;
            Deactivate();
        }
    }

    void Draw()
    {
        if (!m_IsActive || m_PlayID == -1 || SpriteAni_IsStopped(m_PlayID)) return;
        Billboard_Draw_Animation(m_PlayID, m_Position, m_Scale, m_Scale);
    }

private:
    bool m_IsActive;
    int m_PlayID;
    DirectX::XMFLOAT3 m_Position = { 0, 0, 0 };
    float m_Scale;
};

#endif // BILLBOARD_EFFECT_H