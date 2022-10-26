/*
File name: Menu_HowtoPlay.cpp
Project name: Phone! Pong!
Primary Author: Minho Chae
Copyright information:
    "All content © 2019 DigiPen (USA) Corporation, all rights reserved."
*/

#include "CommonLevel.h"
#include "Game.h"
#include "Menu_HowtoPlay.h"

const SDL_Color WHITE = { 255, 255, 255, 255 };
const SDL_Color BLACK = { 0, 0, 0, 255 };
const SDL_Color GOLD = { 255, 215, 0, 255 };
const SDL_Color SILVER = { 192,192,192, 255 };
const SDL_Color BRONZE = { 205, 127, 50, 255 };
const SDL_Color MINT = { 135, 206, 175, 255 };


void MenuHowToPlay::Initialize()
{
   

    m_backgroundColor = MINT;

    // Set Camera
    // You don't need to do this anymore
    //camera.Initialize(int(State::m_width), int(State::m_height));
    // Set position of the camera
    camera.position.Set(0, 0, 0);

    // Set main font
    // Load the font file to use as a main font
    // and set the default size of it
    mainFont = TTF_OpenFont("font/Default.ttf", 30);

    JumpAnimation.SetName("jump animation");
    JumpAnimation.transform.position.Set(-300.f, 250.f, 0.f);
    JumpAnimation.transform.SetScale(100, 140);
    JumpAnimation.sprite.Free();
    JumpAnimation.sprite.LoadImage("texture/Jump_Animation.png", State::m_renderer);
    JumpAnimation.sprite.activeAnimation = true;
    JumpAnimation.sprite.SetFrame(5);
    JumpAnimation.sprite.SetSpeed(10.f);
    AddObject(&JumpAnimation);

    RopeAnimation.SetName("rope animation");
    RopeAnimation.transform.position.Set(300.f, 250.f, 0.f);
    RopeAnimation.transform.SetScale(100, 140);
    RopeAnimation.sprite.Free();
    RopeAnimation.sprite.LoadImage("texture/Rope_Animation.png", State::m_renderer);
    RopeAnimation.sprite.activeAnimation = true;
    RopeAnimation.sprite.SetFrame(3);
    RopeAnimation.sprite.SetSpeed(8.f);
    AddObject(&RopeAnimation);


    AltAnimation.SetName("alt animation");
    AltAnimation.transform.position.Set(-400.f, 120.f, 0.f);
    AltAnimation.transform.SetScale(100, 100);
    AltAnimation.sprite.Free();
    AltAnimation.sprite.LoadImage("texture/Space.png", State::m_renderer);
    AltAnimation.sprite.activeAnimation = true;
    AltAnimation.sprite.SetFrame(2);
    AltAnimation.sprite.SetSpeed(2.f);
    AddObject(&AltAnimation);

    CtrlAnimation.SetName("ctrl animation");
    CtrlAnimation.transform.position.Set(200.f, 120.f, 0.f);
    CtrlAnimation.transform.SetScale(100, 100);
    CtrlAnimation.sprite.Free();
    CtrlAnimation.sprite.LoadImage("texture/Enter.png", State::m_renderer);
    CtrlAnimation.sprite.activeAnimation = true;
    CtrlAnimation.sprite.SetFrame(2);
    CtrlAnimation.sprite.SetSpeed(2.f);
    AddObject(&CtrlAnimation);


    CreateAndRegisterHudText(&JumpText, "-  JUMP", "JUMP");
    JumpText.transform.SetScale(150.f, 60.f);
    JumpText.transform.position.Set(-250.f, 120.f, 0.f);

    CreateAndRegisterHudText(&RopeText, "-  ROPE", "ROPE");
    RopeText.transform.SetScale(150.f, 60.f);
    RopeText.transform.position.Set(350.f, 120.f, 0.f);

    CreateAndRegisterHudText(&JumpExplainText, "You should do jump before using the rope!", "JumpExplain");
    JumpExplainText.transform.SetScale(500.f, 25.f);
    JumpExplainText.transform.position.Set(-300.f, 50.f, 0.f); 

    CreateAndRegisterHudText(&RopeExplainText, "By using the rope, you can charge the battery!", "JumpExplain");
    RopeExplainText.transform.SetScale(500.f, 25.f);
    RopeExplainText.transform.position.Set(300.f, 50.f, 0.f);

    CreateAndRegisterHudText(&FeverTimeText, "Fever Time", "Fever Time");
    FeverTimeText.transform.SetScale(250.f, 60.f);
    FeverTimeText.transform.position.Set(0.f, -50.f, 0.f);

    CreateAndRegisterHudText(&FeverTimeExplainText_1, "If you collect all alphabets(D,I,G,I,P,E,N),", "Fever Time Explain 1");
    FeverTimeExplainText_1.transform.SetScale(800.f, 30.f);
    FeverTimeExplainText_1.transform.position.Set(0.f, -120.f, 0.f);

    CreateAndRegisterHudText(&FeverTimeExplainText_2, "you're going to go to a special stage where", "Fever Time Explain 2");
    FeverTimeExplainText_2.transform.SetScale(800.f, 30.f);
    FeverTimeExplainText_2.transform.position.Set(0.f, -160.f, 0.f);

    CreateAndRegisterHudText(&FeverTimeExplainText_3, "you can get massive extra score!", "Fever Time Explain 3");
    FeverTimeExplainText_3.transform.SetScale(650.f, 30.f);
    FeverTimeExplainText_3.transform.position.Set(0.f, -200.f, 0.f);

    CreateAndRegisterHudText(&Key, "<Press Enter to MainMenu>", "Key");
    Key.transform.SetScale(600.f, 40.f);
    Key.transform.position.Set(0.f, -330.f, 0.f);
}

void MenuHowToPlay::Update(float dt)
{
    if (State::m_input->IsTriggered(SDL_SCANCODE_RETURN))
    {
        State::m_game->Change(LV_MainMenu);
    }
    UpdatePhysics(dt);
    Render(dt);
}
void MenuHowToPlay::Close()
{
    // Wrap up state
    ClearBaseState();
}

void MenuHowToPlay::CreateAndRegisterHudText(Object *textObject, const char *textContents, const char *id)
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