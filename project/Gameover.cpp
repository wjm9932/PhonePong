/*
File name: Gameover.cpp
Project name: Phone! Pong!
Primary Author: Jaemin Woo
Secondary Author: Gyuyoung Park
Copyright information:
    "All content © 2019 DigiPen (USA) Corporation, all rights reserved."
*/

#include "CommonLevel.h"
#include "Game.h"
#include "Gameover.h"
#include "yoonsooLV1.h"
#include <iostream>

const SDL_Color WHITE = { 255, 255, 255, 255 };
const SDL_Color BLACK = { 0, 0, 0, 255 };
const SDL_Color GOLD = { 255, 215, 0, 255 };
const SDL_Color SILVER = { 192,192,192, 255 };
const SDL_Color BRONZE = { 205, 127, 50, 255 };


void Gameover::Initialize()
{

    m_backgroundColor = { 0, 0, 255,255 };

    Background.SetName("background");
    Background.transform.position.Set(0.f, 0.f, 0.f);
    Background.transform.SetScale(1280, 720.f);
    Background.sprite.Free();
    Background.sprite.LoadImage("texture/Ranking_Background.png", State::m_renderer);
    AddObject(&Background);

    // Set Camera
    // You don't need to do this anymore
    //camera.Initialize(int(State::m_width), int(State::m_height));
    // Set position of the camera
    camera.position.Set(0, 0, 0);

    // Set main font
    // Load the font file to use as a main font
    // and set the default size of it
    mainFont = TTF_OpenFont("font/Default.ttf", 30);

    rankText1st.transform.position.Set(-150.0f, 220.f, 0.f);
    rankText1st.transform.SetScale(100.f, 70.f);
    rankText1st.text.font = mainFont;
    rankText1st.text.isHud = true;
    rankText1st.text.color = GOLD;
    rankText1st.text.SetText("1st");
    AddObject(&rankText1st);

    rankText1st_1.transform.position.Set(0.0f, 220.f, 0.f);
    rankText1st_1.transform.SetScale(100.f, 70.f);
    rankText1st_1.text.font = mainFont;
    rankText1st_1.text.isHud = true;
    rankText1st_1.text.color = GOLD;
    rankText1st_1.text.SetText(" %s:", players[4].first.c_str());
    AddObject(&rankText1st_1);


    rankText1st_2.transform.position.Set(150.0f, 220.f, 0.f);
    rankText1st_2.transform.SetScale(100.f, 70.f);
    rankText1st_2.text.font = mainFont;
    rankText1st_2.text.isHud = true;
    rankText1st_2.text.color = GOLD;
    rankText1st_2.text.SetText(" %d", players[4].second);
    AddObject(&rankText1st_2);

    rankText2nd.transform.position.Set(-150.0f, 130.f, 0.f);
    rankText2nd.transform.SetScale(100.f, 70.f);
    rankText2nd.text.font = mainFont;
    rankText2nd.text.isHud = true;
    rankText2nd.text.color = SILVER;
    rankText2nd.text.SetText("2nd");
    AddObject(&rankText2nd);

    rankText2nd_1.transform.position.Set(0.0f, 130.f, 0.f);
    rankText2nd_1.transform.SetScale(100.f, 70.f);
    rankText2nd_1.text.font = mainFont;
    rankText2nd_1.text.isHud = true;
    rankText2nd_1.text.color = SILVER;
    rankText2nd_1.text.SetText(" %s:", players[3].first.c_str());
    AddObject(&rankText2nd_1);

    rankText2nd_2.transform.position.Set(150.0f, 130.f, 0.f);
    rankText2nd_2.transform.SetScale(100.f, 70.f);
    rankText2nd_2.text.font = mainFont;
    rankText2nd_2.text.isHud = true;
    rankText2nd_2.text.color = SILVER;
    rankText2nd_2.text.SetText(" %d", players[3].second);
    AddObject(&rankText2nd_2);

    rankText3rd.transform.position.Set(-150.0f, 40.f, 0.f);
    rankText3rd.transform.SetScale(100.f, 70.f);
    rankText3rd.text.font = mainFont;
    rankText3rd.text.isHud = true;
    rankText3rd.text.color = BRONZE;
    rankText3rd.text.SetText("3rd");
    AddObject(&rankText3rd);

    rankText3rd_1.transform.position.Set(0.0F, 40.f, 0.f);
    rankText3rd_1.transform.SetScale(100.f, 70.f);
    rankText3rd_1.text.font = mainFont;
    rankText3rd_1.text.isHud = true;
    rankText3rd_1.text.color = BRONZE;
    rankText3rd_1.text.SetText(" %s:", players[2].first.c_str());
    AddObject(&rankText3rd_1);

    rankText3rd_2.transform.position.Set(150.0f, 40.f, 0.f);
    rankText3rd_2.transform.SetScale(100.f, 70.f);
    rankText3rd_2.text.font = mainFont;
    rankText3rd_2.text.isHud = true;
    rankText3rd_2.text.color = BRONZE;
    rankText3rd_2.text.SetText(" %d", players[2].second);
    AddObject(&rankText3rd_2);

    rankText4th.transform.position.Set(-150.0f, -50.f, 0.f);
    rankText4th.transform.SetScale(100.f, 70.f);
    rankText4th.text.font = mainFont;
    rankText4th.text.isHud = true;
    rankText4th.text.color = BLACK;
    rankText4th.text.SetText("4th");
    AddObject(&rankText4th);

    rankText4th_1.transform.position.Set(0.0f, -50.f, 0.f);
    rankText4th_1.transform.SetScale(100.f, 70.f);
    rankText4th_1.text.font = mainFont;
    rankText4th_1.text.isHud = true;
    rankText4th_1.text.color = BLACK;
    rankText4th_1.text.SetText(" %s:", players[1].first.c_str());
    AddObject(&rankText4th_1);

    rankText4th_2.transform.position.Set(150.0f, -50.f, 0.f);
    rankText4th_2.transform.SetScale(100.f, 70.f);
    rankText4th_2.text.font = mainFont;
    rankText4th_2.text.isHud = true;
    rankText4th_2.text.color = BLACK;
    rankText4th_2.text.SetText(" %d", players[1].second);
    AddObject(&rankText4th_2);

    rankText5th.transform.position.Set(-150.0f, -140.f, 0.f);
    rankText5th.transform.SetScale(100.f, 70.f);
    rankText5th.text.font = mainFont;
    rankText5th.text.isHud = true;
    rankText5th.text.color = BLACK;
    rankText5th.text.SetText("5th");
    AddObject(&rankText5th);

    rankText5th_1.transform.position.Set(0.0f, -140.f, 0.f);
    rankText5th_1.transform.SetScale(100.f, 70.f);
    rankText5th_1.text.font = mainFont;
    rankText5th_1.text.isHud = true;
    rankText5th_1.text.color = BLACK;
    rankText5th_1.text.SetText(" %s:", players[0].first.c_str());
    AddObject(&rankText5th_1);

    rankText5th_2.transform.position.Set(150.0f, -140.f, 0.f);
    rankText5th_2.transform.SetScale(100.f, 70.f);
    rankText5th_2.text.font = mainFont;
    rankText5th_2.text.isHud = true;
    rankText5th_2.text.color = BLACK;
    rankText5th_2.text.SetText(" %d", players[0].second);
    AddObject(&rankText5th_2);

    CreateAndRegisterHudText(&Key, "<Press Enter to Play / Press M to MainMenu>", "Key");
    Key.transform.SetScale(600.f, 50.f);
    Key.text.color = BLACK;
    Key.transform.position.Set(0.f, -260.f, 0.f);
}
void Gameover::Update(float dt)
{
    if (State::m_input->IsTriggered(SDL_SCANCODE_RETURN))
    {


        State::m_game->Change((LV_LevelYoonsoo));

    }
    else if (State::m_input->IsPressed(SDL_SCANCODE_M))
    {

        State::m_game->Change(LV_MainMenu);

    }
    UpdatePhysics(dt);
    Render(dt);
}
void Gameover::Close()
{
    // Wrap up state
    ClearBaseState();
    score = 0;
}
void Gameover::CreateAndRegisterHudText(Object *textObject, const char *textContents, const char *id)
{
    // Set the object's name
    textObject->SetName(id);

    // Set the scale of the object
    textObject->transform.SetScale(650, 130);

    // Set the text to render out
    textObject->text.SetText(State::m_renderer, textContents, mainFont);

    // Set the colors r,g,b,a (0~255)
    textObject->text.color = WHITE;

    // Set either if the object to be hud or not
    textObject->text.isHud = true;

    // Register the object to the state
    AddObject(textObject);
}
