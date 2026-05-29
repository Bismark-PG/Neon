/*==============================================================================

	Managed Billboard Enemy Type [Billboard_Enemy.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef BILLBOARD_ENEMY_H
#define BILLBOARD_ENEMY_H
#include "Billboard_Object.h"
#include "Texture_Manager.h"

class Billboard_Enemy : public Billboard_Object
{
public:
    static int Enemy_Normal_ID;

    static void Initialize_Resource()
    {
        // Enemy_Normal_ID = Texture_Manager::GetInstance()->GetID("Enemy_Basic");
    }

    Billboard_Enemy(int texID, const DirectX::XMFLOAT3& pos, float scaleX, float scaleY)
        : Billboard_Object(texID, pos, scaleX, scaleY)
    {
    }

    virtual void Update(double elapsed_time) override
    {
        // float dt = static_cast<float>(elapsed_time);
    }

    void OnHit()
    {
        Deactivate();
    }
};

#endif // BILLBOARD_ENEMY_H