/*==============================================================================

    Management Global Game Light [Light_Manager.h]

    Author : Choi HyungJoon

==============================================================================*/
#include "Project_Header.h"
#include "Light_Manager.h"
#include "Shader_Manager.h"
#include "Palette.h"       
using namespace DirectX;

XMFLOAT3 Direction = { 0.0f, -1.0f, 0.0f };
XMFLOAT4 Color_Direction = { 1.0f, 1.0f, 1.0f, 1.0f };
XMFLOAT4 Color_Ambient = { 0.2f, 0.2f, 0.2f, 1.0f };

Light_Manager& Light_Manager::GetInstance()
{
    static Light_Manager instance;
    return instance;
}

void Light_Manager::Init()
{
    Global_Light_Reset();

    // Set Ambient, World Light And Color
    m_Ambient.Color = Color_Ambient;
    m_Directional.Color = Color_Direction;

	// Point Lights Initialization
    for (int i = 0; i < 4; i++)
    {
        m_PointLights[i].Position = { 0,0,0 };
        m_PointLights[i].Range = 0.0f;
        m_PointLights[i].Color = { 0,0,0,0 };
    }
    m_PointLightCount = 0;
}

void Light_Manager::Global_Light_Set_Up() const
{
    if (!Shader_Manager::GetInstance()) return;

    Shader_Manager::GetInstance()->SetLightAmbient(m_Ambient.Color);
    Shader_Manager::GetInstance()->SetLightDirectional(m_Directional.Vector, m_Directional.Color);

    Shader_Manager::GetInstance()->SetPointLightCount(m_PointLightCount);
    for (int i = 0; i < 4; i++)
    {
        Shader_Manager::GetInstance()->SetPointLight(i,
            m_PointLights[i].Position,
            m_PointLights[i].Range,
            m_PointLights[i].Color);
    }

    Shader_Manager::GetInstance()->SetDiffuseColor({ 1.0f, 1.0f, 1.0f, 1.0f });
}

void Light_Manager::Global_Light_Update(float elapsed_time)
{
	// Point Light Update Logic (If Needed)
}

// --- Setters ---
void Light_Manager::Set_Ambient_Color(const DirectX::XMFLOAT4& color)
{
    m_Ambient.Color = color;
}

void Light_Manager::Set_Directional_Light(const DirectX::XMFLOAT4& dir, const DirectX::XMFLOAT4& color)
{
    XMVECTOR Dir = XMLoadFloat4(&dir);
    Dir = XMVector3Normalize(Dir);
    XMStoreFloat4(&m_Directional.Vector, Dir);

    m_Directional.Color = color;
}

void Light_Manager::Set_Point_Light(int index, const DirectX::XMFLOAT3& pos, float range, const DirectX::XMFLOAT4& color)
{
    if (index < 0 || index >= 4) return;
    m_PointLights[index].Position = pos;
    m_PointLights[index].Range = range;
    m_PointLights[index].Color = color;
}

void Light_Manager::Set_Point_Light_Active_Count(int count)
{
    m_PointLightCount = count;
}

void Light_Manager::Global_Light_Reset()
{
    // Reset
    m_Ambient.Color = Color_Ambient;
    m_Directional.Color = Color_Direction;
}
