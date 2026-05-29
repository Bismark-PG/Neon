/*==============================================================================

	Effect Draw [Effect.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef EFFECT_H
#define EFFECT_H

void Effect_Initialize();
void Effect_Finalize();

void Effect_Update(float elapsed_time);
void Effect_Draw();

void Effect_Create(const DirectX::XMFLOAT2& position);

#endif // EFFECT_H