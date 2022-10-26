/*
File name: Splash.cpp
Project name: Phone! Pong!
Primary Author: Minho Chae
Copyright information:
    "All content © 2019 DigiPen (USA) Corporation, all rights reserved."
*/

#include "CommonLevel.h"
#include "Game.h"


const SDL_Color WHITE = { 255, 255, 255, 255 };
const SDL_Color GOLD = { 255, 215, 0, 255 };
const SDL_Color SILVER = { 192,192,192, 255 };
const SDL_Color BRONZE = { 205, 127, 50, 255 };


void Splash::Initialize()
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

    Digipen.SetName("Digipen Logo");
    Digipen.transform.position.Set(0.f, 0.f, 0.f);
    Digipen.transform.SetScale(1280, 720);
    Digipen.sprite.Free();
    Digipen.sprite.LoadImage("texture/Digipen.png", State::m_renderer);
    AddObject(&Digipen);
}

void Splash::Update(float dt)
{
    splash_count -= dt;
    if (splash_count <= 0)
    {
        State::m_game->Change(LV_MainMenu);
    }
    UpdatePhysics(dt);
    Render(dt);
}
void Splash::Close()
{
    // Wrap up state
    ClearBaseState();
}

void Splash::CreateAndRegisterHudText(Object *textObject, const char *textContents, const char *id)
{
    // Set the object's name
    textObject->SetName(id);

    // Set the text to render out
    textObject->text.SetText(State::m_renderer, textContents, mainFont);

    // Set the colors r,g,b,a (0~255)
    textObject->text.color = WHITE;

    // Set either if the object to be hud or not
    textObject->text.isHud = true;

    // Register the object to the state
    AddObject(textObject);
}