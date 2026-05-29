/*==============================================================================

	Manage System Logic [System_Logic_Manager.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Project_Header.h"
#include "System_Logic_Manager.h"
#include "Main_Header.h"
#include "Debug_Collision.h"

void System_Manager::Initialize(HWND hWnd, ID3D11Device* Device, ID3D11DeviceContext* Context)
{
	// Initialize System Tools
	SystemTimer_Initialize();
	KeyLogger_Initialize(hWnd);

	// Initialize Audio Tools
	Audio_Manager::GetInstance()->Init();
	Game_Audio_Initialize();

	// Initialize Shader Tools
	Shader_Manager::GetInstance()->Init(Device, Context);
	Light_Manager::GetInstance().Init();

	// Initialize Texture Tools
	Grid_Initialize(Device, Context);
	Cube_Initialize(Device, Context);
	Texture_Manager::GetInstance()->Init(Device, Context);
	Game_Texture_Initialize();
	Sprite_Initialize(Device, Context);
	SpriteAni_Initialize();
	Billboard_Initialize();
	Billboard_Manager::Instance().Init();

	// Initialize Game Setting Tools
	Fade_Initialize();
	Mouse_UI_set();

	// Initialize Debug Tools
	GUI_Init(hWnd, Device, Context);
	Debug_Camera_Initialize();
	Debug_Collision_Initialize(Device);

	// Initialize Main Logic
	Game_Logic_Initialize();
}

void System_Manager::Update(double elapsed_time, bool IS_Controller_Set)
{
	Fade_Update(elapsed_time);

	// Set Key Logger With FPS
	KeyLogger_Update();
	Controller_Set_Update();
	IS_Controller_Set = Controller_Set_UP();

	// Update Game Texture
	if (IS_Controller_Set)
	{
		SpriteAni_Update(elapsed_time);
	}
	else if (!IS_Controller_Set)
	{
		SpriteAni_Update(elapsed_time);
	}

	// Update Main Logic
	Game_Logic_Update(elapsed_time);
}

void System_Manager::Draw(double FPS)
{
	// Draw Texture
	Direct3D_Clear();
	Sprite_Begin();

#if defined(DEBUG) || defined(_DEBUG)
	// Draw GUI
		// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::GetIO().MouseDrawCursor = true;

	// Game Scene Editor
	GUI_Screen_Scene_Editor(FPS);

	// Player, Camera Editor
	GUI_Player_Editor();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif	

	// Real Draw Start
	Main_Game_Screen_Update();

	// Controller Input Alert
	Controller_Set_Draw();

	// Fade Draw
	Fade_Draw();

	Direct3D_Present();
}

void System_Manager::Finalize()
{
	// Finalize Main Logic
	Game_Logic_Finalize();

	// Finalize Debug Tools
	Debug_Collision_Finalize();
	Debug_Camera_Finalize();
	GUI_Final();

	// Finalize Game Setting Tools
	Fade_Finalize();

	// Finalize Texture Tools
	Billboard_Finalize();
	Billboard_Manager::Instance().Final();
	SpriteAni_Finalize();
	Sprite_Finalize();
	Game_Texture_Finalize();
	Texture_Manager::GetInstance()->Final();
	Cube_Finalize();
	Grid_Finalize();

	// Finalize Shader Tools
	Shader_Manager::GetInstance()->Final();

	// Finalize Audio Tools
	Game_Audio_Finalize();
	Audio_Manager::GetInstance()->Final();

	// Finalize System Tools
	Direct3D_Finalize();
	Mouse_Finalize();
}