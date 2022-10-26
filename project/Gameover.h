/*
File name: Gameover.h
Project name: Phone! Pong!
Primary Author: Jaemin Woo
Secondary Author: Gyuyoung Park
Copyright information:
    "All content © 2019 DigiPen (USA) Corporation, all rights reserved."
*/

#pragma once
#include "engine\State.h"
#include "engine\Object.h"
#include "yoonsooLV1.h"
#include "List.h"

class Level1;

class Gameover : public State
{
	friend class Game;
protected:
	Gameover() : State()
	{

	};
	~Gameover() override {};

	void Initialize() override;
	void Update(float);
	void CreateAndRegisterHudText(Object *textObject, const char *textContents, const char *id);
	void Close();
private:
	Object Choice;
	Object Key;

    Object Background;
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