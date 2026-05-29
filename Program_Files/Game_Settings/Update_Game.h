/*==============================================================================

	Update Game Screen [Update_Game.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H
#include "Game_Screen_Manager.h"

void Game_Logic_Initialize();
void Game_Logic_Finalize();
void Game_Logic_Update(double elapsed_time);

void Game_Logic_Menu(Main_Screen State, float elapsed_time);
void Game_Logic_Sub(Sub_Screen State, float elapsed_time);
void Game_Logic_InGame(Game_Select_Screen State, float elapsed_time);

void Main_Game_Screen_Update();
void Sub_Game_Screen_Update();
void Game_Select_Screen_Update();

#endif // GAME_MANAGER_H