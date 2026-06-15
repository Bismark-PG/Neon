/*==============================================================================

	Enemy Normal Type Class [Enemy_Normal.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef ENEMY_NORMAL_H
#define ENEMY_NORMAL_H
#include "Enemy.h"
#include "Texture_Manager.h"
#include "Billboard_Manager.h"
#include "Palette.h"

class Enemy_Normal : public Enemy
{
public:
    static int Normal_TexID;

    static void Initialize_Resource()
    {
        Normal_TexID = Texture_Manager::GetInstance()->GetID("Enemy");
    }

	virtual void Update_Logic(float dt) override
    {
        Position.z -= m_Info.Speed * dt;
    }

    virtual void Draw() override
    {
        if (!m_IsActive) return;
        Billboard_Draw(Normal_TexID, Position, m_Info.Scale, m_Info.Scale,
            { 0.5f, 0.5f }, PALETTE::White ,Billboard_Facing::YAW_ROLL_ONLY);
    }
};

#endif // ENEMY_NORMAL_H