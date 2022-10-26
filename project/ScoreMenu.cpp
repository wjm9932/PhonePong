/*
File name: ScoreMenu.cpp
Project name: Phone! Pong!
Primary Author: Jaemin Woo
Copyright information:
    "All content © 2019 DigiPen (USA) Corporation, all rights reserved."
*/
/*
File name: ScoreMenu.cpp
Project name:
Primary Author: Jaemin Woo
Secondary Author: Yoonsoo Kwon
Copyright information:
    "All content © 2019 DigiPen (USA) Corporation, all rights reserved."
*/

#include "CommonLevel.h"
#include "Game.h"
#include "yoonsooLV1.h"
#include "ScoreMenu.h"

const SDL_Color WHITE = { 255, 255, 255, 255 };
const SDL_Color BLACK = { 0, 0, 0, 255 };
const SDL_Color MINT = { 135, 206, 175, 255 };
void ScoreMenu::initRank()
{
    FILE *file = fopen("Rank.txt", "r+");

    // If there is no .txt file, then make new .txt file. 
    if (file == NULL)
    {
        char* temp_1 = new char;
        char* temp_2 = new char;
        char* temp_3 = new char;
        char* temp_4 = new char;
        char* temp_5 = new char;

        FILE *file3 = fopen("Rank.txt", "w+");
        fscanf(file3, "%s %d %s %d %s %d %s %d %s %d", 
            temp_1, &players[0].second,
            temp_2, &players[1].second,
            temp_3, &players[2].second,
            temp_4, &players[3].second,
            temp_5, &players[4].second);
        fclose(file3);

        delete temp_1;
        delete temp_2;
        delete temp_3;
        delete temp_4;
        delete temp_5;
    }
    else
    {
        char* temp_1 = new char;
        char* temp_2 = new char;
        char* temp_3 = new char;
        char* temp_4 = new char;
        char* temp_5 = new char;

        fscanf(file, "%s %d %s %d %s %d %s %d %s %d", 
            temp_1, &players[0].second,
            temp_2, &players[1].second,
            temp_3, &players[2].second,
            temp_4, &players[3].second,
            temp_5, &players[4].second);
        fclose(file);

        players[0].first = temp_1;
        players[1].first = temp_2;
        players[2].first = temp_3;
        players[3].first = temp_4;
        players[4].first = temp_5;

        delete temp_1;
        delete temp_2;
        delete temp_3;
        delete temp_4;
        delete temp_5;

    }
}
void ScoreMenu::Initialize()
{
    initRank();

    //m_backgroundColor = MINT;
    // Set Camera
    // You don't need to do this anymore
    //camera.Initialize(int(State::m_width), int(State::m_height));
    // Set position of the camera
    camera.position.Set(0, 0, 0);

    min = players[0].second;
    // Set main font
    // Load the font file to use as a main font
    // and set the default size of it   
 
    if (min > score)
    {
        BGM.sound.LoadSound("sound/dead_scene.wav");
        BGM.sound.SetVolume(100);
        AddObject(&BGM);
        BGM.sound.Play();
       
        background.sprite.Free();
        background.sprite.LoadImage("texture/dead_scene.png", State::m_renderer);
        background.transform.SetScale(1280.f, 720.f);
        AddObject(&background);

        mainFont = TTF_OpenFont("font/Default.ttf", 30);

        scoreText.transform.position.Set(0.0f, 200.f, 0.f);
        scoreText.transform.SetScale(350.f, 100.f);
        scoreText.text.font = mainFont;
        scoreText.text.isHud = true;
        scoreText.text.color = WHITE;
        scoreText.text.SetText("You got a %d point!!", score);
        AddObject(&scoreText);

        Text.transform.position.Set(0.f, 0.f, 0.f);
        Text.transform.SetScale(800.f, 100.f);
        Text.text.font = mainFont;
        Text.text.isHud = true;
        Text.text.color = WHITE;
        Text.text.SetText("Keep Trying!! You Will Be Able to Be Ranked!!");
        AddObject(&Text);

        key.transform.position.Set(0.f, -200.f, 0.f);
        key.transform.SetScale(600.f, 100.f);
        key.text.font = mainFont;
        key.text.isHud = true;
        key.text.color = WHITE;
        key.text.SetText("<Press Enter to Check Rank>");
        AddObject(&key);
    }
    else
    {
        State::m_game->Change((LV_Input));
    }
    
}
void ScoreMenu::Update(float dt)
{
    if (State::m_input->IsTriggered(SDL_SCANCODE_RETURN))
    {
        State::m_game->Change((LV_DEADSCENE));
    }
    UpdatePhysics(dt);
    Render(dt);
}
void ScoreMenu::Close()
{
    // Wrap up state
    RemoveObject(&scoreText);
    RemoveObject(&Text);
    RemoveObject(&key);
    RemoveObject(&background);
    BGM.sound.Free();
    RemoveObject(&BGM);
    ClearBaseState();
}