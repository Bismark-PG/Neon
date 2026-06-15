/*==============================================================================

	Managed Billboard System [Billboard_Manager.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef BILLBOARD_MANAGER_H
#define BILLBOARD_MANAGER_H
#include "Project_Header.h"
#include "Collision.h"
#include "Billboard_Effect.h"
#include <vector>

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

    void Create_Effect(const DirectX::XMFLOAT3& pos, float scale, Effect_Type Type);

private:
    Billboard_Manager() = default;
    ~Billboard_Manager() = default;

	// Pool for Effects (e.g., explosion, smoke)
    static constexpr int MAX_EFFECT_POOL = 200;
    std::vector<class Billboard_Effect*> m_EffectPool;
};

#endif // BILLBOARD_MANAGER_H