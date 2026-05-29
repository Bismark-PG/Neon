/*==============================================================================

	Update Game Screen [Update_Game.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Project_Header.h"
#include "Update_Game.h"
#include "debug_ostream.h"
#include "System_Timer.h"
#include "mouse.h"
#include "Main_Game.h"
#include "Main_Menu.h"
#include "KeyLogger.h"

void Game_Logic_Initialize()
{
	Main_Game_Initialize();
	Main_Menu_Initialize();
}

void Game_Logic_Finalize()
{
	Main_Menu_Finalize();
	Main_Game_Finalize();
}
void Game_Logic_Update(double elapsed_time)
{
	float dt = static_cast<float>(elapsed_time);

	if (KeyLogger_IsTrigger(KK_ESCAPE))
	{
		Game_Manager::GetInstance()->Update_Main_Screen(Main_Screen::MAIN);
		Game_Manager::GetInstance()->Update_Game_Select_Screen(Game_Select_Screen::G_WAIT);
		Set_Main_Menu_Buffer(Main_Select_Buffer::None);
		Mouse_SetMode(MOUSE_POSITION_MODE_ABSOLUTE);  
	}

	Game_Manager::GetInstance()->Apply_Screen_Changes();

	Main_Screen M_State = Game_Manager::GetInstance()->Get_Current_Main_Screen();
	Sub_Screen S_State = Game_Manager::GetInstance()->Get_Current_Sub_Screen();
	Game_Select_Screen G_State = Game_Manager::GetInstance()->Get_Current_Game_Select_Screen();

	Game_Logic_Menu(M_State, dt);
	Game_Logic_Sub(S_State, dt);
	Game_Logic_InGame(G_State, dt);
}

void Game_Logic_Menu(Main_Screen State, float elapsed_time)
{
	switch (State)
	{
	case Main_Screen::M_WAIT:
	case Main_Screen::MAIN:
		Main_Menu_Update(elapsed_time);
		break;

	case Main_Screen::MENU_SELECT:

		break;

	case Main_Screen::SELECT_SETTINGS:
	case Main_Screen::SELECT_GAME:
	case Main_Screen::EXIT:
		break;
	}
}

void Game_Logic_Sub(Sub_Screen State, float elapsed_time)
{
	switch (State)
	{
	case Sub_Screen::S_WAIT:
	case Sub_Screen::S_DONE:
		break;

	case Sub_Screen::SETTINGS:
		break;
	}
}

void Game_Logic_InGame(Game_Select_Screen State, float elapsed_time)
{
	switch (State)
	{
	case Game_Select_Screen::G_WAIT:
	case Game_Select_Screen::G_DONE:
		break;

	case Game_Select_Screen::GAME_MENU_SELECT:
		Main_Game_Update(elapsed_time);
		break;

	case Game_Select_Screen::GAME_PLAYING:
		break;

	case Game_Select_Screen::GAME_IN_GAME_MENU:
		break;

	case Game_Select_Screen::GAME_SETTING:
		break;
	}
}

void Main_Game_Screen_Update()
{
	Main_Screen current_screen = Game_Manager::GetInstance()->Get_Current_Main_Screen();

	switch (current_screen)
	{
	case Main_Screen::M_WAIT:
		Mouse_SetMode(MOUSE_POSITION_MODE_ABSOLUTE);
		Game_Manager::GetInstance()->Update_Main_Screen(Main_Screen::MAIN);
		break;

	case Main_Screen::MAIN:
		Main_Menu_Draw();
		break;

	case Main_Screen::MENU_SELECT:
		break;

	case Main_Screen::SELECT_GAME:
		Game_Select_Screen_Update();
		break;

	case Main_Screen::SELECT_SETTINGS:
		Sub_Game_Screen_Update();
		break;

	case Main_Screen::EXIT:
		Game_Manager::GetInstance()->Update_Main_Screen(Main_Screen::M_DONE);
		Main_Menu_Draw();
		Debug::D_Out << "Exiting Game..." << std::endl;
		break;

	case Main_Screen::M_DONE:
		Main_Menu_Draw();
		Debug::D_Out << "Programme Done" << std::endl;
		break;
	}
}

void Sub_Game_Screen_Update()
{
	Sub_Screen current_screen = Game_Manager::GetInstance()->Get_Current_Sub_Screen();

	switch (current_screen)
	{
	case Sub_Screen::S_WAIT:
		break;

	case Sub_Screen::SETTINGS:
		break;

	case Sub_Screen::S_DONE:
		Game_Manager::GetInstance()->Update_Main_Screen(Main_Screen::MENU_SELECT);
		Game_Manager::GetInstance()->Update_Sub_Screen(Sub_Screen::S_WAIT);
		break;
	}
}

void Game_Select_Screen_Update()
{
	Game_Select_Screen current_screen = Game_Manager::GetInstance()->Get_Current_Game_Select_Screen();

	switch (current_screen)
	{
	case Game_Select_Screen::G_WAIT:
		break;

	case Game_Select_Screen::GAME_MENU_SELECT:
		Main_Game_Draw();
		break;

	case Game_Select_Screen::GAME_PLAYING:
		break;

	case Game_Select_Screen::GAME_IN_GAME_MENU:
		break;

	case Game_Select_Screen::GAME_SETTING:
		break;

	case Game_Select_Screen::G_DONE:
		Game_Manager::GetInstance()->Update_Main_Screen(Main_Screen::MENU_SELECT);
		Game_Manager::GetInstance()->Update_Game_Select_Screen(Game_Select_Screen::G_WAIT);
		break;
	}
}