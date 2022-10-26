/*
File name: ScoeObject.cpp
Project name: Phone! Pong!
Primary Author: shinil Kang
Copyright information:
    "All content © 2019 DigiPen (USA) Corporation, all rights reserved."
*/

#include "ScoreObject.h"
#include "MyObject.h"
#include "yoonsooLV1.h"

ScoreObject::ScoreObject()
	:isPlayed(false)
{
}

ScoreObject::ScoreObject(ScoreObject* obj)
	: MyObject(obj), isPlayed(false)
{
}


ScoreObject::~ScoreObject()
{
}

void ScoreObject::Initialize()
{

}

void ScoreObject::Update(float /*dt*/)
{
	if (physics.HasBody())
	{
		if (physics.IsColliding())
		{
			isDead = true;
		}
	}
}

bool ScoreObject::getIsPlayed()
{
	return isPlayed;
}

void ScoreObject::setIsPlayed(bool set)
{
	isPlayed = set;
}
