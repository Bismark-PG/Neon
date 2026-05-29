/*==============================================================================

	Managed Billboard Target Type [Billboard_Player.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef BILLBOARD_PLAYER_H
#define BILLBOARD_PLAYER_H
#include "Billboard_Object.h"
#include "Texture_Manager.h"

class Billboard_Player : public Billboard_Object
{
public:
    static int Player_ID;

    static void Initialize_Resource()
    {
        Player_ID = Texture_Manager::GetInstance()->GetID("Player");
    }

    Billboard_Player(int texID, const DirectX::XMFLOAT3& pos, float scaleX, float scaleY)
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
    
	void Draw() override
	{
		if (!m_IsActive) return;

		Billboard_Draw(Player_ID, m_Position, m_ScaleX, m_ScaleY, m_Pivot);
	}
};

#endif // BILLBOARD_PLAYER_H