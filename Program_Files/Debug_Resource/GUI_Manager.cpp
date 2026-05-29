/*==============================================================================

	Manage GUI Resource [GUI_Manager.h]

	Author : Choi HyungJoon

==============================================================================*/
#include "Project_Header.h"
#include "GUI_Manager.h"
#include "Light_Manager.h"
#include "Player.h"
#include "Debug_Camera.h"
#include "Player_Camera.h"
#include "Debug_Collision.h"
#include "Shader_Manager.h"
#include "Game_Screen_Manager.h"
#include "Main_Menu.h"

using namespace DirectX;

void GUI_Init(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // keyboard control
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // docking..?

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(device, context);
}

void GUI_Final()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void GUI_Screen_Scene_Editor(double FPS)
{
    ImGui::Begin("Screen Debug Menu");
    ImGui::Text("FPS: %.1f", FPS);

    // ==========================================
    //           [Screen & State Debug]
    // ==========================================
    if (ImGui::CollapsingHeader("State & Screen Debug", ImGuiTreeNodeFlags_DefaultOpen))
    {
        const char* main_screen_names[] = { "M_WAIT", "MAIN", "MENU_SELECT", "SELECT_GAME", "SELECT_SETTINGS", "EXIT", "M_DONE" };
        const char* sub_screen_names[] = { "S_WAIT", "SETTINGS", "S_DONE" };
        const char* game_select_names[] = { "G_WAIT", "GAME_MENU_SELECT", "GAME_PLAYING", "GAME_IN_GAME_MENU", "GAME_SETTING", "G_DONE" };
        const char* main_buffer_names[] = { "None", "Wait", "Start", "Setting", "Exit", "Done" };

        int current_main = static_cast<int>(Game_Manager::GetInstance()->Get_Current_Main_Screen());
        if (ImGui::Combo("Main Screen", &current_main, main_screen_names, IM_ARRAYSIZE(main_screen_names)))
        {
            Game_Manager::GetInstance()->Update_Main_Screen(static_cast<Main_Screen>(current_main));
        }

        int current_sub = static_cast<int>(Game_Manager::GetInstance()->Get_Current_Sub_Screen());
        if (ImGui::Combo("Sub Screen", &current_sub, sub_screen_names, IM_ARRAYSIZE(sub_screen_names)))
        {
            Game_Manager::GetInstance()->Update_Sub_Screen(static_cast<Sub_Screen>(current_sub));
        }

        int current_game = static_cast<int>(Game_Manager::GetInstance()->Get_Current_Game_Select_Screen());
        if (ImGui::Combo("Game Screen", &current_game, game_select_names, IM_ARRAYSIZE(game_select_names)))
        {
            Game_Manager::GetInstance()->Update_Game_Select_Screen(static_cast<Game_Select_Screen>(current_game));
        }

        ImGui::Separator();

        int current_buffer = static_cast<int>(Get_Main_Menu_Buffer());
        if (ImGui::Combo("Main Buffer", &current_buffer, main_buffer_names, IM_ARRAYSIZE(main_buffer_names)))
        {
            Set_Main_Menu_Buffer(static_cast<Main_Select_Buffer>(current_buffer));
        }
    }
    ImGui::End();
}

void GUI_Player_Editor()
{
    ImGui::Begin("Player Debug Menu");

    // ==========================================
    //           [Player Camera Debug]
    // ==========================================
    if (ImGui::CollapsingHeader("Player Camera Debug", ImGuiTreeNodeFlags_DefaultOpen))
    {
        static float camOffsetX = 0.0f, camOffsetY = 5.0f, camOffsetZ = -15.0f;
        ImGui::SliderFloat("Camera Offset X", &camOffsetX, -10.0f, 10.0f);
        ImGui::SliderFloat("Camera Offset Y", &camOffsetY, -10.0f, 10.0f);
        ImGui::SliderFloat("Camera Offset Z", &camOffsetZ, -50.0f, 0.0f);
        GUI_Set_Camera_Offset(camOffsetX, camOffsetY, camOffsetZ);

        static float camPitch = 0.0f;
        ImGui::SliderFloat("Camera Pitch", &camPitch, -89.0f, 89.0f);
        GUI_Set_Camera_Pitch(XMConvertToRadians(camPitch));

        static float camLerpSpeed = 8.0f;
        ImGui::SliderFloat("Camera Lerp Speed", &camLerpSpeed, 1.0f, 30.0f);
        GUI_Set_Camera_Lerp_Speed(camLerpSpeed);

        static float camSizeBase = 3.0f;
        ImGui::SliderFloat("Camera Size Base", &camSizeBase, 1.0f, 10.0f);
        GUI_Set_Camera_Size_Base(camSizeBase);
    }
    ImGui::End();
}