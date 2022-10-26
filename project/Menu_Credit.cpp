/*
File name: Menu_Credit.cpp
Project name: Phone! Pong!
Primary Author: Minho Chae
Copyright information:
    "All content © 2019 DigiPen (USA) Corporation, all rights reserved."
*/

#include "CommonLevel.h"
#include "Game.h"
#include "Menu_Credit.h"

const SDL_Color WHITE = { 255, 255, 255, 255 };
const SDL_Color BLACK = { 0,0, 0, 255 };
const SDL_Color GOLD = { 255, 215, 0, 255 };
const SDL_Color SILVER = { 192,192,192, 255 };
const SDL_Color BRONZE = { 205, 127, 50, 255 };


void MenuCredit::Initialize()
{
    m_backgroundColor = { 0, 0, 255,255 };

    // Set Camera
    // You don't need to do this anymore
    //camera.Initialize(int(State::m_width), int(State::m_height));
    // Set position of the camera
    camera.position.Set(0, 0, 0);

    // Set main font
    // Load the font file to use as a main font
    // and set the default size of it
    mainFont = TTF_OpenFont("font/Default.ttf", 30);

    Background.SetName("background");
    Background.transform.position.Set(0.f, 0.f, 0.f);
    Background.transform.SetScale(1280, 720);
    Background.sprite.Free();
    Background.sprite.LoadImage("texture/Credit_Background.png", State::m_renderer);
    AddObject(&Background);


    CreateAndRegisterHudText(&President, "President: Claude Comair", "President");
    President.transform.SetScale(360.f, 30.f);
    President.transform.position.Set(0.f, 300.f, 0.f);

    CreateAndRegisterHudText(&Instructor, "Instructor: David Ly", "Instructor");
    Instructor.transform.SetScale(300.f, 30.f);
    Instructor.transform.position.Set(0.f, 260.f, 0.f);

    CreateAndRegisterHudText(&TeachingAssistant, "TeachingAssistant: Hun Yang", "TeachingAssistant");
    TeachingAssistant.transform.SetScale(440.f, 30.f);
    TeachingAssistant.transform.position.Set(0.f, 220.f, 0.f);

    CreateAndRegisterHudText(&Shinil, "Producer: Kang Shinil", "Shinil");
    Shinil.transform.SetScale(300.f, 30.f);
    Shinil.transform.position.Set(0.f, 180.f, 0.f);

    CreateAndRegisterHudText(&Yoonsoo, "Technical Director: Kwon Yoonsoo", "Yoonsoo");
    Yoonsoo.transform.SetScale(480.f, 30.f);
    Yoonsoo.transform.position.Set(0.f, 140.f, 0.f);

    CreateAndRegisterHudText(&Jaemin, "Test Manager: Woo Jaemin", "Jaemin");
    Jaemin.transform.SetScale(380.f, 30.f);
    Jaemin.transform.position.Set(0.f, 100.f, 0.f);

    CreateAndRegisterHudText(&Minho, "Lead Designer: Chae Minho", "Minho");
    Minho.transform.SetScale(400.f, 30.f);
    Minho.transform.position.Set(0.f, 60.f, 0.f);

    CreateAndRegisterHudText(&Gyuyoung, "Product Manager: Park Gyuyoung", "Gyuyoung");
    Gyuyoung.transform.SetScale(500.f, 30.f);
    Gyuyoung.transform.position.Set(0.f, 20.f, 0.f);

    CreateAndRegisterHudText(&Sound, "Sound:", "Sound");
    Sound.transform.SetScale(100.f, 30.f);
    Sound.transform.position.Set(0.f, -20.f, 0.f);

    CreateAndRegisterHudText(&Sound_Copyright, "https://creativecommons.org/licenses/by/3.0/", "Sound Copyright");
    Sound_Copyright.transform.SetScale(500.f, 30.f);
    Sound_Copyright.transform.position.Set(0.f, -60.f, 0.f);

    CreateAndRegisterHudText(&Sound_Copyright_Jump_1, "\"Platformer Jumping Sounds\"by dklon licensed CC BY 3.0: ", "Sound Jump 1");
    Sound_Copyright_Jump_1.transform.SetScale(700.f, 30.f);
    Sound_Copyright_Jump_1.transform.position.Set(0.f, -100.f, 0.f);

    CreateAndRegisterHudText(&Sound_Copyright_Jump_2, "https://opengameart.org/content/platformer-jumping-sounds", "Sound Jump 2");
    Sound_Copyright_Jump_2.transform.SetScale(700.f, 30.f);
    Sound_Copyright_Jump_2.transform.position.Set(0.f, -130.f, 0.f);

    CreateAndRegisterHudText(&Sound_Copyright_Rank_1, "\"Circus Dilemma\"by Matthew Pablo licensed CC BY 3.0: ", "Sound Rank 1");
    Sound_Copyright_Rank_1.transform.SetScale(700.f, 30.f);
    Sound_Copyright_Rank_1.transform.position.Set(0.f, -170.f, 0.f);

    CreateAndRegisterHudText(&Sound_Copyright_Rank_2, "https://opengameart.org/content/circus-dilemma", "Sound Rank 2");
    Sound_Copyright_Rank_2.transform.SetScale(700.f, 30.f);
    Sound_Copyright_Rank_2.transform.position.Set(0.f, -200.f, 0.f);

    CreateAndRegisterHudText(&Key, "<Press Enter to MainMenu>", "Key");
    Key.transform.SetScale(600.f, 40.f);
    Key.transform.position.Set(0.f, -330.f, 0.f);
}

void MenuCredit::Update(float dt)
{
    if (State::m_input->IsTriggered(SDL_SCANCODE_RETURN))
    {
        State::m_game->Change(LV_MainMenu);
    }
    UpdatePhysics(dt);
    Render(dt);
}
void MenuCredit::Close()
{
    // Wrap up state
    ClearBaseState();
}

void MenuCredit::CreateAndRegisterHudText(Object *textObject, const char *textContents, const char *id)
{
    // Set the object's name
    textObject->SetName(id);

    // Set the text to render out
    textObject->text.SetText(State::m_renderer, textContents, mainFont);

    // Set the colors r,g,b,a (0~255)
    textObject->text.color = BLACK;

    // Set either if the object to be hud or not
    textObject->text.isHud = true;

    // Register the object to the state
    AddObject(textObject);
}