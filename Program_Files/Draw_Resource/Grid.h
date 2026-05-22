/*==============================================================================

	Make Grid [Grid.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef GRID_H
#define GRID_H

void Grid_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Grid_Finalize(void);
void Grid_Draw(void);

#endif // GRID_H

