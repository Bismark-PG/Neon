/*==============================================================================

	Manage Map Scroll [Map_Manager.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Project_Header.h"
#include "Map_Manager.h"
#include "Texture_Manager.h"
#include "Billboard.h"

void Map_Manager::Init()
{
    m_FloorTexID = Texture_Manager::GetInstance()->GetID("BG_F"); 
    m_SkyTexID = Texture_Manager::GetInstance()->GetID("BG_S");

    m_Current_Floor_V = 0.0f;
    m_Current_Sky_U = 0.0f;
}

void Map_Manager::Final()
{

}

void Map_Manager::Reset()
{
    m_Current_Floor_V = 0.0f;
    m_Current_Sky_U = 0.0f;
}

void Map_Manager::Update(float dt)
{
	// Scroll Map By UV Scroll
    m_Current_Floor_V -= m_Scroll_Speed * dt;

    // Infinite Treadmill Effect
    // When V < -1.0, wrap it back to 0.0
    if (m_Current_Floor_V <= -1.0f) 
    {
        m_Current_Floor_V += 1.0f;
    }

	// Scroll Sky By UV Scroll
    m_Current_Sky_U -= m_Sky_Scroll_Speed * dt;

	// Infinite Treadmill Effect
	// When U < -1.0, wrap it back to 0.0
    if (m_Current_Sky_U <= -1.0f)
    {
        m_Current_Sky_U += 1.0f;
    }
}

void Map_Manager::Draw()
{
    Draw_Sky();
    Draw_Floor();
}

void Map_Manager::Draw_Floor()
{
    if (m_FloorTexID == -1) return;

    UV_Parameter Floor_UV = {};
	Floor_UV.scale = { 10.0f, 10.0f }; // Scaling For Tiling
	Floor_UV.translation = { 0.0f, m_Current_Floor_V }; // V Scroll

	// Floor Size (X is Width, Z is Depth)
    float Floor_Size_X = 500.0f;
	float Floor_Size_Z = 500.0f; // Z is Depth

    // Floor Position
    DirectX::XMFLOAT3 Floor_Pos = { 0.0f, -2.0f, 200.0f };

    // Trandmill Effect
    Billboard_Draw_Internal(m_FloorTexID, Floor_Pos, Floor_Size_X, Floor_Size_Z,
        { 0.5f, 0.5f }, Floor_UV, { 1.0f, 1.0f, 1.0f, 1.0f }, Billboard_Facing::FLOOR);
}

void Map_Manager::Draw_Sky()
{
    if (m_SkyTexID == -1) return;

    UV_Parameter Sky_UV = {};
	Sky_UV.scale = { 4.0f, 2.0f }; // Scaling For Tiling
	Sky_UV.translation = { m_Current_Sky_U, 0.0f }; // U Scroll

	// Sky Size (X is Width, Y is Height)
    float Sky_Size_X = 1500.0f;
    float Sky_Size_Y = 800.0f;

	// Sky Position
    DirectX::XMFLOAT3 Sky_Pos = { 0.0f, 200.0f, 400.0f };

    // Trandmill Effect
    Billboard_Draw_Internal(m_SkyTexID, Sky_Pos, Sky_Size_X, Sky_Size_Y,
        { 0.5f, 0.5f }, Sky_UV, { 1.0f, 1.0f, 1.0f, 1.0f }, Billboard_Facing::FIXED_NONE);
}