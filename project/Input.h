/*
File name: Input.h
Project name: Phone! Pong!
Primary Author: Jaemin Woo
Copyright information:
    "All content © 2019 DigiPen (USA) Corporation, all rights reserved."
*/

#pragma once
#include "engine\State.h"
#include "engine\Object.h"
#include "yoonsooLV1.h"

class Input2 : public State
{
    friend class Game;
protected:
    Input2() : State()
    {

    };
    ~Input2() override {};

    void Initialize() override;
    void Update(float dt);
    void Close();
    void setRank();
    void setPosition(const char *textContents);
    void Createtext(Object *textObject, const char *textContents);

private:
    std::pair<std::string, int>temp;

    Object Background;

    Object getRank;
    Object getRank2;

    Object text1;
    Object text2;
    Object text3;
};

