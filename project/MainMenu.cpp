/*
File name: MainMenu.cpp
Project name: Phone! Pong!
Primary Author: Minho Chae
Copyright information:
    "All content © 2019 DigiPen (USA) Corporation, all rights reserved."
*/

#include "CommonLevel.h"

const SDL_Color WHITE = { 255, 255, 255, 255 };
const SDL_Color MAROON = { 128, 0, 0, 255 };
const SDL_Color BLACK = { 0, 0, 0, 255 };

extern Rank players[5] = { Rank{"-", 0}, Rank{"-", 0} , Rank{"-", 0}, Rank{"-", 0}, Rank{"-", 0} };

void MainMenu::Initialize()
{
    backgroundMusic.LoadMusic("sound/AI_2.mp3");
    m_pBGM = &backgroundMusic;
    backgroundMusic.Play();

	// Set Camera
	// You don't need to do this anymore
	//camera.Initialize(int(State::m_width), int(State::m_height));
	// Set position of the camera
	camera.position.Set(0, 0, 0);

	// Set main font
	// Load the font file to use as a main font
	// and set the default size of it
	mainFont = TTF_OpenFont("font/Default.ttf", 48);

    //initialize and add background 
    background.SetName("Background");
    background.transform.position.Set(0, 0, 0);
    background.transform.SetScale(1280.f, 720.f);
    background.sprite.Free();
    background.sprite.LoadImage("texture/background_play.png", m_renderer);
    background.sprite.isHud = true;
    AddObject(&background);

    select.SetName("select");
    select.transform.position.Set(-130, 0, 0);
    select.transform.SetScale(50.f, 50.f);
    select.sprite.Free();
    select.sprite.LoadImage("texture/select.png", m_renderer);
    select.sprite.isHud = true;
    AddObject(&select);

    clickSound.sound.LoadSound("sound/click_sound.wav");
    clickSound.sound.SetVolume(64);
    AddObject(&clickSound);

}

void MainMenu::Update(float dt)
{
	/******************* State ******************/

    // function for button event
    ButtonEvent();

	// Must be one of the last functions called at the end of State Update
	UpdatePhysics(dt);
	Render(dt);
}

void MainMenu::Close()
{
	// Wrap up state
    backgroundMusic.Free();
    //clickSound.sound.Free();
	ClearBaseState();
}

void MainMenu::CreateAndRegisterHudText(Object *textObject, const char *textContents, const char *id)
{
	// Set the object's name
	textObject->SetName(id);
    
    textObject->transform.SetScale(300, 80);
    

	// Set the text to render out
	textObject->text.SetText(State::m_renderer, textContents, mainFont);

	// Set the colors r,g,b,a (0~255)
    textObject->text.color = BLACK;

	// Set either if the object to be hud or not
	textObject->text.isHud = true;

	// Register the object to the state
	AddObject(textObject);
}

// function for button event
void MainMenu::ButtonEvent()
{

    if (m_input->IsTriggered(SDL_SCANCODE_DOWN))
    {
        if (select.transform.position.y > CREDIT_POSITION) {
            select.transform.position.y -= KEY_MOVE;
        }
    }
    if (m_input->IsTriggered(SDL_SCANCODE_UP))
    {
        if (select.transform.position.y < PLAY_POSITION) {
            select.transform.position.y += KEY_MOVE;
        }

    }

    if (m_input->IsTriggered(SDL_SCANCODE_RETURN))
    {
        switch (static_cast<int>(select.transform.position.y))
        {
            case PLAY_POSITION:
                m_game->Change(LV_LevelYoonsoo);
                break;
            case HOW_TO_PLAY_POSITION:
                m_game->Change(LV_MenuHowToPlay);
                break;
            case RANKING_POSITION:
                m_game->Change(LV_MenuRanking);
                break;
            case CREDIT_POSITION:
                m_game->Change(LV_MenuCredit);
                break;
        }
    }

    switch (static_cast<int>(select.transform.position.y))
    {
    case PLAY_POSITION:
        background.sprite.Free();
        background.sprite.LoadImage("texture/background_play.png", m_renderer);
        break;
    case HOW_TO_PLAY_POSITION:
        background.sprite.Free();
        background.sprite.LoadImage("texture/background_howtoplay.png", m_renderer);
        break;
    case RANKING_POSITION:
        background.sprite.Free();
        background.sprite.LoadImage("texture/background_ranking.png", m_renderer);
        break;
    case CREDIT_POSITION:
        background.sprite.Free();
        background.sprite.LoadImage("texture/background_credit.png", m_renderer);
        break;
    }
}

