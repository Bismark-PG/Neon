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
   

    //---------------------------------------------<< Card Texture >>---------------------------------------------//

    
    //----------------------------------------------<< BG Texture >>----------------------------------------------//


    //----------------------------------------------<< UI Texture >>----------------------------------------------//

    //-------------------------------------------------Others Texture-------------------------------------------------//
    Texture_M->Load("Bismark_LOGO", L"Resource/Texture/Other/Bismark.jpg");
    Texture_M->Load("Bismark_LOGO_B", L"Resource/Texture/Other/Bismark_Black.jpg");
    Texture_M->Load("Bismark_Name", L"Resource/Texture/Other/Name_W.png");
    Texture_M->Load("Pixel_Withe", L"Resource/Texture/Other/W_Pixel.png");
    Texture_M->Load("Pixel_Black", L"Resource/Texture/Other/B_Pixel.png");

    // Sample
    Texture_M->Load("TextSample", L"Resource/Texture/BG/RGBCMYK/White.png");
}