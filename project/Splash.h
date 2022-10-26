/*
File name: Splash.h
Project name: Phone! Pong!
Primary Author: Minho Chae
Copyright information:
    "All content © 2019 DigiPen (USA) Corporation, all rights reserved."
*/


#pragma once
#include "engine\State.h"
#include "engine\Object.h"

class MainMenu;

class Splash : public State
{
    friend class Game;
protected:
    Splash() : State()
    {

    };
    ~Splash() override {};

    void Initialize() override;
    void Update(float dt) override;
    void CreateAndRegisterHudText(Object *textObject, const char *textContents, const char *id);
    void Close() override;
private:

    //objects for animations
    Object Digipen;

    float splash_count = 2;
};