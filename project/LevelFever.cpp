/*
File name: LevelFever.cpp
Project name:
Primary Author: Shinil kang
Secondary Author: Yoonsoo Kwon
Copyright information:
    "All content © 2019 DigiPen (USA) Corporation, all rights reserved."
*/

#include "LevelFever.h"
#include <stdio.h>
#include "yoonsooLV1.h"
#include "PRNG.h"
#include "Game.h"

const SDL_Color MINT = { 0, 255, 255, 255 };
const SDL_Color BLACK = { 0, 0, 0, 255 };
const SDL_Color RED = { 255,0,0,255 };
extern  bool nextStage;
LevelFever::LevelFever()
{
}


LevelFever::~LevelFever()
{
}

void LevelFever::Initialize()
{
    nextStage = false;
	m_backgroundColor = MINT;

    background.sprite.Free();
    background.sprite.LoadImage("texture/Fever_Time_Background.png", State::m_renderer);
    background.transform.SetScale(1280.f, 720.f);
    AddObject(&background);

    m_backgroundColor = MINT;
    timer = 0;
    // Set position of the camera
    camera.position.Set(0, 0, 0);

    SetPhysicsWorld(gravity);
    // Set main font
    // Load the font file to use as a main font
    // and set the default size of it
    mainFont = TTF_OpenFont("font/Default.ttf", 48);

    animation.SetName("Walking boy");
    animation.transform.position.Set(0.f, 0, 0.f);
    animation.transform.SetScale(80, 110);

    //animation.transform.rotation = 45.F;
    animation.sprite.Free();
    animation.sprite.LoadImage("texture/Fever_Pong.png", State::m_renderer);
    // Set toggle true so animation activates 
    animation.sprite.activeAnimation = false;
    // Set the number of frames to update
    animation.sprite.SetFrame(4);
    // Set the speed of frame updating
    animation.sprite.SetSpeed(20.f);
    // Generate rigid-body passing world and transform
    animation.physics.GenerateBody(GetPhysicsWorld(), &animation.transform);
    animation.physics.ActiveGhostCollision(true);
    animation.physics.SetVelocity(playerVelocity.x, 0);


    timerTEXT.transform.position.Set(200.f, 0.f, 0.f);
    timerTEXT.transform.SetScale(500.f, 150.f);
    timerTEXT.text.font = mainFont;
    timerTEXT.text.isHud = true;
    timerTEXT.text.color = RED;



    backgroundMusic.LoadMusic("sound/Pomade.mp3");
    m_pBGM = &backgroundMusic;
    backgroundMusic.Play();

    coinSoundEffect.sound.LoadSound("sound/coin.wav");
    
    AddObject(&coinSoundEffect);
    AddObject(&FeverTimeText);
    AddObject(&timerTEXT);
    AddObject(&animation);
    LoadLevelInfo();
    InitializeScoreUI();
    InitializeMapGenerator();
    charged_dt = 0;
    Health = 200;
}

void LevelFever::Update(float dt)
{
    ControlCamera(dt);
    Score();
    UpdateMapGenerator(dt);
    //It can prevent rope generate during the rope already connected to JointBox 
    if (m_input->IsPressed(SDL_SCANCODE_SPACE))
    {
        animation.physics.SetVelocity(b2Vec2{ animation.physics.GetVelocity().x , 100 });//(animation.physics.GetVelocity().y + 35 * 3 / charged_dt) > 2 ? animation.physics.GetVelocity().y + 35 * 3 / charged_dt : 2});
    }

    timerTEXT.text.SetText("Time : %0.2f", feverTimer);
    if (feverTimer <= 0.0f) {
        State::m_game->Change(LV_LevelYoonsoo);
    }
    
    if (animation.transform.position.y < FALL_POSITION) {

        State::m_game->Change(LV_LevelYoonsoo);
    }
    if (animation.transform.position.x > FEVER_TEXT_REMOVE_POSITION)
    {
        FeverTimeText.text.Free();
    }

    //function for fever effect
    FeverEffectObject();
    UpdatePhysics(dt);

    Render(dt);
}

void LevelFever::Close()
{
    // Wrap up MapGenerator
    while (mg.GameMapSize() != 0)
    {
        RemoveFirstMapFromLevel();
        mg.RemoveGameMap();
    }
    mg.RemoveMap();

    SaveLevelInfo();
    backgroundMusic.Free();
    coinSoundEffect.sound.Free();
    ClearBaseState();
}


