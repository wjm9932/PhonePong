/*
File name: MainMenu.h
Project name: Phone! Pong!
Primary Author: Minho Chae
Copyright information:
    "All content © 2019 DigiPen (USA) Corporation, all rights reserved."
*/

#pragma once
#include "engine\State.h"
#include "engine\Object.h"

class MainMenu : public State
{
	friend class Game;

protected:

	MainMenu() : State() {};
	~MainMenu() override {};

	// Derived initialize function
	void Initialize() override;
	// Derived Update function
	void Update(float dt) override;
	// Derived Close function
	void Close() override;

    enum MenuPostions
    {
        PLAY_POSITION = 0,
        HOW_TO_PLAY_POSITION = -68,
        RANKING_POSITION = -136,
        CREDIT_POSITION = -204,
        KEY_MOVE = 68
    };
private:

    //objects for icons
    Object smartPhone_icon;
    Object select;

    // object for background image
    Object background;
    Object clickSound;
    
    void CreateAndRegisterHudText(Object *textObject, const char *textContents, const char *id);

    // function for button event
    void ButtonEvent();



};

