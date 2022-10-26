/*
File name: ScoeObject.h
Project name: Phone! Pong!
Primary Author: shinil Kang
Copyright information:
    "All content © 2019 DigiPen (USA) Corporation, all rights reserved."
*/

#pragma once
#include "MyObject.h"
class ScoreObject :
	public MyObject
{
public:
	ScoreObject();
	ScoreObject(ScoreObject* obj);
	virtual ~ScoreObject();

	virtual void Initialize();
	virtual void Update(float dt);

	bool getIsPlayed();
	void setIsPlayed(bool set);

private:
	bool isPlayed = false;
};