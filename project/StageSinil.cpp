/*
File name: StageSinil.cpp
Project name: Phone! Pong!
Primary Author: shinil Kang
Copyright information:
    "All content © 2019 DigiPen (USA) Corporation, all rights reserved."
*/

#include "StageSinil.h"

const SDL_Color BLACK = { 0, 0, 0, 255 };

void LevelSinil::Initialize()
{
	m_backgroundColor = BLACK;

	camera.position.SetZero();

	houseObject.SetName("House");
	houseObject.transform.position.SetZero();
	houseObject.transform.SetScale(128.f, 128.f);
	houseObject.transform.rotation = 0;

	houseObject.sprite.LoadImage("texture/House.png", State::m_renderer);
	houseObject.sprite.isHud = false;

	AddObject(&houseObject);
}

void LevelSinil::Update(float dt)
{
	Render(dt);
}

void LevelSinil::Close()
{
	ClearBaseState();
}