void LevelFever::ControlCamera(float dt)
{
    // Set the camera's speed
    static bool toggle = true;
    if (isUp == true) {
        cameraPos_Y += 8;
        cameraPos_Z -= 2;
        if (cameraPos_Y > rangeOf_Y) {
            isDown = true;
            isUp = false;
        }
    }
    else if (isDown == true) {
        cameraPos_Y -=8;
        if (cameraPos_Z < 0) {
            cameraPos_Z += 2;
        }
        if (cameraPos_Y < -rangeOf_Y) {
            isUp = true;
            isDown = false;
        }
    }
    camera.position = b2Vec3(animation.transform.position.x + 350, cameraPos_Y, cameraPos_Z);
    //Put the camera to Player and make gravity by setting the velocity
    

    animation.physics.SetVelocity(b2Vec2{ playerVelocity });

    background.transform.position = camera.position;

    feverTimer -=dt;
	charged_dt += dt;
}

void LevelFever::InitializeMapGenerator()
{
    Map* m1 = new Map;
    // Constructing ScoreObject
    ScoreObject* scoreObject = new ScoreObject;
    scoreObject->SetPhysicsWorld(GetPhysicsWorld());
    scoreObject->sprite.color.r = scoreObject->sprite.color.g = scoreObject->sprite.color.b = 0xff;
    scoreObject->SetScale(b2Vec2{ 100.f, 100.f });
    scoreObject->SetSource("texture/Fever_Score_Object.png");
    scoreObject->SetName("scoreObject");
    scoreObject->transform.position.Set(10.f, 0.f, 0.f);
    scoreObject->physics.bodyType = Physics::STATIC;
    scoreObject->physics.GenerateBody(GetPhysicsWorld(), &scoreObject->transform);
    scoreObject->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(scoreObject);

    // X-Position of Signal should equal with the last score object in map!!!
    MyObject* signal = new MyObject;
    signal->SetSource("texture/rect.png");
    signal->SetScale(b2Vec2{ 1.f, 1.f });
    signal->SetName("Signal");
    signal->transform.position.Set(10.f, -1000.f, 0.f);
    m1->AddObject(signal);

    mg.AddMap(m1);

    mg.AddMapToGame(0);
    for (int i = 0; i < mg.GetBackFromGame()->GetScoreObjectSize(); i++)
    {
        // Time to add ScoreObject
        ScoreObject* initialScoreObject = mg.GetBackFromGame()->GetScoreObject(i);
        // 400.f is arbitary number
        initialScoreObject->transform.position.x -= 40.f;
        initialScoreObject->physics.GenerateBody(GetPhysicsWorld(), &initialScoreObject->transform);
        initialScoreObject->physics.ActiveGhostCollision(initialScoreObject->physics.IsActiveGhost());
        AddObject(initialScoreObject);
    }

    mg.GetBackFromGame()->Calculate();
}

void LevelFever::UpdateMapGenerator(float dt)
{

    // map list node go left side from character's position
    if (camera.position.x - 700.f > mg.GetMapFromGame(0)->getEndPosition())
    {
        MapGeneratorRemoveObject();
    }

    // Calculating sum of all object's x scale
    float xSum = 0.f;
    for (int mapCount = 0; mapCount < mg.GameMapSize(); ++mapCount)
    {
        for (int objCount = 0; objCount < mg.GetMapFromGame(mapCount)->GetScoreObjectSize(); ++objCount)
        {
            xSum += mg.GetMapFromGame(mapCount)->GetScoreObject(objCount)->GetScale().x;
        }
    }

    // Updating ScoreObjects
    for (int objectCount = 0; objectCount < mg.GetMapFromGame(0)->GetScoreObjectSize(); objectCount++)
    {
        MyObject* obj = mg.GetMapFromGame(0)->GetScoreObject(objectCount);
        obj->Update(dt);
        if (obj->isDead)
        {
            coinSoundEffect.sound.Play();
            coinSoundEffect.sound.SetVolume(256);
            score += score_up;
            RemoveObject(obj);
            mg.GetMapFromGame(0)->EraseScoreObject(objectCount);
        }
    }

    // Add object when x of all presented objects is equal or less than arbitary value
    if (xSum <= 200.f)
    {
        // make comment should not be processed to check init objects
        MapGeneratorAddObject();
    }
}

