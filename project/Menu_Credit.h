/*
File name: Menu_Credit.h
Project name: Phone! Pong!
Primary Author: Minho Chae
Copyright information:
    "All content © 2019 DigiPen (USA) Corporation, all rights reserved."
*/

#pragma once
#include "engine\State.h"
#include "engine\Object.h"

class MainMenu;

class MenuCredit : public State
{
    friend class Game;
protected:
    MenuCredit() : State()
    {

    };
    ~MenuCredit() override {};

    void Initialize() override;
    void Update(float dt) override;
    void CreateAndRegisterHudText(Object *textObject, const char *textContents, const char *id);
    void Close() override;
private:

    //objects for texts
    Object President;
    Object Instructor;
    Object TeachingAssistant;
    Object Shinil;
    Object Yoonsoo;
    Object Jaemin;
    Object Minho;
    Object Gyuyoung;
    Object Sound;
    Object Sound_Copyright;
    Object Sound_Copyright_Jump_1;
    Object Sound_Copyright_Jump_2;
    Object Sound_Copyright_Rank_1;
    Object Sound_Copyright_Rank_2;
    Object Key;

    //object for background
    Object Background;
};