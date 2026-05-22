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

void Main_Game_Update(double elapsed_time)
{
    Player_Camera_Update(elapsed_time);
    Player_Update(elapsed_time);
}

void Main_Game_Draw()
{
    Shader_Manager::GetInstance()->Begin3D();

    Grid_Draw();
    Player_Draw();
}