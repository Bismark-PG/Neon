/*==============================================================================

    Main Game Screen [Main_Game.h]

    Author : Choi HyungJoon

==============================================================================*/
#pragma once
#ifndef MAIN_GAME_H
#define MAIN_GAME_H


class Main_Game_Manager
{
public:
    static Main_Game_Manager* GetIntance();

    Main_Game_Manager(const Main_Game_Manager&) = delete;
    Main_Game_Manager& operator=(const Main_Game_Manager&) = delete;

    void In_Game_Initialize();
    void In_Game_Finalize();

    void In_Game_Reset();

    void In_Game_Update(float elapsed_time);
    void In_Game_Draw();

private:
    Main_Game_Manager() = default;
    ~Main_Game_Manager() = default;
};
#endif // MAIN_GAME_H