/*==============================================================================

    Management Texture Resource [Game_Texture.h]

    Author : Choi HyungJoon

==============================================================================*/
#include "Project_Header.h"
#include "Game_Texture.h"
#include "Texture_Manager.h"

Texture_Manager* Texture_M;

void Game_Texture_Initialize()
{
    Texture_M = Texture_Manager::GetInstance();
    Game_Texture_Load();
}

void Game_Texture_Finalize()
{
}

void Game_Texture_Load()
{
    //------------------------------------------------Debug Texture------------------------------------------------//
    Texture_M->Load("Debug_Text", L"Resource/Texture/Other/Console_ASCII_512.png");
    
    Texture_M->Load("W", L"Resource/Texture/BG/RGBCMYK/White.png");
    Texture_M->Load("R", L"Resource/Texture/BG/RGBCMYK/Red.png");
    Texture_M->Load("G", L"Resource/Texture/BG/RGBCMYK/Green.png");
    Texture_M->Load("B", L"Resource/Texture/BG/RGBCMYK/Blue.png");
    Texture_M->Load("C", L"Resource/Texture/BG/RGBCMYK/Cyan.png");
    Texture_M->Load("M", L"Resource/Texture/BG/RGBCMYK/Magenta.png");
    Texture_M->Load("Y", L"Resource/Texture/BG/RGBCMYK/Yellow.png");
    Texture_M->Load("K", L"Resource/Texture/BG/RGBCMYK/Black.png");

    //-------------------------------------------<< In Game Texture >>-------------------------------------------//
    //---------------------In Game : Player Texture---------------------//
    Texture_M->Load("Player", L"Resource/Texture/Other/OBJ.png");
   
    //---------------------In Game : Enemy Texture---------------------//
    Texture_M->Load("Enemy", L"Resource/Texture/Other/HP.png");

    //---------------------In Game : Effect Texture---------------------//
    Texture_M->Load("Effect_Explosion", L"Resource/Texture/Animation/Explosion.png");
    
    //---------------------------------------------<< Card Texture >>---------------------------------------------//

    
    //----------------------------------------------<< BG Texture >>----------------------------------------------//
    Texture_M->Load("BG_F", L"Resource/Texture/BG/BG_Floor.png");
    Texture_M->Load("BG_S", L"Resource/Texture/BG/BG_Sky.png");

    //----------------------------------------------<< UI Texture >>----------------------------------------------//
    //---------------------UI : Title Texture---------------------//
    Texture_M->Load("Title_Bracket", L"Resource/Texture/UI/Intro/Title_Bracket.png");
    Texture_M->Load("Title_Bracket_RGB", L"Resource/Texture/UI/Intro/Title_Bracket_Alpha.png");

    //---------------------UI : Title Logo Texture---------------------//
    Texture_M->Load("Intro_Logo", L"Resource/Texture/UI/Intro/Title_Logo.png");
    Texture_M->Load("Intro_Made_By", L"Resource/Texture/UI/Intro/Title_MadeBy.png");
    Texture_M->Load("Intro_Corporation", L"Resource/Texture/UI/Intro/Title_Corporation.png");
    Texture_M->Load("Intro_Name", L"Resource/Texture/UI/Intro/Title_Name.png");

    //---------------------UI : Title Intro Texture---------------------//
    Texture_M->Load("Intro_SystemOS", L"Resource/Texture/UI/Intro/Title_System.png");
    Texture_M->Load("Intro_DtatInit", L"Resource/Texture/UI/Intro/Title_Data.png");
    Texture_M->Load("Intro_Ready", L"Resource/Texture/UI/Intro/Title_Ready.png");
    Texture_M->Load("Intro_Oper", L"Resource/Texture/UI/Intro/Title_Operation_Alpha.png");
    Texture_M->Load("Intro_Nova", L"Resource/Texture/UI/Intro/Title_Nova_Alpha.png");
    Texture_M->Load("Intro_Flare", L"Resource/Texture/UI/Intro/Title_Flare_Anim.png");
    
    //---------------------UI : Main Menu Texture---------------------//
    Texture_M->Load("Start", L"Resource/Texture/UI/Menu_Start.png");
    Texture_M->Load("Settings", L"Resource/Texture/UI/Menu_Settings.png");
    Texture_M->Load("Exit", L"Resource/Texture/UI/Menu_Exit.png");
    Texture_M->Load("UI_Back", L"Resource/Texture/UI/Menu_Back.png");

    //---------------------UI : Mouse UI Texture---------------------//
    Texture_M->Load("UI_Mouse_Cursor", L"Resource/Texture/UI/Aim.png");

    //-------------------------------------------------Others Texture-------------------------------------------------//
    Texture_M->Load("Bismark_LOGO", L"Resource/Texture/Other/Bismark.jpg");
    Texture_M->Load("Bismark_LOGO_B", L"Resource/Texture/Other/Bismark_Black.jpg");
    Texture_M->Load("Bismark_Name", L"Resource/Texture/Other/Name_W.png");
    Texture_M->Load("Pixel_Withe", L"Resource/Texture/Other/W_Pixel.png");
    Texture_M->Load("Pixel_Black", L"Resource/Texture/Other/B_Pixel.png");

    // Sample
    Texture_M->Load("TextSample", L"Resource/Texture/BG/RGBCMYK/White.png");
}