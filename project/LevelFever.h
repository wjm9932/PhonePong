/*
File name: LevelFever.h
Project name:
Primary Author: Shinil kang
Secondary Author: Yoonsoo Kwon
Copyright information:
    "All content © 2019 DigiPen (USA) Corporation, all rights reserved."
*/

#pragma once
#include <engine/State.h>
#include "MyObject.h"
#include "MapGenerator.h"

class LevelFever :
	public State
{
public:
	LevelFever();
	virtual ~LevelFever();

	void Initialize() override;
	void Update(float dt) override;
	void Close() override;

	void ControlCamera(float dt);

	void InitializeMapGenerator(void);
	void UpdateMapGenerator(float dt);

	void MapGeneratorAddObject(void);
	void MapGeneratorRemoveObject(void);
	void RemoveFirstMapFromLevel(void);

	// Score Functions
	void Score(void);
	void InitializeScoreUI(void);

	// Level Information Functions
	void LoadLevelInfo(void);
	void SaveLevelInfo(void);

    //function for fever effect
    void FeverEffectObject();

private:
	b2Vec2 playerVelocity{ 25.f, 0.f };
	MyObject animation;
	float charged_dt = 0;

	float Health = 0;

	// object for showing score 
    const int score_up = 300;
	int score = 0;
	Object scoreText;

	// Map Generator
	MapGenerator mg;

    Object background;

	//Fever time
	double timer = 0;
	Object FeverTimeText;
        Object coinSoundEffect;

    //variable for transparency
    const Uint8 alpha_number = 10;

    //vector for fever effect
    std::vector<MyObject*> Fever_Effect;
	int currLevel;
	int checkLoadFeverTime;

    //It's for camera moving
    float32 cameraPos_Y = 0;
    float32 cameraPos_Z = 0;
    float32 rangeOf_Y = 30;
    bool isUp = true;
    bool isDown = false;

    b2Vec2 gravity = { 0.f, -1050.f };

    Object timerTEXT;
    float feverTimer = 15;

    const float FALL_POSITION = -300.f;
    const float FEVER_TEXT_REMOVE_POSITION = 500.f;
};

