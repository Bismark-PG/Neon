/*==============================================================================

    Main Game Screen [Main_Game.cpp]

    Author : Choi HyungJoon

==============================================================================*/
#include "Project_Header.h"
#include "Main_Game.h"
#include "Grid.h"
#include "Cube.h"
#include "Shader_Manager.h" 
#include "Player.h"
#include "Player_Camera.h"
#include "Light_Manager.h"
#include "direct3d.h"

using namespace DirectX;

void Main_Game_Initialize()
{
    Player_Initialize({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
    Player_Camera_Initialize();
}

void Main_Game_Finalize()
{
    Player_Camera_Finalize();
    Player_Finalize();
}

void Main_Game_Update(float elapsed_time)
{
    Player_Camera_Update(elapsed_time);
    Player_Update(elapsed_time);
    Light_Manager::GetInstance().Global_Light_Update(elapsed_time);
}

void Main_Game_Draw()
{
    Direct3D_SetDepthEnable(true);
	Light_Manager::GetInstance().Global_Light_Set_Up();
    Shader_Manager::GetInstance()->Begin3D();

    Grid_Draw();
    Player_Draw();
}