/*==============================================================================

	Manage Map Scroll [Map_Manager.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H
#include <DirectXMath.h>

class Map_Manager
{
public:
    static Map_Manager& GetInstance() 
    {
        static Map_Manager instance;
        return instance;
    }

    void Init();
    void Final();

    void Reset();
    void Update(float dt);

    void Draw();

private:
    Map_Manager() = default;
    ~Map_Manager() = default;

    void Draw_Floor();
    void Draw_Sky();

    // Floor Parameter
    float m_Scroll_Speed = 1.0f; // Map Scroll Speed
	float m_Current_Floor_V = 0.0f;    // U Scroll Value for Floor
    int m_FloorTexID = -1;

    // Sky Parameter
	float m_Sky_Scroll_Speed = 0.05f; // Sky Scroll Speed
	float m_Current_Sky_U = 0.0f;     // V Scroll Value for Sky
    int m_SkyTexID = -1;
};
#endif // MAP_MANAGER_H