void LevelFever::MapGeneratorAddObject()
{
    // Random is crawling by Kevin's code
    mg.AddMapToGame(Random(0, mg.StorageSize() - 1));
    for (int i = 0; i < mg.GetBackFromGame()->GetObjectSize(); i++)
    {
        MyObject* object = mg.GetBackFromGame()->GetObject(i);
        object->transform.position.x += mg.GetMapFromGame(mg.GameMapSize() - 2)->getEndPosition() + 500.f;
    }
    for (int i = 0; i < mg.GetBackFromGame()->GetScoreObjectSize(); i++)
    {
        ScoreObject* scoreObject = mg.GetBackFromGame()->GetScoreObject(i);
        scoreObject->transform.position.x += mg.GetMapFromGame(mg.GameMapSize() - 2)->getEndPosition() + 500.f;
        scoreObject->physics.GenerateBody(GetPhysicsWorld(), &scoreObject->transform);
        scoreObject->sprite.Free();
        scoreObject->sprite.LoadImage(scoreObject->GetSource().c_str(), State::m_renderer);
        scoreObject->physics.ActiveGhostCollision(scoreObject->physics.IsActiveGhost());
        AddObject(scoreObject);
    }
    mg.GetBackFromGame()->Calculate();
}

void LevelFever::MapGeneratorRemoveObject()
{
    // Remove first map in gameMap list
    RemoveFirstMapFromLevel();
    mg.RemoveFirstGameMap();
}

void LevelFever::RemoveFirstMapFromLevel()
{
    for (int i = 0; i < mg.GetMapFromGame(0)->GetObjectSize(); i++)
    {
        RemoveObject(mg.GetMapFromGame(0)->GetObject(i));
    }
    for (int i = 0; i < mg.GetMapFromGame(0)->GetScoreObjectSize(); i++)
    {
        RemoveObject(mg.GetMapFromGame(0)->GetScoreObject(i));
    }
}



void LevelFever::Score()
{
    score++;
    scoreText.text.SetText("Score: %d", score);
}


//function for initializing score UI
void LevelFever::InitializeScoreUI()
{
    scoreText.transform.position.Set(500.f, 260.f, 0.f);
    scoreText.transform.SetScale(200.f, 40.f);
    scoreText.text.font = mainFont;
    scoreText.text.isHud = true;
    scoreText.text.color = BLACK;
    AddObject(&scoreText);
    feverTimer = 15;
}


// Is it better to change into object?
void LevelFever::LoadLevelInfo()
{
    FILE* levelInfo = fopen("LevelInfo.txt", "r+");

    // If there is no text file, make new text file.
    if (levelInfo == nullptr)
    {
        return;
    }

	fscanf(levelInfo, "%f %d %d %f", &Health, &score, &currLevel, reinterpret_cast<float*>(&checkLoadFeverTime));
	fclose(levelInfo);
}

void LevelFever::SaveLevelInfo()
{
    FILE* levelInfo = fopen("LevelInfo.txt", "w");

    if (levelInfo != nullptr)
    {
		fprintf(levelInfo, "%f %d %d %f", Health, score, currLevel, static_cast<float>(checkLoadFeverTime));
		fclose(levelInfo);
    }
}

void LevelFever::FeverEffectObject()
{
    MyObject* FeverEffect = new MyObject;
    FeverEffect->SetName("Fever Effect");
    FeverEffect->transform.position.Set(animation.transform.position.x - 25, animation.transform.position.y, 0.f);
    FeverEffect->transform.SetScale(10, 100);
    FeverEffect->sprite.Free();
    FeverEffect->sprite.LoadImage("texture/Fever_Effect.png", State::m_renderer);

    //FeverEffect->sprite.color.a -= alpha_number;

    for (int i = 0; i < Fever_Effect.size(); i++)
    {

        Fever_Effect.at(i)->sprite.color.a -= alpha_number;
        if (Fever_Effect.at(i)->sprite.color.a <= 10)
        {
            //const auto & tmp = Fever_Effect.at(i);
            RemoveObject(Fever_Effect.at(i));
            //Fever_Effect.erase(Fever_Effect.begin());
            //delete tmp;
        }
    }

    Fever_Effect.push_back(FeverEffect);
    AddObject(FeverEffect);
}