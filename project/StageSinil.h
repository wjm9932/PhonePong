/*
File name: StageSinil.h
Project name: Phone! Pong!
Primary Author: shinil Kang
Copyright information:
    "All content © 2019 DigiPen (USA) Corporation, all rights reserved."
*/

#pragma once

#include "engine/State.h"
#include "engine/Object.h"

class LevelSinil: public State
{
	friend class Game;

protected:
	LevelSinil() : State() {};
	~LevelSinil() override {};

	void Initialize() override;
	void Update(float dt) override;
	void Close() override;

private:
	Object houseObject;
};