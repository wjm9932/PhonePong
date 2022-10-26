/*
File name: Menu_Ranking.h
Project name: Phone! Pong!
Primary Author: Jaemin Woo
Secondary Author: Minho Chae
Copyright information:
    "All content © 2019 DigiPen (USA) Corporation, all rights reserved."
*/

#pragma once
#pragma once
#include "engine\State.h"
#include "engine\Object.h"

class MainMenu;

class MenuRanking : public State
{
    friend class Game;
protected:
    MenuRanking() : State()
    {

    };
    ~MenuRanking() override {};

    void Initialize() override;
    void Update(float dt) override;
    void CreateAndRegisterHudText(Object *textObject, const char *textContents, const char *id);
    void Close() override;
private:

    int min;
    int temp;

    Object Background;

    Object Choice;
    Object Key;
    Object rankText1st;
    Object rankText1st_1;
    Object rankText1st_2;
    Object rankText2nd;
    Object rankText2nd_1;
    Object rankText2nd_2;
    Object rankText3rd;
    Object rankText3rd_1;
    Object rankText3rd_2;
    Object rankText4th;
    Object rankText4th_1;
    Object rankText4th_2;
    Object rankText5th;
    Object rankText5th_1;
    Object rankText5th_2;
};