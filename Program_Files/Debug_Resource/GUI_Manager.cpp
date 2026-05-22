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

void GUI_Model_Editor(double FPS)
{
	ImGui::Begin("Debug Menu");

	ImGui::Text("FPS: %.1f", FPS);
	// ==========================================
	//			   [Player Control]
	// ==========================================
	if (ImGui::CollapsingHeader("Player Control", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// 1. Position Control
		XMFLOAT3 currentPos = Player_Get_POS();
		float posArr[3] = { currentPos.x, currentPos.y, currentPos.z };

		if (ImGui::DragFloat3("Position", posArr, 0.1f, -250.0f, 250.0f))
		{
			currentPos = { posArr[0], posArr[1], posArr[2] };
			Player_Set_POS(currentPos);
		}

		if (ImGui::Button("Reset Position"))
		{
			XMFLOAT3 resetPos = { 0.0f, 5.0f, 0.0f };
			Player_Set_POS(resetPos);
		}

		ImGui::Separator();
	}
	// ==========================================
	//		    [Debug Render Settings]
	// ==========================================
	if (ImGui::CollapsingHeader("Debug Render Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool Is_Debug_Draw = Debug_Draw_Get_State();

		if (ImGui::Checkbox("Show Collision Boxes", &Is_Debug_Draw))
		{
			Debug_Draw_Get_State(Is_Debug_Draw);
		}

		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "[Hitbox Visible]");
	}
	// ==========================================
	//			  [Debug Camera Info]
	// ==========================================
	if (ImGui::CollapsingHeader("Debug Camera Info", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// (1) Debug Camera Position
		XMFLOAT3 dcPos = Debug_Camera_Get_POS();
		float dcPosArr[3] = { dcPos.x, dcPos.y, dcPos.z };

		ImGui::TextColored(ImVec4(0.5f, 0.8f, 1.0f, 1.0f), "[ Debug Camera Transform ]");

		if (ImGui::DragFloat3("Cam Position", dcPosArr, 0.1f))
		{
			Debug_Camera_Set_Position({ dcPosArr[0], dcPosArr[1], dcPosArr[2] });
		}

		// (2) FOV Control
		float fovRad = Debug_Camera_Get_FOV();
		float fovDeg = XMConvertToDegrees(fovRad);

		// 1 ~ 179 Degree
		if (ImGui::DragFloat("FOV (Degree)", &fovDeg, 1.0f, 1.0f, 179.0f))
		{
			// Degree >> Radian 
			Debug_Camera_Set_FOV(XMConvertToRadians(fovDeg));
		}

		// (3) Vectors
		if (ImGui::TreeNode("Direction Vectors (Edit)"))
		{
			// Front
			XMFLOAT3 F = Debug_Camera_Get_Front();
			float FrontArr[3] = { F.x, F.y, F.z };
			if (ImGui::DragFloat3("Front", FrontArr, 0.01f, -1.0f, 1.0f))
			{
				Debug_Camera_Set_Front({ FrontArr[0], FrontArr[1], FrontArr[2] });
			}

			// Vertical
			XMFLOAT3 V = Debug_Camera_Get_Vertical();
			float VerticalArr[3] = { V.x, V.y, V.z };
			if (ImGui::DragFloat3("Vertical", VerticalArr, 0.01f, -1.0f, 1.0f))
			{
				Debug_Camera_Set_Vertical({ VerticalArr[0], VerticalArr[1], VerticalArr[2] });
			}

			// Horizon
			XMFLOAT3 H = Debug_Camera_Get_Horizon();
			float HorizonArr[3] = { H.x, H.y, H.z };
			if (ImGui::DragFloat3("Horizon", HorizonArr, 0.01f, -1.0f, 1.0f))
			{
				Debug_Camera_Set_Horizon({ HorizonArr[0], HorizonArr[1], HorizonArr[2] });
			}

			ImGui::TreePop();
		}
	}

	ImGui::End();
}

void GUI_World_Editor()
{
    ImGui::Begin("World System");

    if (ImGui::CollapsingHeader("Global Light Control", ImGuiTreeNodeFlags_DefaultOpen))
    {
		ImGui::Text("Sun Settings");
		
		// Manage Shadow Distance
		ImGui::SliderFloat("Sun Distance", &g_Sun_Dist, 10.0f, 500.0f);

		// Sun Auto Rotation
		ImGui::Separator();
		ImGui::Checkbox("Auto Rotation", &g_IsSunRotation);

		if (g_IsSunRotation)
		{
			// If Auto, Just Manage Speed, Tilt
			ImGui::SliderFloat("Rotation Speed", &g_Sun_Speed, 0.0f, 2.0f);
			ImGui::SliderFloat("Tilt (Z-Axis)", &g_Sun_Tilt, -1.0f, 1.0f);

			float Dir[3] = { g_Sun_Dir.x, g_Sun_Dir.y, g_Sun_Dir.z };
			ImGui::InputFloat3("Sun Dir (Auto)", Dir, "%.2f", ImGuiInputTextFlags_ReadOnly);
		}
		else
		{
			// If Not Auto, Can Manage Sun POS
			ImGui::Text("Manual Direction Control");
			ImGui::DragFloat3("Sun Direction (XYZ)", &g_Sun_Dir.x, 0.01f, -1.0f, 1.0f);
		}

		ImGui::Separator();

        // Manage Sun Light Color
		ImGui::Text("Light Color");
		ImGui::ColorEdit4("Sun Color", &g_Sun_Color.x);
		ImGui::ColorEdit4("Ambient Color", &g_Ambient_Color.x);
    }

	ImGui::Separator();
	if (ImGui::Button("Reset Global Light (Default)", ImVec2(-1, 30)))
	{
		Light_Manager::GetInstance().Global_Light_Reset();
	}

    ImGui::End();
}