/*
File name: Input.cpp
Project name: Phone! Pong!
Primary Author: Jaemin Woo
Copyright information:
    "All content © 2019 DigiPen (USA) Corporation, all rights reserved."
*/

#include "Input.h"
#include "Game.h"

const SDL_Color WHITE = { 255, 255, 255, 255 };

void Input2::setRank()
{
    players[0].second = score;
    for (int i = 1; i < 5; i++)
    {
        if (players[i - 1].second > players[i].second)
        {
            temp = players[i - 1];
            players[i - 1] = players[i];
            players[i] = temp;
        }
    }
    FILE *file2 = fopen("Rank.txt", "w");
    fprintf(file2, "%s %d %s %d %s %d %s %d %s %d", players[0].first.c_str(), players[0].second, players[1].first.c_str(), players[1].second, players[2].first.c_str(), players[2].second, players[3].first.c_str(), players[3].second, players[4].first.c_str(), players[4].second);
    fclose(file2);
}
void Input2::Initialize()
{
    backgroundMusic.LoadMusic("sound/rank.mp3");
    m_pBGM = &backgroundMusic;
    backgroundMusic.Play();
    players[0].first.clear();
    m_backgroundColor = { 0, 0, 255,255 };

    Background.sprite.Free();
    Background.sprite.LoadImage("texture/Fanfare_Background.png", State::m_renderer);
    Background.transform.SetScale(1280.f, 720.f);
    Background.sprite.activeAnimation = true;
    Background.sprite.SetFrame(2);
    Background.sprite.SetSpeed(5.f);
    AddObject(&Background);

    mainFont = TTF_OpenFont("font/Default.ttf", 30);
    getRank.transform.position.Set(0.0f, 240.f, 0.f);
    getRank.transform.SetScale(300.f, 100.f);
    getRank.text.font = mainFont;
    getRank.text.isHud = true;
    getRank.text.color = WHITE;
    getRank.text.SetText("Congratulation!!");
    AddObject(&getRank);

    getRank2.transform.position.Set(0.0f, 140.0f, 0.f);
    getRank2.transform.SetScale(600.f, 100.f);
    getRank2.text.font = mainFont;
    getRank2.text.isHud = true;
    getRank2.text.color = WHITE;
    getRank2.text.SetText("Please Enter 3 Characters of Your ID");
    AddObject(&getRank2);
}
void Input2::Update(float dt)
{
    if (players[0].first.size() != 3)
    {
        if (State::m_input->IsTriggered(SDL_SCANCODE_A))
        {
            players[0].first.append("A");
            setPosition("A");
        }
        else if (State::m_input->IsTriggered(SDL_SCANCODE_B))
        {
            players[0].first.append("B");
            setPosition("B");
        }
        else if (State::m_input->IsTriggered(SDL_SCANCODE_C))
        {
            players[0].first.append("C");
            setPosition("C");
        }
        else if (State::m_input->IsTriggered(SDL_SCANCODE_D))
        {
            players[0].first.append("D");
            setPosition("D");
        }
        else if (State::m_input->IsTriggered(SDL_SCANCODE_E))
        {
            players[0].first.append("E");
            setPosition("E");
        }
        else if (State::m_input->IsTriggered(SDL_SCANCODE_G))
        {
            players[0].first.append("G");
            setPosition("G");
        }
        else if (State::m_input->IsTriggered(SDL_SCANCODE_H))
        {
            players[0].first.append("H");
            setPosition("H");
        }
        else if (State::m_input->IsTriggered(SDL_SCANCODE_I))
        {
            players[0].first.append("I");
            setPosition("I");
        }
        else if (State::m_input->IsTriggered(SDL_SCANCODE_J))
        {
            players[0].first.append("J");
            setPosition("J");
        }
        else if (State::m_input->IsTriggered(SDL_SCANCODE_K))
        {
            players[0].first.append("K");
            setPosition("K");
        }
        else if (State::m_input->IsTriggered(SDL_SCANCODE_L))
        {
            players[0].first.append("L");
            setPosition("L");
        }
        else if (State::m_input->IsTriggered(SDL_SCANCODE_M))
        {
            players[0].first.append("M");
            setPosition("M");
        }
        else if (State::m_input->IsTriggered(SDL_SCANCODE_N))
        {
            players[0].first.append("N");
            setPosition("N");
        }
        else if (State::m_input->IsTriggered(SDL_SCANCODE_O))
        {
            players[0].first.append("O");
            setPosition("O");
        }
        else if (State::m_input->IsTriggered(SDL_SCANCODE_P))
        {
            players[0].first.append("P");
            setPosition("P");
        }
        else if (State::m_input->IsTriggered(SDL_SCANCODE_Q))
        {
            players[0].first.append("Q");
            setPosition("Q");
        }
        else if (State::m_input->IsTriggered(SDL_SCANCODE_R))
        {
            players[0].first.append("R");
            setPosition("R");
        }
        else if (State::m_input->IsTriggered(SDL_SCANCODE_S))
        {
            players[0].first.append("S");
            setPosition("S");
        }
        else if (State::m_input->IsTriggered(SDL_SCANCODE_T))
        {
            players[0].first.append("T");
            setPosition("T");
        }
        else if (State::m_input->IsTriggered(SDL_SCANCODE_U))
        {
            players[0].first.append("U");
            setPosition("U");
        }
        else if (State::m_input->IsTriggered(SDL_SCANCODE_W))
        {
            players[0].first.append("W");
            setPosition("W");
        }
        else if (State::m_input->IsTriggered(SDL_SCANCODE_X))
        {
            players[0].first.append("X");
            setPosition("X");
        }
        else if (State::m_input->IsTriggered(SDL_SCANCODE_Y))
        {
            players[0].first.append("Y");
            setPosition("Y");
        }
        else if (State::m_input->IsTriggered(SDL_SCANCODE_Z))
        {
            players[0].first.append("Z");
            setPosition("Z");
        }
    }
    if (State::m_input->IsTriggered(SDL_SCANCODE_BACKSPACE))
    {
        if (!(players[0].first.empty()))
        {
            players[0].first.pop_back();
            if (players[0].first.size() == 2)
            {
                text3.text.Free();
            }
            else if (players[0].first.size() == 1)
            {
                text2.text.Free();
            }
            else if (players[0].first.size() == 0)
            {
                text1.text.Free();
            }
        }
    }
    if (State::m_input->IsTriggered(SDL_SCANCODE_RETURN) && players[0].first.size() == 3)
    {
        State::m_game->Change((LV_DEADSCENE));
    }
    Render(dt);
}
void Input2::Close()
{
    if (players[0].first.size() <= 0)
    {
        players[0].first.clear();
        players[0].first.append("-");
    }
    setRank();
    backgroundMusic.Free();
    ClearBaseState();
}

void  Input2::Createtext(Object *textObject, const char *textContents)
{
    // Set the object's name

    if (textObject == &text1)
    {
        textObject->transform.position.Set(-150.0f, -50.0f, 0.0f);
    }
    else if (textObject == &text2)
    {
        textObject->transform.position.Set(0.0f, -50.0f, 0.0f);
    }
    else if (textObject == &text3)
    {
        textObject->transform.position.Set(150.0f, -50.0f, 0.0f);
    }

    // Set the scale of the object
    textObject->transform.SetScale(50, 50);
    textObject->text.font = mainFont;
    // Set the text to render out
    textObject->text.SetText(textContents);

    // Set the colors r,g,b,a (0~255)
    textObject->text.color = WHITE;

    // Set either if the object to be hud or not
    textObject->text.isHud = true;

    // Register the object to the state
    AddObject(textObject);
}

void Input2::setPosition(const char *textContents)
{
    if (players[0].first.size() == 1)
    {
        Createtext(&text1, textContents);
    }
    else if (players[0].first.size() == 2)
    {
        Createtext(&text2, textContents);
    }
    else if (players[0].first.size() == 3)
    {
        Createtext(&text3, textContents);
    }
}