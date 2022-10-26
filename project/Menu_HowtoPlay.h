/*
File name: Menu_HowtoPlay.h
Project name: Phone! Pong!
Primary Author: Minho Chae
Copyright information:
    "All content © 2019 DigiPen (USA) Corporation, all rights reserved."
*/

#pragma once
#include "engine\State.h"
#include "engine\Object.h"

class MainMenu;

class MenuHowToPlay : public State
{
    friend class Game;
protected:
    MenuHowToPlay() : State()
    {

    };
    ~MenuHowToPlay() override {};

    void Initialize() override;
    void Update(float dt) override;
    void CreateAndRegisterHudText(Object *textObject, const char *textContents, const char *id);
    void Close() override;
private:

    //objects for animations
    Object JumpAnimation;
    Object RopeAnimation;
    Object CtrlAnimation;
    Object AltAnimation;

    //object for texts
    Object Key;

    Object JumpText;
    Object RopeText;
    Object FeverTimeText;

    Object JumpExplainText;
    Object RopeExplainText;
    Object FeverTimeExplainText_1;
    Object FeverTimeExplainText_2;
    Object FeverTimeExplainText_3;
};