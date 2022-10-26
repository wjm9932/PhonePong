/*
File name: ScoreMenu.cpp
Project name: Phone! Pong!
Primary Author: Jaemin Woo
Copyright information:
    "All content © 2019 DigiPen (USA) Corporation, all rights reserved."
*/

#pragma once
#include "engine\State.h"
#include "engine\Object.h"

using Rank = std::pair<std::string, int>;

class Level1;

extern Rank players[5];
extern int min;

class ScoreMenu : public State
{
    friend class Game;
protected:
    ScoreMenu() : State()
    {

    };
    ~ScoreMenu() override {};

    void initRank();

    void Initialize() override;
    void Update(float dt);
    //void CreateAndRegisterHudText(Object *textObject, const char *textContents, const char *id);
    void Close();

private:
    int min;
    Object key;
    Object Text;
    Object scoreText, background;
    Object BGM;
};