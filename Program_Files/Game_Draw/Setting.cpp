/*==============================================================================

	Game Setting [Setting.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Project_Header.h"
#include "Setting.h"
#include "direct3d.h"
#include "Texture_Manager.h"
#include "Shader_Manager.h"
#include "KeyLogger.h"
#include "Game_Screen_Manager.h"
#include "Audio_Manager.h"
#include "Main_Menu.h"
#include "Palette.h"
#include "Sprite_Animation.h"
#include "Fade.h"

using namespace DirectX;
using namespace PALETTE;

//----------------UI Texture----------------//
static int Setting_BG = -1;

//----------------------POS----------------------//
static float ScreenW = 0.0f;
static float ScreenH = 0.0f;

static float Panel_X = 0.0f;
static float Panel_Y = 0.0f;
static float Panel_W = 0.0f;
static float Panel_H = 0.0f;

//---------------Private Logic---------------//
void Setting_BG_Draw();

void Setting_Texture();

//-----------------Main Logic-----------------//
void Setting_Initialize()
{
	Setting_Texture();

	ScreenW = static_cast<float>(Direct3D_GetBackBufferWidth());
	ScreenH = static_cast<float>(Direct3D_GetBackBufferHeight());

	// 1. Panel
	Panel_W = ScreenW * 0.5f;  
	Panel_H = ScreenH * 0.5f;  
	Panel_X = ScreenW * 0.25f; 
	Panel_Y = ScreenH * 0.3f;  
}

void Setting_Finalize()
{

}

void Setting_Update(float elapsed_time)
{
	if (KeyLogger_IsTrigger(KK_X) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_B))
	{
		Audio_Manager::GetInstance()->Play_SFX("Buffer_Back");

		Game_Screen_Manager::GetInstance()->Update_Main_Screen(Main_Screen::MENU_SELECT);
		Game_Screen_Manager::GetInstance()->Update_Sub_Screen(Sub_Screen::S_WAIT);

		Set_Main_Menu_Buffer(Main_Select_Buffer::Setting);
	}
}

void Setting_Draw()
{
	Direct3D_SetDepthEnable(false);
	Shader_Manager::GetInstance()->Begin2D();

	if (Setting_BG == -1) return;

	Sprite_Draw(Setting_BG, Panel_X, Panel_Y, Panel_W, Panel_H);
}

//---------------Private Logic---------------//
void Setting_BG_Draw()
{

}

void Setting_Texture()
{
	//------------------UI Texture------------------//
	Setting_BG = Texture_Manager::GetInstance()->GetID("Setting_BG");

	if (Setting_BG == -1)
	{
		Debug::D_Out << "[Setting] Texture Init Error" << std::endl;
		Debug::D_Out << "Setting_BG : " << Setting_BG  << std::endl;
	}
}