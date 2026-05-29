/*==============================================================================

	Managed Billboard Bullet Type [Billboard_Bullet.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef BILLBOARD_BULLET_H
#define BILLBOARD_BULLET_H
#include "Billboard_Object.h"
#include "Texture_Manager.h"

class Billboard_Bullet : public Billboard_Object
{
public:
    static int Bullet_Normal_ID;

    static void Initialize_Resource()
    {
        // Bullet_Normal_ID = Texture_Manager::GetInstance()->GetID("Enemy_Basic");
    }

    Billboard_Bullet(int texID, const DirectX::XMFLOAT3& pos, float scaleX, float scaleY)
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

#endif // BILLBOARD_BULLET_H