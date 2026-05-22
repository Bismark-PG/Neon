/*==============================================================================

	Manage Game Audio Resource [Game_Audio.h]

	Author : Choi HyungJoon

--------------------------------------------------------------------------------
	- Reference

		¡ BGM ¡

		Ÿ DOVA-SYNDROME
		> https://dova-s.jp/

		¡ SFX ¡

		Ÿ  Sound Effect Lab (Œø‰Ê‰¹ƒ‰ƒ{)
		> https://soundeffect-lab.info/

==============================================================================*/
#include "Project_Header.h"
#include "Game_Audio.h"
#include "Audio_Manager.h"

Audio_Manager* Sound_M;

void Game_Audio_Initialize()
{
	Sound_M = Audio_Manager::GetInstance();

	Game_Audio_BGM();
	Game_Audio_SFX();
}

void Game_Audio_Finalize()
{
}

void Game_Audio_BGM()
{
	//------------------------------Main BGM------------------------------//
}

void Game_Audio_SFX()
{
	//--------------------------------Logo SFX--------------------------------//
	Sound_M->Load_SFX("Logo_Draw", "Resource/SFX/Logo_Draw_Fixed.wav");

	//-------------------------------Buffer SFX-------------------------------//

	//--------------------------------Enemy SFX--------------------------------//

	//-------------------------------Player SFX-------------------------------//

	//-------------------------------Weopon SFX-------------------------------//
	
	//---------------------------Upgrade System SFX---------------------------//
}