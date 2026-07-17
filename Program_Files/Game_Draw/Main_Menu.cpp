/*==============================================================================

	Game Main Menu [Main_Menu.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Project_Header.h"
#include "Main_Menu.h"
#include "Fade.h"
#include "Audio_Manager.h"
#include "Game_Screen_Manager.h"
#include "Shader_Manager.h"
#include "Texture_Manager.h"
#include "KeyLogger.h"
#include "Player_Camera.h"
#include "Controller_Input.h"
#include "Light_Manager.h"

using namespace DirectX;

//----------------UI Texture----------------//
static int Main_BG = -1;
static int Main_Title = -1;
static int UI_Start[2] = {-1, -1}, UI_Set[2] = {-1, -1}, UI_Ranking[2] = { -1, -1 },  UI_Exit[2] = { -1, -1 };

//----------------------POS----------------------/
static float BG_W, BG_H;
static float Title_X, Title_Y, Title_W, Title_H;

static float UI_H;

static float Start_W, Start_X, Start_Y;
static float Set_W, Set_X, Set_Y;
static float Rank_W, Rank_X, Rank_Y;
static float Exit_W, Exit_X, Exit_Y;

//----------------State & Data----------------//
// State Info
Main_Select_Buffer M_Buffer = Main_Select_Buffer::None;

// Sound Scale Info
static float Current_Volume = 0.0f;
static double BGM_Fade_Timer = 0.0;
constexpr double BGM_FADE_TIME = 3.0;
bool Is_BGM_Playing;
bool Is_BGM_Fading_In;

// Mouse Info
Mouse_Info Mouse_Menu;
static bool Is_Mouse_Left_Clicked_Prev = false;

// Fade Info
static bool EXIT_STATE = false;
static constexpr double FADE_OUT_TIME = 2.0;
static double Fade_Out_Timer = 0.0;

// Bool Info
static bool Menu_Selected = false;
static bool Controller_Alert = false;
static bool Wait_For_Release = false;

void Main_Menu_Initialize()
{
	Main_Menu_Texture();
	Mouse_SetVisible(false);

	BG_W = static_cast<float>(Direct3D_GetBackBufferWidth());
	BG_H = static_cast<float>(Direct3D_GetBackBufferHeight());

	Title_H = BG_H * 0.25f;
	Title_W = Texture_Manager::GetInstance()->Get_Proportional_Width(Main_Title, Title_H);
	Title_X = BG_W * 0.1f;
	Title_Y = BG_H * 0.175f;

	UI_H = BG_H * 0.05f;
	float Base_X = BG_W * 0.85f;

	// Start Menu
	Start_W = Texture_Manager::GetInstance()->
		Get_Proportional_Width(UI_Start[static_cast<int>(Menu_State::Wait)], UI_H);
	Start_X = Base_X - (Start_W * 0.5f);
	Start_Y = (BG_H * 0.675f) - (UI_H * 0.5f);

	// Setting Menu
	Set_W = Texture_Manager::GetInstance()->
		Get_Proportional_Width(UI_Set[static_cast<int>(Menu_State::Wait)], UI_H);
	Set_X = Base_X - (Set_W * 0.5f);
	Set_Y = (BG_H * 0.75f) - (UI_H * 0.5f);

	// Ranking Menu
	Rank_W = Texture_Manager::GetInstance()->
		Get_Proportional_Width(UI_Ranking[static_cast<int>(Menu_State::Wait)], UI_H);
	Rank_X = Base_X - (Rank_W * 0.5f);
	Rank_Y = (BG_H * 0.825f) - (UI_H * 0.5f);

	// Exit Menu
	Exit_W = Texture_Manager::GetInstance()->
		Get_Proportional_Width(UI_Exit[static_cast<int>(Menu_State::Wait)], UI_H);
	Exit_X = Base_X - (Exit_W * 0.5f);
	Exit_Y = (BG_H * 0.9f) - (UI_H * 0.5f);

	Mouse_Menu.Size = BG_H * 0.05f;
	Mouse_Menu.Prev_X = BG_W * 0.5f;
	Mouse_Menu.Prev_Y = BG_H * 0.5f;

	Set_Main_Menu_Buffer(Main_Select_Buffer::None);
}

void Main_Menu_Finalize()
{
}

void Main_Menu_Update(float elapsed_time)
{
	Mouse_State State = Mouse_Get_Prev_State(Mouse_Menu);
	bool Mouse_Movement = Is_Mouse_Moved();
	bool Is_Mouse_Click = (State.leftButton);

	if (Mouse_Movement)
	{
		if (Is_Mouse_In_RECT(Mouse_Menu.X, Mouse_Menu.Y, Start_X, Start_Y, Start_W, UI_H))
		{
			if (Get_Main_Menu_Buffer() != Main_Select_Buffer::Start)
			{
				Set_Main_Menu_Buffer(Main_Select_Buffer::Start);
				Audio_Manager::GetInstance()->Play_SFX("Buffer_Move");
			}
		}
		else if (Is_Mouse_In_RECT(Mouse_Menu.X, Mouse_Menu.Y, Set_X, Set_Y, Set_W, UI_H))
		{
			if (Get_Main_Menu_Buffer() != Main_Select_Buffer::Setting)
			{
				Set_Main_Menu_Buffer(Main_Select_Buffer::Setting);
				Audio_Manager::GetInstance()->Play_SFX("Buffer_Move");
			}
		}
		else if (Is_Mouse_In_RECT(Mouse_Menu.X, Mouse_Menu.Y, Rank_X, Rank_Y, Rank_W, UI_H))
		{
			if (Get_Main_Menu_Buffer() != Main_Select_Buffer::Ranking)
			{
				Set_Main_Menu_Buffer(Main_Select_Buffer::Ranking);
				Audio_Manager::GetInstance()->Play_SFX("Buffer_Move");
			}
		}
		else if (Is_Mouse_In_RECT(Mouse_Menu.X, Mouse_Menu.Y, Exit_X, Exit_Y, Exit_W, UI_H))
		{
			if (Get_Main_Menu_Buffer() != Main_Select_Buffer::Exit)
			{
				Set_Main_Menu_Buffer(Main_Select_Buffer::Exit);
				Audio_Manager::GetInstance()->Play_SFX("Buffer_Move");
			}
		}
		else
		{
			Set_Main_Menu_Buffer(Main_Select_Buffer::Wait);
		}
	}

	if (KeyLogger_IsTrigger(KK_W) || KeyLogger_IsTrigger(KK_UP) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_UP))
	{
		Main_Select_Buffer current = Get_Main_Menu_Buffer();
		if (current == Main_Select_Buffer::None || current == Main_Select_Buffer::Wait)
		{
			Set_Main_Menu_Buffer(Main_Select_Buffer::Start);
			Audio_Manager::GetInstance()->Play_SFX("Buffer_Move");
		}
		else if (current == Main_Select_Buffer::Setting)
		{ 
			Set_Main_Menu_Buffer(Main_Select_Buffer::Start);
			Audio_Manager::GetInstance()->Play_SFX("Buffer_Move"); 
		}
		else if (current == Main_Select_Buffer::Ranking)
		{
			Set_Main_Menu_Buffer(Main_Select_Buffer::Setting); 
			Audio_Manager::GetInstance()->Play_SFX("Buffer_Move");
		}
		else if (current == Main_Select_Buffer::Exit)
		{
			Set_Main_Menu_Buffer(Main_Select_Buffer::Ranking);
			Audio_Manager::GetInstance()->Play_SFX("Buffer_Move");
		}
	}
	else if (KeyLogger_IsTrigger(KK_S) || KeyLogger_IsTrigger(KK_DOWN) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_DOWN))
	{
		Main_Select_Buffer current = Get_Main_Menu_Buffer();
		if (current == Main_Select_Buffer::None || current == Main_Select_Buffer::Wait)
		{
			Set_Main_Menu_Buffer(Main_Select_Buffer::Start);
			Audio_Manager::GetInstance()->Play_SFX("Buffer_Move");
		}
		else if (current == Main_Select_Buffer::Start) 
		{
			Set_Main_Menu_Buffer(Main_Select_Buffer::Setting);
			Audio_Manager::GetInstance()->Play_SFX("Buffer_Move");
		}
		else if (current == Main_Select_Buffer::Setting) 
		{
			Set_Main_Menu_Buffer(Main_Select_Buffer::Ranking);
			Audio_Manager::GetInstance()->Play_SFX("Buffer_Move");
		}
		else if (current == Main_Select_Buffer::Ranking) 
		{
			Set_Main_Menu_Buffer(Main_Select_Buffer::Exit); 
			Audio_Manager::GetInstance()->Play_SFX("Buffer_Move"); 
		}
	}

	bool Confirm_Input = (KeyLogger_IsTrigger(KK_ENTER) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_A));

	if (Is_Mouse_Click && !Is_Mouse_Left_Clicked_Prev)
	{
		if (Get_Main_Menu_Buffer() == Main_Select_Buffer::Start ||
			Get_Main_Menu_Buffer() == Main_Select_Buffer::Setting ||
			Get_Main_Menu_Buffer() == Main_Select_Buffer::Ranking ||
			Get_Main_Menu_Buffer() == Main_Select_Buffer::Exit)
		{
			Confirm_Input = true;
		}
	}
	Is_Mouse_Left_Clicked_Prev = State.leftButton;

	if (Confirm_Input)
	{
		if (Get_Main_Menu_Buffer() != Main_Select_Buffer::None && Get_Main_Menu_Buffer() != Main_Select_Buffer::Wait)
		{
			Audio_Manager::GetInstance()->Play_SFX("Buffer_Select");

			switch (Get_Main_Menu_Buffer())
			{
			case Main_Select_Buffer::Start:
				Game_Screen_Manager::GetInstance()->Update_Main_Screen(Main_Screen::SELECT_GAME);
				Game_Screen_Manager::GetInstance()->Update_Game_Select_Screen(Game_Select_Screen::GAME_MENU_SELECT);
				Set_Main_Menu_Buffer(Main_Select_Buffer::None);
				break;

			case Main_Select_Buffer::Setting:
				Game_Screen_Manager::GetInstance()->Update_Main_Screen(Main_Screen::SELECT_SETTINGS);
				Game_Screen_Manager::GetInstance()->Update_Sub_Screen(Sub_Screen::SETTINGS);
				Set_Main_Menu_Buffer(Main_Select_Buffer::None);
				break;

			case Main_Select_Buffer::Ranking:
				// Need Ranking Screen Implementation
				break;

			case Main_Select_Buffer::Exit:
				Game_Screen_Manager::GetInstance()->Update_Main_Screen(Main_Screen::EXIT);
				Set_Main_Menu_Buffer(Main_Select_Buffer::Done);
				Fade_Start(FADE_OUT_TIME, true);
				EXIT_STATE = true;
				Fade_Out_Timer = 0.0;
				break;
			}
		}
	}
}

void Main_Menu_Draw()
{
	Direct3D_SetDepthEnable(false);
	Shader_Manager::GetInstance()->Begin2D();

	Main_Menu_BG_Draw();
	Main_Menu_UI_Draw();

	Mouse_UI_Draw(Get_Main_Menu_Mouse_POS());
}

void Main_Menu_BG_Draw()
{
	Sprite_Draw(Main_BG, A_Zero, A_Zero, BG_W, BG_H);
	Sprite_Draw(Main_Title, Title_X, Title_Y, Title_W, Title_H);
}

void Main_Menu_UI_Draw()
{
	int State_Wait = static_cast<int>(Menu_State::Wait);
	int State_Glow = static_cast<int>(Menu_State::Glow);

	Sprite_Draw((M_Buffer == Main_Select_Buffer::Start) ? UI_Start[State_Glow] : UI_Start[State_Wait],
		Start_X, Start_Y, Start_W, UI_H, A_Zero);
	
	Sprite_Draw((M_Buffer == Main_Select_Buffer::Setting) ? UI_Set[State_Glow] : UI_Set[State_Wait],
		Set_X, Set_Y, Set_W, UI_H, A_Zero);

	Sprite_Draw((M_Buffer == Main_Select_Buffer::Ranking) ? UI_Ranking[State_Glow] : UI_Ranking[State_Wait],
		Rank_X, Rank_Y, Rank_W, UI_H, A_Zero);

	Sprite_Draw((M_Buffer == Main_Select_Buffer::Exit) ? UI_Exit[State_Glow] : UI_Exit[State_Wait],
		Exit_X, Exit_Y, Exit_W, UI_H, A_Zero);
}

Main_Select_Buffer Get_Main_Menu_Buffer()
{
	return M_Buffer;
}

void Set_Main_Menu_Buffer(Main_Select_Buffer Buffer)
{
	M_Buffer = Buffer;
}

bool IF_IS_Game_Done()
{
	return EXIT_STATE;
}

Mouse_Info Get_Main_Menu_Mouse_POS()
{
	return Mouse_Menu;
}

void Main_Menu_Texture()
{
	int State_Wait = static_cast<int>(Menu_State::Wait);
	int State_Glow = static_cast<int>(Menu_State::Glow);

	//---------------Main Menu Texture---------------//
	Main_BG  = Texture_Manager::GetInstance()->GetID("K");
	Main_Title  = Texture_Manager::GetInstance()->GetID("Title");

	UI_Start	[State_Wait]	= Texture_Manager::GetInstance()->GetID("Start_N");
	UI_Set		[State_Wait]	= Texture_Manager::GetInstance()->GetID("Settings_N");
	UI_Ranking	[State_Wait]	= Texture_Manager::GetInstance()->GetID("Ranking_N");
	UI_Exit		[State_Wait]	= Texture_Manager::GetInstance()->GetID("Exit_N");

	UI_Start	[State_Glow]	= Texture_Manager::GetInstance()->GetID("Start_G");
	UI_Set		[State_Glow]	= Texture_Manager::GetInstance()->GetID("Settings_G");
	UI_Ranking	[State_Glow]	= Texture_Manager::GetInstance()->GetID("Ranking_G");
	UI_Exit		[State_Glow]	= Texture_Manager::GetInstance()->GetID("Exit_G");

	if (Main_BG == -1 || Main_Title == -1
		|| UI_Start[State_Wait] == -1 || UI_Set[State_Wait] == -1 || UI_Ranking[State_Wait] == -1 || UI_Exit[State_Wait] == -1
		|| UI_Start[State_Glow] == -1 || UI_Set[State_Glow] == -1 || UI_Ranking[State_Glow] == -1 || UI_Exit[State_Glow] == -1)
	{
		Debug::D_Out << "[Main Menu] Texture Init Error" << std::endl;
		Debug::D_Out << "Main_BG : " << Main_BG 
			<< "Main_Title : " << Main_Title 
			<< "UI_Start : " << UI_Start 
			<< "UI_Set : " << UI_Set 
			<< "UI_Exit : " << UI_Exit << std::endl;
	}
}
