/*
 File name: yoonsooLV1.cpp
 Project name: Phone! Pong!
 Primary Author: Yoonsoo Kwon, Shinil Kang
 Secondary Author: Jaemin Woo, Minho Chae
 Copyright information:
 "All content © 2019 DigiPen (USA) Corporation, all rights reserved."

     */
#include "CommonLevel.h"
#include "yoonsooLV1.h"
#include "MyObject.h"
#include "PRNG.h"
#include "ScoreObject.h"
#include <corecrt_wstdio.h>
#include <stdio.h>

const  SDL_Color SKY_BLUE = { 64, 64, 255, 255 };
const SDL_Color WHITE = { 255, 255, 255, 0 };
const SDL_Color BLACK = { 0, 0, 0, 255 };
const SDL_Color RED = { 255, 0, 0, 255 };
const SDL_Color BROWN = { 153, 102, 051, 255 };
const SDL_Color GREY = { 84, 84, 84, 150 };
const SDL_Color CITY_GREY = { 50, 50, 50, 255 };
const SDL_Color HIGHWAY_COLOr = { 234, 171, 86, 255 };
const SDL_Color FACTORY_COLOr = { 69, 69, 69, 255 };

float currentPlayerLoc;

extern  bool nextStage = false;
extern int score = 0;

b2Vec2 playerVelocity(25, 0.f);

void Level1::Initialize()
{
    m_backgroundColor = FACTORY_COLOr;
    background.sprite.Free();
    background.sprite.LoadImage("texture/Factory_Background.png", State::m_renderer);
    background.transform.position = b2Vec3{ 0.f , 0.f, 0.f };
    background.transform.SetScale(2000.f, 724.f);
    background.sprite.activeAnimation = true;
    background.sprite.SetFrame(4);
    background.sprite.SetSpeed(6.f);
    AddObject(&background);

    // Set position of the camera
    camera.position.Set(0, 0, 0);

    Filter.SetName("Filter");
    Filter.transform.position.Set(0.f, 0.f, 0.f);
    Filter.transform.SetScale(b2Vec2{ 1920.f, 1080.f });
    Filter.sprite.color = WHITE;
    Filter.sprite.Free();
    Filter.sprite.LoadImage("texture/rect.png", State::m_renderer);
    Filter.sprite.isHud = true;
    AddObject(&Filter);

    warningFilter.SetName("WarningFilter");
    warningFilter.transform.position.Set(0.f, 0.f, 0.f);
    warningFilter.transform.SetScale(b2Vec2{ 1344.f, 756.f });
    warningFilter.sprite.Free();
    warningFilter.sprite.LoadImage("texture/FilterDead.png", State::m_renderer);
    warningFilter.sprite.color = WHITE;
    warningFilter.sprite.color.a = 0;
    warningFilter.sprite.isHud = true;
    AddObject(&warningFilter);

    backgroundMusic.LoadMusic("sound/Bouncey.mp3");
    m_pBGM = &backgroundMusic;
    backgroundMusic.Play();

    // Set main font
    mainFont = TTF_OpenFont("font/Default.ttf", 48);

    // function for Initializing
    InitializeVariables();

    b2Vec2 gravity(0.f, -235.f);
    SetPhysicsWorld(gravity);

    animation.SetName("smartphone");
    animation.transform.position.Set(-400.f, -100.f, 0.f);
    animation.transform.SetScale(80, 110);

    animation.sprite.Free();
    animation.sprite.LoadImage("texture/Pong.png", State::m_renderer);
    // Set toggle true so animation activates 
    animation.sprite.activeAnimation = false;

    // Generate rigid-body passing world and transform
    animation.physics.ActiveGhostCollision(true);
    animation.physics.Free();
    animation.physics.GenerateBody(GetPhysicsWorld(), &animation.transform);
    animation.physics.SetVelocity(playerVelocity.x, 0);

    preparePlatform.SetName("preparePlatform");
    preparePlatform.transform.position.Set(0.f, -400.f, 0.f);
    preparePlatform.transform.SetScale(b2Vec2{ 1200.f, 320.f });
    preparePlatform.sprite.Free();
    preparePlatform.sprite.LoadImage("texture/prepare_platform.png", State::m_renderer);
    preparePlatform.sprite.color.a = 255;
    preparePlatform.physics.bodyType = Physics::STATIC;
    AddObject(&preparePlatform);

    animationSensor.SetName("Sensor");
    animationSensor.transform.SetScale(50, 5);
    animation.physics.SetVelocity(playerVelocity.x, 0);

    animationSensor.sprite.Free();

    // Generate rigidbody passing world and transform
    animationSensor.physics.Free();
    animationSensor.physics.GenerateBody(GetPhysicsWorld(), &animationSensor.transform);
    animationSensor.physics.ActiveGhostCollision(true);

    animation.physics.SetVelocity(playerVelocity.x, 0);

    warningSoundEffect.sound.LoadSound("sound/Spaceship_Alarm.wav");
    jumpSoundEffect.sound.LoadSound("sound/jumping.wav");
    ropeSoundEffect.sound.LoadSound("sound/burst fire.wav");
    fallingSoundEffect.sound.LoadSound("sound/GameOver.wav");
    phoneOffSoundEffect.sound.LoadSound("sound/MachinePowerOff.wav");
    phoneOffSoundEffect.sound.SetVolume(128);
    coinSoundEffect.sound.LoadSound("sound/coin.wav");
    coinSoundEffect.sound.SetVolume(100);



    AddObject(&animation);
    AddObject(&animationSensor);

    AddObject(&coinSoundEffect);
    AddObject(&warningSoundEffect);
    AddObject(&ropeSoundEffect);
    AddObject(&jumpSoundEffect);
    AddObject(&fallingSoundEffect);
    AddObject(&phoneOffSoundEffect);

    LoadLevelInfo();

    InitializeMapGenerator();
}

void Level1::UpdateWarningFilter()
{
    if (isWarningFilterOn)
    {
        if (isBrighter)
        {
            warningFilter.sprite.color.a += 5;
            if (warningFilter.sprite.color.a > 230)
            {
                isBrighter = !isBrighter;
            }
        }
        else
        {
            warningFilter.sprite.color.a -= 5;
            if (warningFilter.sprite.color.a < 15)
            {
                isBrighter = !isBrighter;
            }
        }
    }
    else
    {
        warningFilter.sprite.color.a = 0;
    }
}

void Level1::Update(float dt)
{
    setMusic();
    if (nextStage == false)
    {
        TransformObjects(dt);
    }
    // for checking the time
    timer += dt;
    timer2 += dt;
    charged_dt += dt;
    // By adding the collide check condition
        // It can prevent rope generate during the rope already connected to JointBox 
    if (ropeCheck == true)
    {
        if (m_input->IsTriggered(SDL_SCANCODE_RETURN) &&
            rope_joint_def.collideConnected == false &&
            mg.GetMapFromGame(0)->getIsRopeAvailable())
        {
            makeNewRope();
        }
    }
    if (m_input->IsTriggered(SDL_SCANCODE_T)) {
        fever_object_count++;
        std::cout << fever_object_count << std::endl;
    }


    // To remove rope and image update
    RemoveRope_RopeTransformUpdate(JointBox);
    SpeedUp();
    UpdateMapGenerator(dt);

    if (State::m_input->IsTriggered(SDL_SCANCODE_SPACE) && canJump && !nextStage)
    {
        if (animation.physics.GetVelocity().y > -0.2 && animation.physics.GetVelocity().y < 0.2)
        {
            Jumping(animation);
            jumpSoundEffect.sound.Play();
            UpdatePhysics(dt);
            //Render(dt);
        }
    }

    // Update Warning Filter
    UpdateWarningFilter();

    //function for checking collision and execute event
    FeverObjectEvent();

    UpdatePhysics(dt);

    ControlCamera(dt);

    Render(dt);

    // if player fall, set HP 0 
    if (animationSensor.transform.position.y < PONG_FALL_RANGE)
    {
        fallCheck = true;
        if (deadCheck == false && fallCheck == true)
        {
            fallingSoundEffect.sound.Play();
            animation.sprite.Free();
            animation.sprite.LoadImage("texture/Fall_Pong.png", State::m_renderer);
            animation.transform.SetScale(80.f, 140.f);
            animation.sprite.activeAnimation = true;
            animation.sprite.SetFrame(4);
            animation.sprite.SetSpeed(8.f);
            AddObject(&animation);

            animation.physics.ActiveGhostCollision(true);
            animation.physics.SetVelocity(b2Vec2{ 0, 100 });
            playerVelocity = b2Vec2{ 0.f, 0.f };
            deadCheck = true;
        }
        Health = 0;
    }
    // if player's HP become 0, change state to dead scene
    if (Health <= 0)
    {
        deadCount -= dt;
        ropeCheck = false;
        jumpCheck = false;
        backgroundMusic.Free();

        if (deadCheck == false && fallCheck == false)
        {
            phoneOffSoundEffect.sound.Play();

            animation.sprite.Free();
            animation.sprite.LoadImage("texture/Dead_Pong.png", State::m_renderer);
            animation.transform.SetScale(80.f, 110.f);
            animation.sprite.activeAnimation = true;
            animation.sprite.SetFrame(10);
            animation.sprite.SetSpeed(5.f);
            AddObject(&animation);

            deadCheck = true;
        }

        playerVelocity = b2Vec2{ 0.f, 0.f };
        if (deadCount <= 0)
        {
            m_game->Change(LV_ScoreMenu);
        }
    }

    if (animation.transform.position.y <= PONG_LANDING_RANGE_UP && animation.transform.position.y >= PONG_LANDING_RANGE_DOWN)
    {
        if (Health > 0)
        {
            animation.sprite.Free();
            animation.sprite.LoadImage("texture/Pong.png", m_renderer);
        }
    }

    if (m_input->IsPressed(SDL_SCANCODE_0))
    {
        timer2 = 75;
    }
    if (Health > 0)
    {
        Score();
    }
}

void Level1::Close()
{
    // Make rope Free
    if (rope != nullptr)
    {
        // Rope Clean Up?
        RemoveObject(rope);
        delete rope;
        rope = nullptr;
        JointBox = nullptr;
        collisionBox = nullptr;
        rope_joint_def.collideConnected = false;
        GetPhysicsWorld()->DestroyJoint(rope_joint);
    }

    mapGeneratorMakeEmpty();

    // Save level data
    if (isGoingFever)
    {
        SaveLevelInfo();
    }
    else
    {
        DeleteLevelInfo();
    }

    warningSoundEffect.sound.Free();
    jumpSoundEffect.sound.Free();
    ropeSoundEffect.sound.Free();
    fallingSoundEffect.sound.Free();
    phoneOffSoundEffect.sound.Free();
    coinSoundEffect.sound.Free();
    RemoveObject(&warningSoundEffect);
    RemoveObject(&jumpSoundEffect);
    RemoveObject(&fallingSoundEffect);
    RemoveObject(&phoneOffSoundEffect);
    RemoveObject(&coinSoundEffect);
    ClearBaseState();
}

void Level1::InitializeMapGenerator(void)
{
    StageChange(currLevel);
}

void Level1::UpdateMapGenerator(float dt)
{
    canJump = true;
    // map list node go left side from character's position
    if (camera.position.x - CAMERA_OUTSIDE_POSITION > mg.GetMapFromGame(0)->getEndPosition())
    {
        canJump = false;
        MapGeneratorAddObject();
        MapGeneratorRemoveObject();
        std::cout << "Map Removed!" << std::endl;
    }
    // Add object when x of all presented objects is equal or less than arbitary value    
    if (camera.position.x + CAMERA_OUTSIDE_POSITION > mg.GetBackFromGame()->getStartPosition())
    {
        canJump = false;
        // make comment should not be processed to check init objects
        MapGeneratorAddObject();
        std::cout << "Map Generated!" << std::endl;
    }
    // Updating ScoreObjects
    for (int objectCount = 0; objectCount < mg.GetMapFromGame(0)->GetScoreObjectSize(); objectCount++)
    {
        ScoreObject* obj = mg.GetMapFromGame(0)->GetScoreObject(objectCount);
        obj->Update(dt);

        // Remove when Collided
        if (obj->isDead)
        {
            if (obj->getIsPlayed() == false)
            {
                coinSoundEffect.sound.Play();
                obj->setIsPlayed(true);
                score += SCORE_UP;
            }

            //checking object name and turn fever object check to true
            if (obj->GetName() == "Object_D")
            {
                alphabet_D.sprite.Free();
                alphabet_D.sprite.LoadImage("texture/UI_D_TRUE.png", State::m_renderer);
                if (!d_get_check)
                {
                    fever_object_count++;
                }

                d_get_check = true;
            }
            if (obj->GetName() == "Object_I_1")
            {
                alphabet_I_1.sprite.Free();
                alphabet_I_1.sprite.LoadImage("texture/UI_I_1_TRUE.png", State::m_renderer);
                if (!i_1_get_check)
                {
                    fever_object_count++;
                }
                i_1_get_check = true;
            }
            if (obj->GetName() == "Object_G")
            {
                alphabet_G.sprite.Free();
                alphabet_G.sprite.LoadImage("texture/UI_G_TRUE.png", State::m_renderer);
                if (!g_get_check)
                {
                    fever_object_count++;
                }
                g_get_check = true;
            }
            if (obj->GetName() == "Object_I_2")
            {
                alphabet_I_2.sprite.Free();
                alphabet_I_2.sprite.LoadImage("texture/UI_I_2_TRUE.png", State::m_renderer);
                if (!i_2_get_check)
                {
                    fever_object_count++;
                }
                i_2_get_check = true;
            }
            if (obj->GetName() == "Object_P")
            {
                alphabet_P.sprite.Free();
                alphabet_P.sprite.LoadImage("texture/UI_P_TRUE.png", State::m_renderer);
                if (!p_get_check)
                {
                    fever_object_count++;
                }
                p_get_check = true;
            }
            if (obj->GetName() == "Object_E")
            {
                alphabet_E.sprite.Free();
                alphabet_E.sprite.LoadImage("texture/UI_E_TRUE.png", State::m_renderer);
                if (!e_get_check)
                {
                    fever_object_count++;
                }
                e_get_check = true;
            }
            if (obj->GetName() == "Object_N")
            {
                alphabet_N.sprite.Free();
                alphabet_N.sprite.LoadImage("texture/UI_N_TRUE.png", State::m_renderer);
                if (!n_get_check)
                {
                    fever_object_count++;
                }
                n_get_check = true;
            }
            RemoveObject(obj);
        }
    }
}

void Level1::MapGeneratorAddObject()
{
    // Random is crawling by Kevin's code
    mg.AddMapToGame(Random(0, mg.StorageSize() - 1));
    for (int i = 0; i < mg.GetBackFromGame()->GetObjectSize(); i++)
    {
        MyObject* object = mg.GetBackFromGame()->GetObject(i);
        object->transform.position.x += mg.GetMapFromGame(mg.GameMapSize() - 2)->getEndPosition() + 250.f;
        object->physics.Free();
        object->physics.GenerateBody(GetPhysicsWorld(), &object->transform);
        object->sprite.Free();
        object->sprite.LoadImage(object->GetSource().c_str(), State::m_renderer);
        object->physics.ActiveGhostCollision(object->physics.IsActiveGhost());
        AddObject(object);
    }
    for (int i = 0; i < mg.GetBackFromGame()->GetScoreObjectSize(); i++)
    {
        FeverObjects feverType;
        ScoreObject* scoreObject = mg.GetBackFromGame()->GetScoreObject(i);
        std::string source = scoreObject->GetSource();
        std::string name = "coin" + (i + 1);
        if (source != "texture/Score_Object.png")
        {
            feverType = static_cast<FeverObjects>(Random(0, FeverObjects::NumOfFeverObjects - 1));
            source = GetFeverTypeSource(feverType);
            name = GetFeverTypeName(feverType);
        }
        scoreObject->transform.position.x += mg.GetMapFromGame(mg.GameMapSize() - 2)->getEndPosition() + 250.f;
        scoreObject->physics.Free();
        scoreObject->physics.GenerateBody(GetPhysicsWorld(), &scoreObject->transform);
        scoreObject->sprite.Free();
        scoreObject->sprite.LoadImage(source.c_str(), State::m_renderer);
        scoreObject->SetName(name.c_str());
        scoreObject->physics.ActiveGhostCollision(scoreObject->physics.IsActiveGhost());
        AddObject(scoreObject);
    }
    mg.GetBackFromGame()->Calculate();
}

void Level1::MapGeneratorRemoveObject()
{
    // Remove first map in gameMap list
    RemoveFirstMapFromLevel();
    mg.RemoveFirstGameMap();
}

void Level1::RemoveFirstMapFromLevel()
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


void Level1::Score()
{
    score++;
    scoreText.text.SetText("Score: %d", score);
}


void Level1::ControlCamera(float dt)
{
    if (!nextStage)
    {
        camera.position = b2Vec3(animation.transform.position.x + 350 - controlCamera_Zoom_X, 0, currentCameraZoomIn_Z);
        background.transform.position.y = camera.position.y;
        background.transform.position.x = camera.position.x - charged_dt * 5.f;
    }

    animationSensor.transform.position = b2Vec3(animation.transform.position.x, animation.transform.position.y - 55, 0);

    charged_dt += dt;
}

void Level1::TransformObjects(float dt)
{
    Health -= hpDecrease * dt;

    if (Health > HEALTH_WARNING_RANGE_DOWN && Health < HEALTH_WARNING_RANGE_UP)
    {
        warningSoundEffect.sound.Play();
    }
    if (Health < HEALTH_WARNING_RANGE)
    {
        isWarningFilterOn = true;
    }
    else
    {
        isWarningFilterOn = false;
    }

    if (rope_joint_def.collideConnected == true)
    {
        Health += HEALTH_CHARGE * dt;

        if (Health > HEALTH_SUFFICIENT)
        {
            Health = HEALTH_MAX;
        }
    }
    healthBar.transform.SetScale(Health, 68.0f);
    healthBar.transform.position.Set(-110 + healthBar.transform.GetScale().x / 2, -320.0f, 0.0f);
}

void Level1::makeNewRope()
{
    rope = new Object;
    rope->SetName("Rope");

    for (int i = 0; i < mg.GetMapFromGame(0)->GetNumOfCollisionBox(); i++)
    {
        //To get joinBoxt object
        JointBox = mg.GetMapFromGame(0)->GetJointBox(i);
        //To get CollisionBox object
        collisionBox = mg.GetMapFromGame(0)->GetObject(i);

        if (collisionBox->physics.HasBody())
        {
            if (collisionBox->physics.IsColliding())
            {
                std::cout << "Collide" << std::endl;
                rope_position = animation.transform.position + JointBox->transform.position;
                float ropeX = JointBox->transform.position.x - animation.transform.position.x;
                float ropeY = JointBox->transform.position.y - animation.transform.position.y;

                float ropeSize = sqrt((ropeX*ropeX) + (ropeY*ropeY));

                rope->transform.position.Set(rope_position.x / 2.0f, rope_position.y / 2.0f, rope_position.z);
                rope->transform.SetScale(ropeSize, 40);

                rope->transform.rotation = (ANGLE_180*GetAngle(JointBox)) / PI;
                rope->sprite.Free();
                rope->sprite.LoadImage("texture/Cable.png", m_renderer);

                rope_joint_def.bodyA = JointBox->physics.GetBody();
                rope_joint_def.bodyB = animation.physics.GetBody();

                //Set rope size here
                rope_joint_def.length = 7;

                rope_joint_def.collideConnected = true;
                rope_joint = (b2DistanceJoint*)GetPhysicsWorld()->CreateJoint(&rope_joint_def);
                ropeSoundEffect.sound.Play();
                AddObject(rope);
                isCameraZoom = true;
                //RemoveRope_RopeTransformUpdate(/*JointBox*/);
                return;
            }

        }
    }
    //No joint only make rope
    rope_position = animation.transform.position;
    rope->transform.position.Set(rope_position.x + 85, rope_position.y + 85, rope_position.z);
    rope->transform.SetScale(250, 40);
    rope->transform.rotation = ANGLE_45;
    rope->sprite.Free();
    rope->sprite.LoadImage("texture/Cable.png", m_renderer);
    AddObject(rope);
    timer = 0;
    isCameraZoom = true;

}

void Level1::Jumping(Object& object)
{
    object.transform.position = b2Vec3{ object.transform.position.x, -90.f, 0.f };
    object.physics.GetBody()->SetTransform(b2Vec2{ object.transform.position.x, -90.f }, 0.f);
    animation.sprite.Free();
    animation.sprite.LoadImage("texture/Jump_Pong.png", m_renderer);

    object.physics.SetVelocity(b2Vec2{ object.physics.GetVelocity().x ,75.f });
}

//need initialize the setting
void Level1::makeFadeOutAndIn(Stage stage) {

    animation.physics.SetVelocity(0.f, 0.f);
    SDL_Color color = BLACK;
    Filter.sprite.color = color;
    Filter.sprite.color.a = prevAlphaValue;

    //check the alpha for fade in
    if (Filter.sprite.color.a < 255 && Filter.sprite.color.a > 5 && isTimetoAlphaUp == true)
    {
        nextStage = true;
        if (Filter.sprite.color.a > 250) {
            StageChange(stage);

            isTimetoAlphaUp = false;
            isTimetoAlphaDown = true;
        }
        //It's kind of speed for fade in
        Filter.sprite.color.a += 3;
        prevAlphaValue = Filter.sprite.color.a;
    }
    //Check the alpha for fade out
    else if (isTimetoAlphaDown == true) {
        //It's time to move to next stage so need kind of initialize.
        if (Filter.sprite.color.a < 5) {
            isStageChange = true;
            isTimetoAlphaDown = false;
            isTimetoAlphaUp = true;
            prevAlphaValue = 6;
            timer2 = prevTimeforFadeinOut;
            nextStage = false;
            return;
        }
        //It's kind of speed for fade out
        Filter.sprite.color.a -= 3;
        prevAlphaValue = Filter.sprite.color.a;
    }
}
void Level1::SpeedUp()
{   //Make sure make isStageChange false before move to next statements
    if ((timer2 > checkTimerforspeedUp*2 && timer2 < checkTimerforspeedUp * 2 +0.4f)
        || (timer2 > checkTimerforspeedUp * 3 +0.2 && timer2 <checkTimerforspeedUp * 3 +0.4f)) {
        isStageChange = false;
    }

    animation.physics.SetVelocity(b2Vec2{ playerVelocity.x , animation.physics.GetVelocity().y });
    //Highway stage
    if (timer2 > checkTimerforspeedUp -0.2 && timer2 < checkTimerforspeedUp*2 +0.2)
    {
        //It's kind of pause(before move to next stage)
        if (isStageChange != true)
        {
            //Save preveTime to start game from the paused time
            prevTimeforFadeinOut = timer2;
            makeFadeOutAndIn(DownTown);
        }
    }
    //Downtown stage
    else if (timer2 > checkTimerforspeedUp*2 -0.2 && timer2 < checkTimerforspeedUp*3 +0.2 )
    {
        if (isStageChange != true)
        {
            prevTimeforFadeinOut = timer2;
            makeFadeOutAndIn(HighWay);
        }
        if (isStageChange == true) {
            if (isHighway == false) {
                charged_dt = 0;
                isHighway = true;
            }
        }
    }

    else if (timer2 > checkTimerforspeedUp*3 +0.2)// && timer2 < 160)
    {
        if (isStageChange != true)
        {
            prevTimeforFadeinOut = timer2;
            makeFadeOutAndIn(CountryRoad);
        }
        if (isStageChange == true) {
            if (isCountryRoad == false) {
                charged_dt = 0;
                isCountryRoad = true;
            }
            // Flag On
            onSpeedUp = false;
        }
    }

    ////Screen up the speed up text and remove + - 0.2 are to check epsilon 
    if (timer2 <checkSecond + 0.2f && timer2 > checkSecond - 0.2f && onSpeedUp == true) {
        speedUpText.transform.position.Set(200.f, 100.f, 0.f);
        speedUpText.transform.SetScale(500.f, 150.f);
        speedUpText.text.font = mainFont;
        speedUpText.text.isHud = true;
        speedUpText.text.color = RED;
        speedUpText.text.SetText("Speed Up!!");
        AddObject(&speedUpText);
        checkPrevSecond = checkSecond;
        checkSecond += 20.0f;

    }
    if (timer2 <checkPrevSecond + 1.2f && timer2 >checkPrevSecond + 0.8f)
    {
        speedUpText.text.Free();
    }
}




void Level1::MapGeneratorInitFactory()
{
    // Map with One Rope Joint.
    Map* m1 = new Map;

    // Constructing Collision box for rope joint
    MyObject* collisionBox1 = new MyObject;
    collisionBox1->SetName("CollisionBox1");
    collisionBox1->transform.position.Set(450.f, 30.f, 0.f);
    collisionBox1->SetScale(b2Vec2{ 80.f, 160.f });
    collisionBox1->SetSource("texture/Tutorial_Object.png");
    collisionBox1->physics.bodyType = Physics::STATIC;
    collisionBox1->physics.Free();
    collisionBox1->physics.GenerateBody(GetPhysicsWorld(), &collisionBox1->transform);
    collisionBox1->SetPhysicsWorld(GetPhysicsWorld());
    collisionBox1->physics.ActiveGhostCollision(true);
    m1->AddObject(collisionBox1);
    m1->SetNumOfCollisionBox(1);


    // Constructing Rope Joint
    MyObject* objectForRope = new MyObject;
    objectForRope->SetName("objectForRope");
    objectForRope->transform.position.Set(700.f, 200.0f, 0.0f);
    objectForRope->SetScale(b2Vec2{ 75, 400 });
    objectForRope->SetSource("texture/Charge_Object.png");
    objectForRope->physics.bodyType = Physics::STATIC;
    objectForRope->physics.Free();
    objectForRope->physics.GenerateBody(GetPhysicsWorld(), &objectForRope->transform);
    objectForRope->SetPhysicsWorld(GetPhysicsWorld());
    objectForRope->physics.ActiveGhostCollision(true);
    m1->AddObject(objectForRope);

    // Constructing Platform
    MyObject* object1 = new MyObject;
    object1->SetSource("texture/Factory_Tile_New.png");
    object1->SetPhysicsWorld(GetPhysicsWorld());
    object1->transform.position.Set(collisionBox1->transform.position.x - 250.f, -400.f, 0.f);
    object1->SetScale(b2Vec2{ 600.f, 320.f });
    object1->SetName("test1");
    object1->transform.rotation = 0;
    m1->AddObject(object1);

    // Constructing ScoreObject


    ScoreObject* Object_G = new ScoreObject;
    Object_G->SetPhysicsWorld(GetPhysicsWorld());
    Object_G->SetScale(b2Vec2{ 50.f, 50.f });
    Object_G->SetSource("texture/Object_G.png");
    Object_G->SetName("Object_G");
    Object_G->transform.position.Set(700.f, 50.f, 0.f);
    Object_G->physics.bodyType = Physics::STATIC;
    Object_G->physics.Free();
    Object_G->physics.GenerateBody(GetPhysicsWorld(), &Object_G->transform);
    Object_G->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(Object_G);

    // Constructing Platform
    MyObject* object2 = new MyObject;
    //object2->SetPhysicsWorld(GetPhysicsWorld());
    object2->SetSource("texture/Factory_Tile_New2.png");
    object2->transform.position.Set(1200.f, -400.f, 0.f);
    object2->SetScale(b2Vec2{ 600.f, 320.f });
    object2->SetName("test2");
    object2->physics.bodyType = Physics::STATIC;
    m1->AddObject(object2);

    // Coin
    ScoreObject* coin16 = new ScoreObject;
    coin16->SetPhysicsWorld(GetPhysicsWorld());
    coin16->SetScale(b2Vec2{ 50.f, 50.f });
    coin16->SetSource("texture/Score_Object.png");
    coin16->SetName("coin1");
    coin16->transform.position.Set(0.f, -100.f, 0.f);
    coin16->physics.bodyType = Physics::STATIC;
    coin16->physics.Free();
    coin16->physics.GenerateBody(GetPhysicsWorld(), &coin16->transform);
    coin16->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin16);

    // Coin
    ScoreObject* coin17 = new ScoreObject;
    coin17->SetPhysicsWorld(GetPhysicsWorld());
    coin17->SetScale(b2Vec2{ 50.f, 50.f });
    coin17->SetSource("texture/Score_Object.png");
    coin17->SetName("coin1");
    coin17->transform.position.Set(100.f, -160.f, 0.f);
    coin17->physics.bodyType = Physics::STATIC;
    coin17->physics.Free();
    coin17->physics.GenerateBody(GetPhysicsWorld(), &coin17->transform);
    coin17->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin17);

    // Coin
    ScoreObject* coin18 = new ScoreObject;
    coin18->SetPhysicsWorld(GetPhysicsWorld());
    coin18->SetScale(b2Vec2{ 50.f, 50.f });
    coin18->SetSource("texture/Score_Object.png");
    coin18->SetName("coin1");
    coin18->transform.position.Set(200.f, -160.f, 0.f);
    coin18->physics.bodyType = Physics::STATIC;
    coin18->physics.Free();
    coin18->physics.GenerateBody(GetPhysicsWorld(), &coin18->transform);
    coin18->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin18);

    // Coin
    ScoreObject* coin19 = new ScoreObject;
    coin19->SetPhysicsWorld(GetPhysicsWorld());
    coin19->SetScale(b2Vec2{ 50.f, 50.f });
    coin19->SetSource("texture/Score_Object.png");
    coin19->SetName("coin1");
    coin19->transform.position.Set(300.f, -60.f, 0.f);
    coin19->physics.bodyType = Physics::STATIC;
    coin19->physics.Free();
    coin19->physics.GenerateBody(GetPhysicsWorld(), &coin19->transform);
    coin19->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin19);

    // Coin
    ScoreObject* coin20 = new ScoreObject;
    coin20->SetPhysicsWorld(GetPhysicsWorld());
    coin20->SetScale(b2Vec2{ 50.f, 40.f });
    coin20->SetSource("texture/Score_Object.png");
    coin20->SetName("coin1");
    coin20->transform.position.Set(400.f, 40.f, 0.f);
    coin20->physics.bodyType = Physics::STATIC;
    coin20->physics.Free();
    coin20->physics.GenerateBody(GetPhysicsWorld(), &coin20->transform);
    coin20->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin20);

    // Coin
    ScoreObject* coin25 = new ScoreObject;
    coin25->SetPhysicsWorld(GetPhysicsWorld());
    coin25->SetScale(b2Vec2{ 50.f, 40.f });
    coin25->SetSource("texture/Score_Object.png");
    coin25->SetName("coin1");
    coin25->transform.position.Set(900.f, -60.f, 0.f);
    coin25->physics.bodyType = Physics::STATIC;
    coin25->physics.Free();
    coin25->physics.GenerateBody(GetPhysicsWorld(), &coin25->transform);
    coin25->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin25);

    // Coin
    ScoreObject* coin21 = new ScoreObject;
    coin21->SetPhysicsWorld(GetPhysicsWorld());
    coin21->SetScale(b2Vec2{ 50.f, 40.f });
    coin21->SetSource("texture/Score_Object.png");
    coin21->SetName("coin1");
    coin21->transform.position.Set(1000.f, -160.f, 0.f);
    coin21->physics.bodyType = Physics::STATIC;
    coin21->physics.Free();
    coin21->physics.GenerateBody(GetPhysicsWorld(), &coin21->transform);
    coin21->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin21);

    // Coin
    ScoreObject* coin22 = new ScoreObject;
    coin22->SetPhysicsWorld(GetPhysicsWorld());
    coin22->SetScale(b2Vec2{ 50.f, 40.f });
    coin22->SetSource("texture/Score_Object.png");
    coin22->SetName("coin1");
    coin22->transform.position.Set(1100.f, -160.f, 0.f);
    coin22->physics.bodyType = Physics::STATIC;
    coin22->physics.Free();
    coin22->physics.GenerateBody(GetPhysicsWorld(), &coin22->transform);
    coin22->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin22);

    // Coin
    ScoreObject* coin23 = new ScoreObject;
    coin23->SetPhysicsWorld(GetPhysicsWorld());
    coin23->SetScale(b2Vec2{ 50.f, 40.f });
    coin23->SetSource("texture/Score_Object.png");
    coin23->SetName("coin1");
    coin23->transform.position.Set(1200.f, -160.f, 0.f);
    coin23->physics.bodyType = Physics::STATIC;
    coin23->physics.Free();
    coin23->physics.GenerateBody(GetPhysicsWorld(), &coin23->transform);
    coin23->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin23);

    // Coin
    ScoreObject* coin24 = new ScoreObject;
    coin24->SetPhysicsWorld(GetPhysicsWorld());
    coin24->SetScale(b2Vec2{ 50.f, 40.f });
    coin24->SetSource("texture/Score_Object.png");
    coin24->SetName("coin1");
    coin24->transform.position.Set(1300.f, -160.f, 0.f);
    coin24->physics.bodyType = Physics::STATIC;
    coin24->physics.Free();
    coin24->physics.GenerateBody(GetPhysicsWorld(), &coin24->transform);
    coin24->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin24);

    mg.AddMap(m1);

    // Second Map
    Map* m2 = new Map;

    ScoreObject* Object_E = new ScoreObject;
    Object_E->SetPhysicsWorld(GetPhysicsWorld());
    Object_E->SetScale(b2Vec2{ 50.f, 50.f });
    Object_E->SetSource("texture/Object_E.png");
    Object_E->SetName("Object_E");
    Object_E->transform.position.Set(600.f, 50.f, 0.f);
    Object_E->physics.bodyType = Physics::STATIC;
    Object_E->physics.Free();
    Object_E->physics.GenerateBody(GetPhysicsWorld(), &Object_E->transform);
    Object_E->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(Object_E);

    // Why distance changed so dynamically? I should find the reasons why.
    MyObject* normalPlatform = new MyObject;
    normalPlatform->SetPhysicsWorld(GetPhysicsWorld());
    normalPlatform->SetSource("texture/Factory_Tile_New2.png");
    normalPlatform->SetScale(b2Vec2{ 1200.f, 320.f });
    normalPlatform->SetName("Normal Platform");
    normalPlatform->transform.position.Set(600.f, -400.f, 0.f);
    normalPlatform->physics.bodyType = Physics::STATIC;
    m2->AddObject(normalPlatform);

    // Coin
    ScoreObject* coin2 = new ScoreObject;
    coin2->SetPhysicsWorld(GetPhysicsWorld());
    coin2->SetScale(b2Vec2{ 50.f, 50.f });
    coin2->SetSource("texture/Score_Object.png");
    coin2->SetName("coin1");
    coin2->transform.position.Set(900.f, -160.f, 0.f);
    coin2->physics.bodyType = Physics::STATIC;
    coin2->physics.Free();
    coin2->physics.GenerateBody(GetPhysicsWorld(), &coin2->transform);
    coin2->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin2);

    // Coin
    ScoreObject* coin13 = new ScoreObject;
    coin13->SetPhysicsWorld(GetPhysicsWorld());
    coin13->SetScale(b2Vec2{ 50.f, 50.f });
    coin13->SetSource("texture/Score_Object.png");
    coin13->SetName("coin1");
    coin13->transform.position.Set(1300.f, 0.f, 0.f);
    coin13->physics.bodyType = Physics::STATIC;
    coin13->physics.Free();
    coin13->physics.GenerateBody(GetPhysicsWorld(), &coin13->transform);
    coin13->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin13);

    // Coin
    ScoreObject* coin12 = new ScoreObject;
    coin12->SetPhysicsWorld(GetPhysicsWorld());
    coin12->SetScale(b2Vec2{ 50.f, 50.f });
    coin12->SetSource("texture/Score_Object.png");
    coin12->SetName("coin1");
    coin12->transform.position.Set(1200.f, -100.f, 0.f);
    coin12->physics.bodyType = Physics::STATIC;
    coin12->physics.Free();
    coin12->physics.GenerateBody(GetPhysicsWorld(), &coin12->transform);
    coin12->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin12);

    // Coin
    ScoreObject* coin4 = new ScoreObject;
    coin4->SetPhysicsWorld(GetPhysicsWorld());
    coin4->SetScale(b2Vec2{ 50.f, 50.f });
    coin4->SetSource("texture/Score_Object.png");
    coin4->SetName("coin1");
    coin4->transform.position.Set(1100.f, -160.f, 0.f);
    coin4->physics.bodyType = Physics::STATIC;
    coin4->physics.Free();
    coin4->physics.GenerateBody(GetPhysicsWorld(), &coin4->transform);
    coin4->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin4);

    // Coin
    ScoreObject* coin3 = new ScoreObject;
    coin3->SetPhysicsWorld(GetPhysicsWorld());
    coin3->SetScale(b2Vec2{ 50.f, 50.f });
    coin3->SetSource("texture/Score_Object.png");
    coin3->SetName("coin1");
    coin3->transform.position.Set(1000.f, -160.f, 0.f);
    coin3->physics.bodyType = Physics::STATIC;
    coin3->physics.Free();
    coin3->physics.GenerateBody(GetPhysicsWorld(), &coin3->transform);
    coin3->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin3);

    // Coin
    ScoreObject* coin1 = new ScoreObject;
    coin1->SetPhysicsWorld(GetPhysicsWorld());
    coin1->SetScale(b2Vec2{ 50.f, 50.f });
    coin1->SetSource("texture/Score_Object.png");
    coin1->SetName("coin1");
    coin1->transform.position.Set(800.f, -130.f, 0.f);
    coin1->physics.bodyType = Physics::STATIC;
    coin1->physics.Free();
    coin1->physics.GenerateBody(GetPhysicsWorld(), &coin1->transform);
    coin1->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin1);

    // Coin
    ScoreObject* coin5 = new ScoreObject;
    coin5->SetPhysicsWorld(GetPhysicsWorld());
    coin5->SetScale(b2Vec2{ 50.f, 50.f });
    coin5->SetSource("texture/Score_Object.png");
    coin5->SetName("coin1");
    coin5->transform.position.Set(700.f, -30.f, 0.f);
    coin5->physics.bodyType = Physics::STATIC;
    coin5->physics.Free();
    coin5->physics.GenerateBody(GetPhysicsWorld(), &coin5->transform);
    coin5->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin5);

    // Coin
    ScoreObject* coin6 = new ScoreObject;
    coin6->SetPhysicsWorld(GetPhysicsWorld());
    coin6->SetScale(b2Vec2{ 50.f, 50.f });
    coin6->SetSource("texture/Score_Object.png");
    coin6->SetName("coin1");
    coin6->transform.position.Set(500.f, -30.f, 0.f);
    coin6->physics.bodyType = Physics::STATIC;
    coin6->physics.Free();
    coin6->physics.GenerateBody(GetPhysicsWorld(), &coin6->transform);
    coin6->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin6);

    // Coin
    ScoreObject* coin8 = new ScoreObject;
    coin8->SetPhysicsWorld(GetPhysicsWorld());
    coin8->SetScale(b2Vec2{ 50.f, 50.f });
    coin8->SetSource("texture/Score_Object.png");
    coin8->SetName("coin1");
    coin8->transform.position.Set(400.f, -130.f, 0.f);
    coin8->physics.bodyType = Physics::STATIC;
    coin8->physics.Free();
    coin8->physics.GenerateBody(GetPhysicsWorld(), &coin8->transform);
    coin8->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin8);

    // Coin
    ScoreObject* coin9 = new ScoreObject;
    coin9->SetPhysicsWorld(GetPhysicsWorld());
    coin9->SetScale(b2Vec2{ 50.f, 50.f });
    coin9->SetSource("texture/Score_Object.png");
    coin9->SetName("coin1");
    coin9->transform.position.Set(300.f, -160.f, 0.f);
    coin9->physics.bodyType = Physics::STATIC;
    coin9->physics.Free();
    coin9->physics.GenerateBody(GetPhysicsWorld(), &coin9->transform);
    coin9->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin9);

    // Coin
    ScoreObject* coin10 = new ScoreObject;
    coin10->SetPhysicsWorld(GetPhysicsWorld());
    coin10->SetScale(b2Vec2{ 50.f, 50.f });
    coin10->SetSource("texture/Score_Object.png");
    coin10->SetName("coin1");
    coin10->transform.position.Set(200.f, -160.f, 0.f);
    coin10->physics.bodyType = Physics::STATIC;
    coin10->physics.Free();
    coin10->physics.GenerateBody(GetPhysicsWorld(), &coin10->transform);
    coin10->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin10);

    // Coin
    ScoreObject* coin11 = new ScoreObject;
    coin11->SetPhysicsWorld(GetPhysicsWorld());
    coin11->SetScale(b2Vec2{ 50.f, 50.f });
    coin11->SetSource("texture/Score_Object.png");
    coin11->SetName("coin1");
    coin11->transform.position.Set(100.f, -160.f, 0.f);
    coin11->physics.bodyType = Physics::STATIC;
    coin11->physics.Free();
    coin11->physics.GenerateBody(GetPhysicsWorld(), &coin11->transform);
    coin11->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin11);

    m2->ToggleRopeAvailable();

    mg.AddMap(m2);
}

void Level1::MapGeneratorInitHighWay()
{
    m_backgroundColor = BLACK;
    background.sprite.Free();
    background.sprite.LoadImage("texture/Highway_Background.png", State::m_renderer);
    background.transform.SetScale(2000.f, 724.f);
    background.sprite.activeAnimation = true;
    background.sprite.SetFrame(2);
    background.sprite.SetSpeed(2.f);
    AddObject(&background);

    // Map 1 with 2 platforms and 2 rope joint boxes.
    Map* m1 = new Map;

    // Constructing Collision box for rope joint
    MyObject* collisionBox0 = new MyObject;
    collisionBox0->SetName("CollisionBox0");
    collisionBox0->transform.position.Set(550.f, 30.f, 0.f);
    collisionBox0->transform.SetScale(b2Vec2{ 80.f, 160.f });
    collisionBox0->SetScale(b2Vec2{ 80.f, 80.f });
    collisionBox0->SetSource("texture/rect.png");
    collisionBox0->sprite.color.a = 0;
    collisionBox0->physics.bodyType = Physics::STATIC;
    collisionBox0->physics.Free();
    collisionBox0->physics.GenerateBody(GetPhysicsWorld(), &collisionBox0->transform);
    collisionBox0->SetPhysicsWorld(GetPhysicsWorld());
    collisionBox0->physics.ActiveGhostCollision(true);
    m1->AddObject(collisionBox0);

    // Constructing Collision box for rope joint
    MyObject* collisionBox1 = new MyObject;
    collisionBox1->SetName("CollisionBox1");
    collisionBox1->transform.position.Set(1100.f, 30.f, 0.f);
    collisionBox1->transform.SetScale(b2Vec2{ 80.f, 160.f });
    collisionBox1->SetScale(b2Vec2{ 80.f, 80.f });
    collisionBox1->SetSource("texture/rect.png");
    collisionBox1->sprite.color.a = 0;
    collisionBox1->physics.bodyType = Physics::STATIC;
    collisionBox1->physics.Free();
    collisionBox1->physics.GenerateBody(GetPhysicsWorld(), &collisionBox1->transform);
    collisionBox1->SetPhysicsWorld(GetPhysicsWorld());
    collisionBox1->physics.ActiveGhostCollision(true);
    m1->AddObject(collisionBox1);
    m1->SetNumOfCollisionBox(2);

    // Constructing Rope Joint
    MyObject* objectForRope0 = new MyObject;
    objectForRope0->SetName("objectForRope0");
    objectForRope0->transform.position.Set(800.f, 200.0f, 0.0f);
    objectForRope0->SetScale(b2Vec2{ 75, 400 });
    objectForRope0->SetSource("texture/Charge_Object.png");
    objectForRope0->physics.bodyType = Physics::STATIC;
    objectForRope0->physics.Free();
    objectForRope0->physics.GenerateBody(GetPhysicsWorld(), &objectForRope0->transform);
    objectForRope0->SetPhysicsWorld(GetPhysicsWorld());
    objectForRope0->physics.ActiveGhostCollision(true);
    m1->AddObject(objectForRope0);

    // Constructing Rope Joint
    MyObject* objectForRope1 = new MyObject;
    objectForRope1->SetName("objectForRope1");
    objectForRope1->transform.position.Set(1350.f, 200.0f, 0.0f);
    objectForRope1->SetScale(b2Vec2{ 75, 400 });
    objectForRope1->SetSource("texture/Charge_Object.png");
    objectForRope1->physics.bodyType = Physics::STATIC;
    objectForRope1->physics.Free();
    objectForRope1->physics.GenerateBody(GetPhysicsWorld(), &objectForRope1->transform);
    objectForRope1->SetPhysicsWorld(GetPhysicsWorld());
    objectForRope1->physics.ActiveGhostCollision(true);
    m1->AddObject(objectForRope1);

    ScoreObject* Object_P = new ScoreObject;
    Object_P->SetPhysicsWorld(GetPhysicsWorld());
    Object_P->SetScale(b2Vec2{ 50.f, 50.f });
    Object_P->SetSource("texture/Object_P.png");
    Object_P->SetName("Object_P");
    Object_P->transform.position.Set(200, 50.f, 0.f);
    Object_P->physics.bodyType = Physics::STATIC;
    Object_P->physics.Free();
    Object_P->physics.GenerateBody(GetPhysicsWorld(), &Object_P->transform);
    Object_P->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(Object_P);

    // Constructing Platform
    MyObject* pattern0Platform0 = new MyObject;
    pattern0Platform0->SetSource("texture/Highway_Tile_New.png");
    pattern0Platform0->SetPhysicsWorld(GetPhysicsWorld());
    pattern0Platform0->SetScale(b2Vec2{ 600.f, 320.f });
    pattern0Platform0->SetName("pattern0Platform0");
    pattern0Platform0->transform.position.Set(300.f, -400.f, 0.f);
    pattern0Platform0->transform.rotation = 0;
    m1->AddObject(pattern0Platform0);

    // Coin
    ScoreObject* coin1 = new ScoreObject;
    coin1->SetPhysicsWorld(GetPhysicsWorld());
    coin1->SetScale(b2Vec2{ 50.f, 50.f });
    coin1->SetSource("texture/Score_Object.png");
    coin1->SetName("coin1");
    coin1->transform.position.Set(100.f, -110.f, 0.f);
    coin1->physics.bodyType = Physics::STATIC;
    coin1->physics.Free();
    coin1->physics.GenerateBody(GetPhysicsWorld(), &coin1->transform);
    coin1->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin1);

    // Coin
    ScoreObject* coin2 = new ScoreObject;
    coin2->SetPhysicsWorld(GetPhysicsWorld());
    coin2->SetScale(b2Vec2{ 50.f, 50.f });
    coin2->SetSource("texture/Score_Object.png");
    coin2->SetName("coin2");
    coin2->transform.position.Set(200.f, -160.f, 0.f);
    coin2->physics.bodyType = Physics::STATIC;
    coin2->physics.Free();
    coin2->physics.GenerateBody(GetPhysicsWorld(), &coin2->transform);
    coin2->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin2);

    // Coin
    ScoreObject* coin3 = new ScoreObject;
    coin3->SetPhysicsWorld(GetPhysicsWorld());
    coin3->SetScale(b2Vec2{ 50.f, 50.f });
    coin3->SetSource("texture/Score_Object.png");
    coin3->SetName("coin3");
    coin3->transform.position.Set(300.f, -160.f, 0.f);
    coin3->physics.bodyType = Physics::STATIC;
    coin3->physics.Free();
    coin3->physics.GenerateBody(GetPhysicsWorld(), &coin3->transform);
    coin3->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin3);

    // Coin
    ScoreObject* coin4 = new ScoreObject;
    coin4->SetPhysicsWorld(GetPhysicsWorld());
    coin4->SetScale(b2Vec2{ 50.f, 50.f });
    coin4->SetSource("texture/Score_Object.png");
    coin4->SetName("coin4");
    coin4->transform.position.Set(400.f, -160.f, 0.f);
    coin4->physics.bodyType = Physics::STATIC;
    coin4->physics.Free();
    coin4->physics.GenerateBody(GetPhysicsWorld(), &coin4->transform);
    coin4->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin4);

    // Coin
    ScoreObject* coin5 = new ScoreObject;
    coin5->SetPhysicsWorld(GetPhysicsWorld());
    coin5->SetScale(b2Vec2{ 50.f, 50.f });
    coin5->SetSource("texture/Score_Object.png");
    coin5->SetName("coin5");
    coin5->transform.position.Set(500.f, 0.f, 0.f);
    coin5->physics.bodyType = Physics::STATIC;
    coin5->physics.Free();
    coin5->physics.GenerateBody(GetPhysicsWorld(), &coin5->transform);
    coin5->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin5);

    // Coin
    ScoreObject* coin34 = new ScoreObject;
    coin34->SetPhysicsWorld(GetPhysicsWorld());
    coin34->SetScale(b2Vec2{ 50.f, 50.f });
    coin34->SetSource("texture/Score_Object.png");
    coin34->SetName("coin34");
    coin34->transform.position.Set(600.f, 60.f, 0.f);
    coin34->physics.bodyType = Physics::STATIC;
    coin34->physics.Free();
    coin34->physics.GenerateBody(GetPhysicsWorld(), &coin34->transform);
    coin34->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin34);

    // Coin
    ScoreObject* coin35 = new ScoreObject;
    coin35->SetPhysicsWorld(GetPhysicsWorld());
    coin35->SetScale(b2Vec2{ 50.f, 50.f });
    coin35->SetSource("texture/Score_Object.png");
    coin35->SetName("coin35");
    coin35->transform.position.Set(1050.f, -10.f, 0.f);
    coin35->physics.bodyType = Physics::STATIC;
    coin35->physics.Free();
    coin35->physics.GenerateBody(GetPhysicsWorld(), &coin35->transform);
    coin35->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin35);

    // Coin
    ScoreObject* coin36 = new ScoreObject;
    coin36->SetPhysicsWorld(GetPhysicsWorld());
    coin36->SetScale(b2Vec2{ 50.f, 50.f });
    coin36->SetSource("texture/Score_Object.png");
    coin36->SetName("coin36");
    coin36->transform.position.Set(950.f, 50.f, 0.f);
    coin36->physics.bodyType = Physics::STATIC;
    coin36->physics.Free();
    coin36->physics.GenerateBody(GetPhysicsWorld(), &coin36->transform);
    coin36->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin36);

    // Coin
    ScoreObject* coin37 = new ScoreObject;
    coin37->SetPhysicsWorld(GetPhysicsWorld());
    coin37->SetScale(b2Vec2{ 50.f, 50.f });
    coin37->SetSource("texture/Score_Object.png");
    coin37->SetName("coin37");
    coin37->transform.position.Set(1150.f, 50.f, 0.f);
    coin37->physics.bodyType = Physics::STATIC;
    coin37->physics.Free();
    coin37->physics.GenerateBody(GetPhysicsWorld(), &coin37->transform);
    coin37->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin37);

    // Constructing Platform
    MyObject* pattern0Platform1 = new MyObject;
    pattern0Platform1->SetSource("texture/Highway_Tile_New.png");
    pattern0Platform1->SetPhysicsWorld(GetPhysicsWorld());
    pattern0Platform1->SetScale(b2Vec2{ 600.f, 320.f });
    pattern0Platform1->SetName("pattern0Platform1");
    pattern0Platform1->transform.position.Set(2000.f, -400.f, 0.f);
    pattern0Platform1->transform.rotation = 0;
    m1->AddObject(pattern0Platform1);

    // Coin
    ScoreObject* coin6 = new ScoreObject;
    coin6->SetPhysicsWorld(GetPhysicsWorld());
    coin6->SetScale(b2Vec2{ 50.f, 50.f });
    coin6->SetSource("texture/Score_Object.png");
    coin6->SetName("coin6");
    coin6->transform.position.Set(1800.f, -160.f, 0.f);
    coin6->physics.bodyType = Physics::STATIC;
    coin6->physics.Free();
    coin6->physics.GenerateBody(GetPhysicsWorld(), &coin6->transform);
    coin6->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin6);

    // Coin
    ScoreObject* coin40 = new ScoreObject;
    coin40->SetPhysicsWorld(GetPhysicsWorld());
    coin40->SetScale(b2Vec2{ 50.f, 50.f });
    coin40->SetSource("texture/Score_Object.png");
    coin40->SetName("coin40");
    coin40->transform.position.Set(1500.f, 140.f, 0.f);
    coin40->physics.bodyType = Physics::STATIC;
    coin40->physics.Free();
    coin40->physics.GenerateBody(GetPhysicsWorld(), &coin40->transform);
    coin40->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin40);

    // Coin
    ScoreObject* coin39 = new ScoreObject;
    coin39->SetPhysicsWorld(GetPhysicsWorld());
    coin39->SetScale(b2Vec2{ 50.f, 50.f });
    coin39->SetSource("texture/Score_Object.png");
    coin39->SetName("coin39");
    coin39->transform.position.Set(1600.f, 40.f, 0.f);
    coin39->physics.bodyType = Physics::STATIC;
    coin39->physics.Free();
    coin39->physics.GenerateBody(GetPhysicsWorld(), &coin39->transform);
    coin39->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin39);

    // Coin
    ScoreObject* coin38 = new ScoreObject;
    coin38->SetPhysicsWorld(GetPhysicsWorld());
    coin38->SetScale(b2Vec2{ 50.f, 50.f });
    coin38->SetSource("texture/Score_Object.png");
    coin38->SetName("coin38");
    coin38->transform.position.Set(1700.f, -60.f, 0.f);
    coin38->physics.bodyType = Physics::STATIC;
    coin38->physics.Free();
    coin38->physics.GenerateBody(GetPhysicsWorld(), &coin38->transform);
    coin38->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin38);

    // Coin
    ScoreObject* coin7 = new ScoreObject;
    coin7->SetPhysicsWorld(GetPhysicsWorld());
    coin7->SetScale(b2Vec2{ 50.f, 50.f });
    coin7->SetSource("texture/Score_Object.png");
    coin7->SetName("coin7");
    coin7->transform.position.Set(1800.f, -160.f, 0.f);
    coin7->physics.bodyType = Physics::STATIC;
    coin7->physics.Free();
    coin7->physics.GenerateBody(GetPhysicsWorld(), &coin7->transform);
    coin7->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin7);

    // Coin
    ScoreObject* coin8 = new ScoreObject;
    coin8->SetPhysicsWorld(GetPhysicsWorld());
    coin8->SetScale(b2Vec2{ 50.f, 50.f });
    coin8->SetSource("texture/Score_Object.png");
    coin8->SetName("coin8");
    coin8->transform.position.Set(2000.f, -160.f, 0.f);
    coin8->physics.bodyType = Physics::STATIC;
    coin8->physics.Free();
    coin8->physics.GenerateBody(GetPhysicsWorld(), &coin8->transform);
    coin8->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin8);

    // Coin
    ScoreObject* coin9 = new ScoreObject;
    coin9->SetPhysicsWorld(GetPhysicsWorld());
    coin9->SetScale(b2Vec2{ 50.f, 50.f });
    coin9->SetSource("texture/Score_Object.png");
    coin9->SetName("coin9");
    coin9->transform.position.Set(2100.f, -160.f, 0.f);
    coin9->physics.bodyType = Physics::STATIC;
    coin9->physics.Free();
    coin9->physics.GenerateBody(GetPhysicsWorld(), &coin9->transform);
    coin9->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin9);

    // Coin
    ScoreObject* coin10 = new ScoreObject;
    coin10->SetPhysicsWorld(GetPhysicsWorld());
    coin10->SetScale(b2Vec2{ 50.f, 50.f });
    coin10->SetSource("texture/Score_Object.png");
    coin10->SetName("coin10");
    coin10->transform.position.Set(2200.f, -160.f, 0.f);
    coin10->physics.bodyType = Physics::STATIC;
    coin10->physics.Free();
    coin10->physics.GenerateBody(GetPhysicsWorld(), &coin10->transform);
    coin10->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin10);

    // Coin
    ScoreObject* coin11 = new ScoreObject;
    coin11->SetPhysicsWorld(GetPhysicsWorld());
    coin11->SetScale(b2Vec2{ 50.f, 50.f });
    coin11->SetSource("texture/Score_Object.png");
    coin11->SetName("coin11");
    coin11->transform.position.Set(2300.f, -60.f, 0.f);
    coin11->physics.bodyType = Physics::STATIC;
    coin11->physics.Free();
    coin11->physics.GenerateBody(GetPhysicsWorld(), &coin11->transform);
    coin11->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin11);

    // Coin
    ScoreObject* coin12 = new ScoreObject;
    coin12->SetPhysicsWorld(GetPhysicsWorld());
    coin12->SetScale(b2Vec2{ 50.f, 50.f });
    coin12->SetSource("texture/Score_Object.png");
    coin12->SetName("coin12");
    coin12->transform.position.Set(2400.f, 40.f, 0.f);
    coin12->physics.bodyType = Physics::STATIC;
    coin12->physics.Free();
    coin12->physics.GenerateBody(GetPhysicsWorld(), &coin12->transform);
    coin12->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin12);

    mg.AddMap(m1);


    // Map 2 with 2 paltforms and 1 rope joint boxes.
    Map* m2 = new Map;

    // Constructing Collision box for rope joint
    MyObject* collisionBox2 = new MyObject;
    collisionBox2->SetName("CollisionBox2");
    collisionBox2->transform.position.Set(800.f, 30.f, 0.f);
    collisionBox2->transform.SetScale(b2Vec2{ 80.f, 160.f });
    collisionBox2->SetScale(b2Vec2{ 80.f, 80.f });
    collisionBox2->SetSource("texture/rect.png");
    collisionBox2->sprite.color.a = 0;

    collisionBox2->physics.bodyType = Physics::STATIC;
    collisionBox2->physics.Free();
    collisionBox2->physics.GenerateBody(GetPhysicsWorld(), &collisionBox2->transform);
    collisionBox2->SetPhysicsWorld(GetPhysicsWorld());
    collisionBox2->physics.ActiveGhostCollision(true);
    m2->AddObject(collisionBox2);
    m2->SetNumOfCollisionBox(1);

    // Constructing Rope Joint
    MyObject* objectForRope2 = new MyObject;
    objectForRope2->SetName("objectForRope3");
    objectForRope2->transform.position.Set(1050.f, 200.0f, 0.0f);
    objectForRope2->SetScale(b2Vec2{ 75, 400 });
    objectForRope2->SetSource("texture/Charge_Object.png");
    objectForRope2->physics.bodyType = Physics::STATIC;
    objectForRope2->physics.Free();
    objectForRope2->physics.GenerateBody(GetPhysicsWorld(), &objectForRope2->transform);
    objectForRope2->SetPhysicsWorld(GetPhysicsWorld());
    objectForRope2->physics.ActiveGhostCollision(true);
    m2->AddObject(objectForRope2);

    // Constructing Platform
    MyObject* pattern1Platform0 = new MyObject;
    pattern1Platform0->SetSource("texture/Highway_Tile_New.png");
    pattern1Platform0->SetPhysicsWorld(GetPhysicsWorld());
    pattern1Platform0->SetScale(b2Vec2{ 800.f, 320.f });
    pattern1Platform0->SetName("pattern1Platform0");
    pattern1Platform0->transform.position.Set(500.f, -400.f, 0.f);
    pattern1Platform0->transform.rotation = 0;
    m2->AddObject(pattern1Platform0);

    // Coin
    ScoreObject* coin20 = new ScoreObject;
    coin20->SetPhysicsWorld(GetPhysicsWorld());
    coin20->SetScale(b2Vec2{ 50.f, 50.f });
    coin20->SetSource("texture/Score_Object.png");
    coin20->SetName("coin20");
    coin20->transform.position.Set(100.f, -160.f, 0.f);
    coin20->physics.bodyType = Physics::STATIC;
    coin20->physics.Free();
    coin20->physics.GenerateBody(GetPhysicsWorld(), &coin20->transform);
    coin20->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin20);

    // Coin
    ScoreObject* coin21 = new ScoreObject;
    coin21->SetPhysicsWorld(GetPhysicsWorld());
    coin21->SetScale(b2Vec2{ 50.f, 50.f });
    coin21->SetSource("texture/Score_Object.png");
    coin21->SetName("coin21");
    coin21->transform.position.Set(200.f, -160.f, 0.f);
    coin21->physics.bodyType = Physics::STATIC;
    coin21->physics.Free();
    coin21->physics.GenerateBody(GetPhysicsWorld(), &coin21->transform);
    coin21->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin21);

    // Coin
    ScoreObject* coin22 = new ScoreObject;
    coin22->SetPhysicsWorld(GetPhysicsWorld());
    coin22->SetScale(b2Vec2{ 50.f, 50.f });
    coin22->SetSource("texture/Score_Object.png");
    coin22->SetName("coin22");
    coin22->transform.position.Set(300.f, -160.f, 0.f);
    coin22->physics.bodyType = Physics::STATIC;
    coin22->physics.Free();
    coin22->physics.GenerateBody(GetPhysicsWorld(), &coin22->transform);
    coin22->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin22);

    // Coin
    ScoreObject* coin23 = new ScoreObject;
    coin23->SetPhysicsWorld(GetPhysicsWorld());
    coin23->SetScale(b2Vec2{ 50.f, 50.f });
    coin23->SetSource("texture/Score_Object.png");
    coin23->SetName("coin23");
    coin23->transform.position.Set(400.f, -160.f, 0.f);
    coin23->physics.bodyType = Physics::STATIC;
    coin23->physics.Free();
    coin23->physics.GenerateBody(GetPhysicsWorld(), &coin22->transform);
    coin23->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin23);

    ScoreObject* Object_D = new ScoreObject;
    Object_D->SetPhysicsWorld(GetPhysicsWorld());
    Object_D->SetScale(b2Vec2{ 50.f, 50.f });
    Object_D->SetSource("texture/Object_D.png");
    Object_D->SetName("Object_D");
    Object_D->transform.position.Set(300, 50.f, 0.f);
    Object_D->physics.bodyType = Physics::STATIC;
    Object_D->physics.Free();
    Object_D->physics.GenerateBody(GetPhysicsWorld(), &Object_D->transform);
    Object_D->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(Object_D);

    // Constructing Platform
    MyObject* pattern1Platform1 = new MyObject;
    pattern1Platform1->SetSource("texture/Highway_Tile_New.png");
    pattern1Platform1->SetPhysicsWorld(GetPhysicsWorld());
    pattern1Platform1->SetScale(b2Vec2{ 300.f, 320.f });
    pattern1Platform1->SetName("pattern3Platform1");
    pattern1Platform1->transform.position.Set(1450.f, -400.f, 0.f);
    pattern1Platform1->transform.rotation = 0;
    m2->AddObject(pattern1Platform1);

    // Coin
    ScoreObject* coin24 = new ScoreObject;
    coin24->SetPhysicsWorld(GetPhysicsWorld());
    coin24->SetScale(b2Vec2{ 50.f, 50.f });
    coin24->SetSource("texture/Score_Object.png");
    coin24->SetName("coin24");
    coin24->transform.position.Set(500.f, -160.f, 0.f);
    coin24->physics.bodyType = Physics::STATIC;
    coin24->physics.Free();
    coin24->physics.GenerateBody(GetPhysicsWorld(), &coin24->transform);
    coin24->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin24);

    // Coin
    ScoreObject* coin25 = new ScoreObject;
    coin25->SetPhysicsWorld(GetPhysicsWorld());
    coin25->SetScale(b2Vec2{ 50.f, 50.f });
    coin25->SetSource("texture/Score_Object.png");
    coin25->SetName("coin25");
    coin25->transform.position.Set(600.f, -60.f, 0.f);
    coin25->physics.bodyType = Physics::STATIC;
    coin25->physics.Free();
    coin25->physics.GenerateBody(GetPhysicsWorld(), &coin25->transform);
    coin25->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin25);

    // Coin
    ScoreObject* coin26 = new ScoreObject;
    coin26->SetPhysicsWorld(GetPhysicsWorld());
    coin26->SetScale(b2Vec2{ 50.f, 50.f });
    coin26->SetSource("texture/Score_Object.png");
    coin26->SetName("coin26");
    coin26->transform.position.Set(700.f, 20.f, 0.f);
    coin26->physics.bodyType = Physics::STATIC;
    coin26->physics.Free();
    coin26->physics.GenerateBody(GetPhysicsWorld(), &coin26->transform);
    coin26->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin26);

    // Coin
    ScoreObject* coin27 = new ScoreObject;
    coin27->SetPhysicsWorld(GetPhysicsWorld());
    coin27->SetScale(b2Vec2{ 50.f, 50.f });
    coin27->SetSource("texture/Score_Object.png");
    coin27->SetName("coin27");
    coin27->transform.position.Set(800.f, 80.f, 0.f);
    coin27->physics.bodyType = Physics::STATIC;
    coin27->physics.Free();
    coin27->physics.GenerateBody(GetPhysicsWorld(), &coin27->transform);
    coin27->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin27);

    // Coin
    ScoreObject* coin28 = new ScoreObject;
    coin28->SetPhysicsWorld(GetPhysicsWorld());
    coin28->SetScale(b2Vec2{ 50.f, 50.f });
    coin28->SetSource("texture/Score_Object.png");
    coin28->SetName("coin28");
    coin28->transform.position.Set(1200.f, 80.f, 0.f);
    coin28->physics.bodyType = Physics::STATIC;
    coin28->physics.Free();
    coin28->physics.GenerateBody(GetPhysicsWorld(), &coin28->transform);
    coin28->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin28);

    // Coin
    ScoreObject* coin29 = new ScoreObject;
    coin29->SetPhysicsWorld(GetPhysicsWorld());
    coin29->SetScale(b2Vec2{ 50.f, 50.f });
    coin29->SetSource("texture/Score_Object.png");
    coin29->SetName("coin29");
    coin29->transform.position.Set(1300.f, -40.f, 0.f);
    coin29->physics.bodyType = Physics::STATIC;
    coin29->physics.Free();
    coin29->physics.GenerateBody(GetPhysicsWorld(), &coin29->transform);
    coin29->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin29);

    // Coin
    ScoreObject* coin30 = new ScoreObject;
    coin30->SetPhysicsWorld(GetPhysicsWorld());
    coin30->SetScale(b2Vec2{ 50.f, 50.f });
    coin30->SetSource("texture/Score_Object.png");
    coin30->SetName("coin30");
    coin30->transform.position.Set(1400.f, -160.f, 0.f);
    coin30->physics.bodyType = Physics::STATIC;
    coin30->physics.Free();
    coin30->physics.GenerateBody(GetPhysicsWorld(), &coin30->transform);
    coin30->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin30);

    // Coin
    ScoreObject* coin33 = new ScoreObject;
    coin33->SetPhysicsWorld(GetPhysicsWorld());
    coin33->SetScale(b2Vec2{ 50.f, 50.f });
    coin33->SetSource("texture/Score_Object.png");
    coin33->SetName("coin33");
    coin33->transform.position.Set(1550.f, -160.f, 0.f);
    coin33->physics.bodyType = Physics::STATIC;
    coin33->physics.Free();
    coin33->physics.GenerateBody(GetPhysicsWorld(), &coin33->transform);
    coin33->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin33);

    // Coin
    ScoreObject* coin31 = new ScoreObject;
    coin31->SetPhysicsWorld(GetPhysicsWorld());
    coin31->SetScale(b2Vec2{ 50.f, 50.f });
    coin31->SetSource("texture/Score_Object.png");
    coin31->SetName("coin31");
    coin31->transform.position.Set(1650.f, -60.f, 0.f);
    coin31->physics.bodyType = Physics::STATIC;
    coin31->physics.Free();
    coin31->physics.GenerateBody(GetPhysicsWorld(), &coin31->transform);
    coin31->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin31);

    // Coin
    ScoreObject* coin32 = new ScoreObject;
    coin32->SetPhysicsWorld(GetPhysicsWorld());
    coin32->SetScale(b2Vec2{ 50.f, 50.f });
    coin32->SetSource("texture/Score_Object.png");
    coin32->SetName("coin31");
    coin32->transform.position.Set(1750.f, 40.f, 0.f);
    coin32->physics.bodyType = Physics::STATIC;
    coin32->physics.Free();
    coin32->physics.GenerateBody(GetPhysicsWorld(), &coin32->transform);
    coin32->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin32);

    mg.AddMap(m2);


    // Map 3 with 3 paltforms and 2 rope joint boxes.
    Map* m3 = new Map;

    // Constructing Collision box for rope joint
    MyObject* collisionBox3 = new MyObject;
    collisionBox3->SetName("CollisionBox3");
    collisionBox3->transform.position.Set(750.f, 30.f, 0.f);
    collisionBox3->transform.SetScale(b2Vec2{ 80.f, 160.f });
    collisionBox3->SetScale(b2Vec2{ 80.f, 80.f });
    collisionBox3->SetSource("texture/rect.png");
    collisionBox3->sprite.color.a = 0;
    collisionBox3->physics.bodyType = Physics::STATIC;
    collisionBox3->physics.Free();
    collisionBox3->physics.GenerateBody(GetPhysicsWorld(), &collisionBox3->transform);
    collisionBox3->SetPhysicsWorld(GetPhysicsWorld());
    collisionBox3->physics.ActiveGhostCollision(true);
    m3->AddObject(collisionBox3);

    // Constructing Collision box for rope joint
    MyObject* collisionBox4 = new MyObject;
    collisionBox4->SetName("CollisionBox1");
    collisionBox4->transform.position.Set(1700.f, 30.f, 0.f);
    collisionBox4->transform.SetScale(b2Vec2{ 80.f, 160.f });
    collisionBox4->SetScale(b2Vec2{ 80.f, 80.f });
    collisionBox4->SetSource("texture/rect.png");
    collisionBox4->sprite.color.a = 0;
    collisionBox4->physics.bodyType = Physics::STATIC;
    collisionBox4->physics.Free();
    collisionBox4->physics.GenerateBody(GetPhysicsWorld(), &collisionBox4->transform);
    collisionBox4->SetPhysicsWorld(GetPhysicsWorld());
    collisionBox4->physics.ActiveGhostCollision(true);
    m3->AddObject(collisionBox4);
    m3->SetNumOfCollisionBox(2);

    // Constructing Rope Joint
    MyObject* objectForRope3 = new MyObject;
    objectForRope3->SetName("objectForRope3");
    objectForRope3->transform.position.Set(1000.f, 200.0f, 0.0f);
    objectForRope3->SetScale(b2Vec2{ 75, 400 });
    objectForRope3->SetSource("texture/Charge_Object.png");
    objectForRope3->physics.bodyType = Physics::STATIC;
    objectForRope3->physics.Free();
    objectForRope3->physics.GenerateBody(GetPhysicsWorld(), &objectForRope3->transform);
    objectForRope3->SetPhysicsWorld(GetPhysicsWorld());
    objectForRope3->physics.ActiveGhostCollision(true);
    m3->AddObject(objectForRope3);

    // Constructing Rope Joint
    MyObject* objectForRope4 = new MyObject;
    objectForRope4->SetName("objectForRope4");
    objectForRope4->transform.position.Set(1950.f, 200.0f, 0.0f);
    objectForRope4->SetScale(b2Vec2{ 75, 400 });
    objectForRope4->SetSource("texture/Charge_Object.png");
    objectForRope4->physics.bodyType = Physics::STATIC;
    objectForRope4->physics.Free();
    objectForRope4->physics.GenerateBody(GetPhysicsWorld(), &objectForRope4->transform);
    objectForRope4->SetPhysicsWorld(GetPhysicsWorld());
    objectForRope4->physics.ActiveGhostCollision(true);
    m3->AddObject(objectForRope4);

    // Constructing Platform
    MyObject* pattern2Platform0 = new MyObject;
    pattern2Platform0->SetSource("texture/Highway_Tile_New.png");
    pattern2Platform0->SetPhysicsWorld(GetPhysicsWorld());
    pattern2Platform0->SetScale(b2Vec2{ 700.f, 320.f });
    pattern2Platform0->SetName("pattern2Platform0");
    pattern2Platform0->transform.position.Set(400.f, -400.f, 0.f);
    pattern2Platform0->transform.rotation = 0;
    m3->AddObject(pattern2Platform0);

    ScoreObject* Object_I_1 = new ScoreObject;
    Object_I_1->SetPhysicsWorld(GetPhysicsWorld());
    Object_I_1->SetScale(b2Vec2{ 50.f, 50.f });
    Object_I_1->SetSource("texture/Object_I_1.png");
    Object_I_1->SetName("Object_I_1");
    Object_I_1->transform.position.Set(100, 50.f, 0.f);
    Object_I_1->physics.bodyType = Physics::STATIC;
    Object_I_1->physics.Free();
    Object_I_1->physics.GenerateBody(GetPhysicsWorld(), &Object_I_1->transform);
    Object_I_1->physics.ActiveGhostCollision(true);
    m3->AddScoreObject(Object_I_1);

    // Constructing Platform
    MyObject* pattern2Platform2 = new MyObject;
    pattern2Platform2->SetSource("texture/Highway_Tile_New.png");
    pattern2Platform2->SetPhysicsWorld(GetPhysicsWorld());
    pattern2Platform2->SetScale(b2Vec2{ 450.f, 320.f });
    pattern2Platform2->SetName("pattern2Platform2");
    pattern2Platform2->transform.position.Set(1500.f, -400.f, 0.f);
    pattern2Platform2->transform.rotation = 0;
    m3->AddObject(pattern2Platform2);

    // Constructing Platform
    MyObject* pattern2Platform1 = new MyObject;
    pattern2Platform1->SetSource("texture/Highway_Tile_New.png");
    pattern2Platform1->SetPhysicsWorld(GetPhysicsWorld());
    pattern2Platform1->SetScale(b2Vec2{ 450.f, 320.f });
    pattern2Platform1->SetName("pattern2Platform1");
    pattern2Platform1->transform.position.Set(2500.f, -400.f, 0.f);
    pattern2Platform1->transform.rotation = 0;
    m3->AddObject(pattern2Platform1);


    // Coin
    ScoreObject* coin41 = new ScoreObject;
    coin41->SetPhysicsWorld(GetPhysicsWorld());
    coin41->SetScale(b2Vec2{ 50.f, 50.f });
    coin41->SetSource("texture/Score_Object.png");
    coin41->SetName("coin41");
    coin41->transform.position.Set(100.f, -60.f, 0.f);
    coin41->physics.bodyType = Physics::STATIC;
    coin41->physics.Free();
    coin41->physics.GenerateBody(GetPhysicsWorld(), &coin41->transform);
    coin41->physics.ActiveGhostCollision(true);
    m3->AddScoreObject(coin41);

    // Coin
    ScoreObject* coin42 = new ScoreObject;
    coin42->SetPhysicsWorld(GetPhysicsWorld());
    coin42->SetScale(b2Vec2{ 50.f, 50.f });
    coin42->SetSource("texture/Score_Object.png");
    coin42->SetName("coin42");
    coin42->transform.position.Set(200.f, -160.f, 0.f);
    coin42->physics.bodyType = Physics::STATIC;
    coin42->physics.Free();
    coin42->physics.GenerateBody(GetPhysicsWorld(), &coin42->transform);
    coin42->physics.ActiveGhostCollision(true);
    m3->AddScoreObject(coin42);

    // Coin
    ScoreObject* coin43 = new ScoreObject;
    coin43->SetPhysicsWorld(GetPhysicsWorld());
    coin43->SetScale(b2Vec2{ 50.f, 50.f });
    coin43->SetSource("texture/Score_Object.png");
    coin43->SetName("coin43");
    coin43->transform.position.Set(300.f, -160.f, 0.f);
    coin43->physics.bodyType = Physics::STATIC;
    coin43->physics.Free();
    coin43->physics.GenerateBody(GetPhysicsWorld(), &coin43->transform);
    coin43->physics.ActiveGhostCollision(true);
    m3->AddScoreObject(coin43);

    // Coin
    ScoreObject* coin44 = new ScoreObject;
    coin44->SetPhysicsWorld(GetPhysicsWorld());
    coin44->SetScale(b2Vec2{ 50.f, 50.f });
    coin44->SetSource("texture/Score_Object.png");
    coin44->SetName("coin44");
    coin44->transform.position.Set(400.f, -160.f, 0.f);
    coin44->physics.bodyType = Physics::STATIC;
    coin44->physics.Free();
    coin44->physics.GenerateBody(GetPhysicsWorld(), &coin44->transform);
    coin44->physics.ActiveGhostCollision(true);
    m3->AddScoreObject(coin44);

    // Coin
    ScoreObject* coin45 = new ScoreObject;
    coin45->SetPhysicsWorld(GetPhysicsWorld());
    coin45->SetScale(b2Vec2{ 50.f, 50.f });
    coin45->SetSource("texture/Score_Object.png");
    coin45->SetName("coin45");
    coin45->transform.position.Set(500.f, -160.f, 0.f);
    coin45->physics.bodyType = Physics::STATIC;
    coin45->physics.Free();
    coin45->physics.GenerateBody(GetPhysicsWorld(), &coin45->transform);
    coin45->physics.ActiveGhostCollision(true);
    m3->AddScoreObject(coin45);

    // Coin
    ScoreObject* coin46 = new ScoreObject;
    coin46->SetPhysicsWorld(GetPhysicsWorld());
    coin46->SetScale(b2Vec2{ 50.f, 50.f });
    coin46->SetSource("texture/Score_Object.png");
    coin46->SetName("coin46");
    coin46->transform.position.Set(600.f, -160.f, 0.f);
    coin46->physics.bodyType = Physics::STATIC;
    coin46->physics.Free();
    coin46->physics.GenerateBody(GetPhysicsWorld(), &coin46->transform);
    coin46->physics.ActiveGhostCollision(true);
    m3->AddScoreObject(coin46);

    // Coin
    ScoreObject* coin47 = new ScoreObject;
    coin47->SetPhysicsWorld(GetPhysicsWorld());
    coin47->SetScale(b2Vec2{ 50.f, 50.f });
    coin47->SetSource("texture/Score_Object.png");
    coin47->SetName("coin47");
    coin47->transform.position.Set(700.f, -40.f, 0.f);
    coin47->physics.bodyType = Physics::STATIC;
    coin47->physics.Free();
    coin47->physics.GenerateBody(GetPhysicsWorld(), &coin47->transform);
    coin47->physics.ActiveGhostCollision(true);
    m3->AddScoreObject(coin47);

    // Coin
    ScoreObject* coin48 = new ScoreObject;
    coin48->SetPhysicsWorld(GetPhysicsWorld());
    coin48->SetScale(b2Vec2{ 50.f, 50.f });
    coin48->SetSource("texture/Score_Object.png");
    coin48->SetName("coin48");
    coin48->transform.position.Set(800.f, 80.f, 0.f);
    coin48->physics.bodyType = Physics::STATIC;
    coin48->physics.Free();
    coin48->physics.GenerateBody(GetPhysicsWorld(), &coin48->transform);
    coin48->physics.ActiveGhostCollision(true);
    m3->AddScoreObject(coin48);

    // Coin
    ScoreObject* coin49 = new ScoreObject;
    coin49->SetPhysicsWorld(GetPhysicsWorld());
    coin49->SetScale(b2Vec2{ 50.f, 50.f });
    coin49->SetSource("texture/Score_Object.png");
    coin49->SetName("coin49");
    coin49->transform.position.Set(1200.f, 80.f, 0.f);
    coin49->physics.bodyType = Physics::STATIC;
    coin49->physics.Free();
    coin49->physics.GenerateBody(GetPhysicsWorld(), &coin49->transform);
    coin49->physics.ActiveGhostCollision(true);
    m3->AddScoreObject(coin49);

    // Coin
    ScoreObject* coin50 = new ScoreObject;
    coin50->SetPhysicsWorld(GetPhysicsWorld());
    coin50->SetScale(b2Vec2{ 50.f, 50.f });
    coin50->SetSource("texture/Score_Object.png");
    coin50->SetName("coin50");
    coin50->transform.position.Set(1300.f, -40.f, 0.f);
    coin50->physics.bodyType = Physics::STATIC;
    coin50->physics.Free();
    coin50->physics.GenerateBody(GetPhysicsWorld(), &coin50->transform);
    coin50->physics.ActiveGhostCollision(true);
    m3->AddScoreObject(coin50);

    // Coin
    ScoreObject* coin51 = new ScoreObject;
    coin51->SetPhysicsWorld(GetPhysicsWorld());
    coin51->SetScale(b2Vec2{ 50.f, 50.f });
    coin51->SetSource("texture/Score_Object.png");
    coin51->SetName("coin51");
    coin51->transform.position.Set(1400.f, -160.f, 0.f);
    coin51->physics.bodyType = Physics::STATIC;
    coin51->physics.Free();
    coin51->physics.GenerateBody(GetPhysicsWorld(), &coin51->transform);
    coin51->physics.ActiveGhostCollision(true);
    m3->AddScoreObject(coin51);

    // Coin
    ScoreObject* coin52 = new ScoreObject;
    coin52->SetPhysicsWorld(GetPhysicsWorld());
    coin52->SetScale(b2Vec2{ 50.f, 50.f });
    coin52->SetSource("texture/Score_Object.png");
    coin52->SetName("coin52");
    coin52->transform.position.Set(1500.f, -160.f, 0.f);
    coin52->physics.bodyType = Physics::STATIC;
    coin52->physics.Free();
    coin52->physics.GenerateBody(GetPhysicsWorld(), &coin52->transform);
    coin52->physics.ActiveGhostCollision(true);
    m3->AddScoreObject(coin52);

    // Coin
    ScoreObject* coin53 = new ScoreObject;
    coin53->SetPhysicsWorld(GetPhysicsWorld());
    coin53->SetScale(b2Vec2{ 50.f, 50.f });
    coin53->SetSource("texture/Score_Object.png");
    coin53->SetName("coin53");
    coin53->transform.position.Set(1600.f, -160.f, 0.f);
    coin53->physics.bodyType = Physics::STATIC;
    coin53->physics.Free();
    coin53->physics.GenerateBody(GetPhysicsWorld(), &coin53->transform);
    coin53->physics.ActiveGhostCollision(true);
    m3->AddScoreObject(coin53);

    // Coin
    ScoreObject* coin59 = new ScoreObject;
    coin59->SetPhysicsWorld(GetPhysicsWorld());
    coin59->SetScale(b2Vec2{ 50.f, 50.f });
    coin59->SetSource("texture/Score_Object.png");
    coin59->SetName("coin59");
    coin59->transform.position.Set(1700.f, -40.f, 0.f);
    coin59->physics.bodyType = Physics::STATIC;
    coin59->physics.Free();
    coin59->physics.GenerateBody(GetPhysicsWorld(), &coin59->transform);
    coin59->physics.ActiveGhostCollision(true);
    m3->AddScoreObject(coin59);

    // Coin
    ScoreObject* coin60 = new ScoreObject;
    coin60->SetPhysicsWorld(GetPhysicsWorld());
    coin60->SetScale(b2Vec2{ 50.f, 50.f });
    coin60->SetSource("texture/Score_Object.png");
    coin60->SetName("coin60");
    coin60->transform.position.Set(1800.f, 60.f, 0.f);
    coin60->physics.bodyType = Physics::STATIC;
    coin60->physics.Free();
    coin60->physics.GenerateBody(GetPhysicsWorld(), &coin60->transform);
    coin60->physics.ActiveGhostCollision(true);
    m3->AddScoreObject(coin60);

    // Coin
    ScoreObject* coin54 = new ScoreObject;
    coin54->SetPhysicsWorld(GetPhysicsWorld());
    coin54->SetScale(b2Vec2{ 50.f, 50.f });
    coin54->SetSource("texture/Score_Object.png");
    coin54->SetName("coin54");
    coin54->transform.position.Set(2200.f, 80.f, 0.f);
    coin54->physics.bodyType = Physics::STATIC;
    coin54->physics.Free();
    coin54->physics.GenerateBody(GetPhysicsWorld(), &coin54->transform);
    coin54->physics.ActiveGhostCollision(true);
    m3->AddScoreObject(coin54);

    // Coin
    ScoreObject* coin55 = new ScoreObject;
    coin55->SetPhysicsWorld(GetPhysicsWorld());
    coin55->SetScale(b2Vec2{ 50.f, 50.f });
    coin55->SetSource("texture/Score_Object.png");
    coin55->SetName("coin55");
    coin55->transform.position.Set(2300.f, -40.f, 0.f);
    coin55->physics.bodyType = Physics::STATIC;
    coin55->physics.Free();
    coin55->physics.GenerateBody(GetPhysicsWorld(), &coin55->transform);
    coin55->physics.ActiveGhostCollision(true);
    m3->AddScoreObject(coin55);

    // Coin
    ScoreObject* coin56 = new ScoreObject;
    coin56->SetPhysicsWorld(GetPhysicsWorld());
    coin56->SetScale(b2Vec2{ 50.f, 50.f });
    coin56->SetSource("texture/Score_Object.png");
    coin56->SetName("coin56");
    coin56->transform.position.Set(2400.f, -160.f, 0.f);
    coin56->physics.bodyType = Physics::STATIC;
    coin56->physics.Free();
    coin56->physics.GenerateBody(GetPhysicsWorld(), &coin56->transform);
    coin56->physics.ActiveGhostCollision(true);
    m3->AddScoreObject(coin56);

    // Coin
    ScoreObject* coin57 = new ScoreObject;
    coin57->SetPhysicsWorld(GetPhysicsWorld());
    coin57->SetScale(b2Vec2{ 50.f, 50.f });
    coin57->SetSource("texture/Score_Object.png");
    coin57->SetName("coin57");
    coin57->transform.position.Set(2500.f, -160.f, 0.f);
    coin57->physics.bodyType = Physics::STATIC;
    coin57->physics.Free();
    coin57->physics.GenerateBody(GetPhysicsWorld(), &coin57->transform);
    coin57->physics.ActiveGhostCollision(true);
    m3->AddScoreObject(coin57);

    // Coin
    ScoreObject* coin58 = new ScoreObject;
    coin58->SetPhysicsWorld(GetPhysicsWorld());
    coin58->SetScale(b2Vec2{ 50.f, 50.f });
    coin58->SetSource("texture/Score_Object.png");
    coin58->SetName("coin58");
    coin58->transform.position.Set(2600.f, -160.f, 0.f);
    coin58->physics.bodyType = Physics::STATIC;
    coin58->physics.Free();
    coin58->physics.GenerateBody(GetPhysicsWorld(), &coin58->transform);
    coin58->physics.ActiveGhostCollision(true);
    m3->AddScoreObject(coin58);

    mg.AddMap(m3);
}

void Level1::MapGeneratorInitDownTown()
{
    m_backgroundColor = CITY_GREY;
    background.sprite.Free();
    background.sprite.LoadImage("texture/City_Background.png", State::m_renderer);
    background.transform.SetScale(2000.f, 724.f);
    background.sprite.activeAnimation = true;
    background.sprite.SetFrame(2);
    background.sprite.SetSpeed(0.5f);
    AddObject(&background);
    // Map 1 with 2 paltforms and 2 



    // Map with One Rope Joint.
    Map* m1 = new Map;

    // Constructing Collision box for rope joint
    MyObject* collisionBox1 = new MyObject;
    collisionBox1->SetName("CollisionBox1");
    collisionBox1->transform.position.Set(450.f, 30.f, 0.f);
    collisionBox1->SetScale(b2Vec2{ 80.f, 160.f });
    collisionBox1->SetSource("texture/rect.png");
    collisionBox1->sprite.color.a = 0;
    collisionBox1->physics.bodyType = Physics::STATIC;
    collisionBox1->physics.Free();
    collisionBox1->physics.GenerateBody(GetPhysicsWorld(), &collisionBox1->transform);
    collisionBox1->SetPhysicsWorld(GetPhysicsWorld());
    collisionBox1->physics.ActiveGhostCollision(true);
    m1->AddObject(collisionBox1);
    m1->SetNumOfCollisionBox(1);


    // Constructing Rope Joint
    MyObject* objectForRope = new MyObject;
    objectForRope->SetName("objectForRope");
    objectForRope->transform.position.Set(700.f, 200.0f, 0.0f);
    objectForRope->SetScale(b2Vec2{ 75, 400 });
    objectForRope->SetSource("texture/Charge_Object.png");
    objectForRope->physics.bodyType = Physics::STATIC;
    objectForRope->physics.Free();
    objectForRope->physics.GenerateBody(GetPhysicsWorld(), &objectForRope->transform);
    objectForRope->SetPhysicsWorld(GetPhysicsWorld());
    objectForRope->physics.ActiveGhostCollision(true);
    m1->AddObject(objectForRope);

    // Constructing Platform
    MyObject* object1 = new MyObject;
    object1->SetSource("texture/City_Tile_New.png");
    object1->SetPhysicsWorld(GetPhysicsWorld());
    object1->transform.position.Set(collisionBox1->transform.position.x - 250.f, -400.f, 0.f);
    object1->SetScale(b2Vec2{ 600.f, 320.f });
    object1->SetName("test1");
    object1->transform.rotation = 0;
    m1->AddObject(object1);

    // Constructing ScoreObject


    ScoreObject* Object_G = new ScoreObject;
    Object_G->SetPhysicsWorld(GetPhysicsWorld());
    Object_G->SetScale(b2Vec2{ 50.f, 50.f });
    Object_G->SetSource("texture/Object_G.png");
    Object_G->SetName("Object_G");
    Object_G->transform.position.Set(700.f, 50.f, 0.f);
    Object_G->physics.bodyType = Physics::STATIC;
    Object_G->physics.Free();
    Object_G->physics.GenerateBody(GetPhysicsWorld(), &Object_G->transform);
    Object_G->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(Object_G);

    // Constructing Platform
    MyObject* object2 = new MyObject;
    object2->SetSource("texture/City_Tile_New.png");
    object2->transform.position.Set(1200.f, -400.f, 0.f);
    object2->SetScale(b2Vec2{ 600.f, 320.f });
    object2->SetName("test2");
    object2->physics.bodyType = Physics::STATIC;
    m1->AddObject(object2);

    // Coin
    ScoreObject* coin16 = new ScoreObject;
    coin16->SetPhysicsWorld(GetPhysicsWorld());
    coin16->SetScale(b2Vec2{ 50.f, 50.f });
    coin16->SetSource("texture/Score_Object.png");
    coin16->SetName("coin1");
    coin16->transform.position.Set(0.f, -100.f, 0.f);
    coin16->physics.bodyType = Physics::STATIC;
    coin16->physics.Free();
    coin16->physics.GenerateBody(GetPhysicsWorld(), &coin16->transform);
    coin16->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin16);

    // Coin
    ScoreObject* coin17 = new ScoreObject;
    coin17->SetPhysicsWorld(GetPhysicsWorld());
    coin17->SetScale(b2Vec2{ 50.f, 50.f });
    coin17->SetSource("texture/Score_Object.png");
    coin17->SetName("coin1");
    coin17->transform.position.Set(100.f, -160.f, 0.f);
    coin17->physics.bodyType = Physics::STATIC;
    coin17->physics.Free();
    coin17->physics.GenerateBody(GetPhysicsWorld(), &coin17->transform);
    coin17->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin17);

    // Coin
    ScoreObject* coin18 = new ScoreObject;
    coin18->SetPhysicsWorld(GetPhysicsWorld());
    coin18->SetScale(b2Vec2{ 50.f, 50.f });
    coin18->SetSource("texture/Score_Object.png");
    coin18->SetName("coin1");
    coin18->transform.position.Set(200.f, -160.f, 0.f);
    coin18->physics.bodyType = Physics::STATIC;
    coin18->physics.Free();
    coin18->physics.GenerateBody(GetPhysicsWorld(), &coin18->transform);
    coin18->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin18);

    // Coin
    ScoreObject* coin19 = new ScoreObject;
    coin19->SetPhysicsWorld(GetPhysicsWorld());
    coin19->SetScale(b2Vec2{ 50.f, 50.f });
    coin19->SetSource("texture/Score_Object.png");
    coin19->SetName("coin1");
    coin19->transform.position.Set(300.f, -60.f, 0.f);
    coin19->physics.bodyType = Physics::STATIC;
    coin19->physics.Free();
    coin19->physics.GenerateBody(GetPhysicsWorld(), &coin19->transform);
    coin19->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin19);

    // Coin
    ScoreObject* coin20 = new ScoreObject;
    coin20->SetPhysicsWorld(GetPhysicsWorld());
    coin20->SetScale(b2Vec2{ 50.f, 40.f });
    coin20->SetSource("texture/Score_Object.png");
    coin20->SetName("coin1");
    coin20->transform.position.Set(400.f, 40.f, 0.f);
    coin20->physics.bodyType = Physics::STATIC;
    coin20->physics.Free();
    coin20->physics.GenerateBody(GetPhysicsWorld(), &coin20->transform);
    coin20->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin20);

    // Coin
    ScoreObject* coin25 = new ScoreObject;
    coin25->SetPhysicsWorld(GetPhysicsWorld());
    coin25->SetScale(b2Vec2{ 50.f, 40.f });
    coin25->SetSource("texture/Score_Object.png");
    coin25->SetName("coin1");
    coin25->transform.position.Set(900.f, -60.f, 0.f);
    coin25->physics.bodyType = Physics::STATIC;
    coin25->physics.Free();
    coin25->physics.GenerateBody(GetPhysicsWorld(), &coin25->transform);
    coin25->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin25);

    // Coin
    ScoreObject* coin21 = new ScoreObject;
    coin21->SetPhysicsWorld(GetPhysicsWorld());
    coin21->SetScale(b2Vec2{ 50.f, 40.f });
    coin21->SetSource("texture/Object_G.png");
    coin21->SetName("coin1");
    coin21->transform.position.Set(1000.f, -160.f, 0.f);
    coin21->physics.bodyType = Physics::STATIC;
    coin21->physics.Free();
    coin21->physics.GenerateBody(GetPhysicsWorld(), &coin21->transform);
    coin21->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin21);

    // Coin
    ScoreObject* coin22 = new ScoreObject;
    coin22->SetPhysicsWorld(GetPhysicsWorld());
    coin22->SetScale(b2Vec2{ 50.f, 40.f });
    coin22->SetSource("texture/Score_Object.png");
    coin22->SetName("coin1");
    coin22->transform.position.Set(1100.f, -160.f, 0.f);
    coin22->physics.bodyType = Physics::STATIC;
    coin22->physics.Free();
    coin22->physics.GenerateBody(GetPhysicsWorld(), &coin22->transform);
    coin22->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin22);

    // Coin
    ScoreObject* coin23 = new ScoreObject;
    coin23->SetPhysicsWorld(GetPhysicsWorld());
    coin23->SetScale(b2Vec2{ 50.f, 40.f });
    coin23->SetSource("texture/Score_Object.png");
    coin23->SetName("coin1");
    coin23->transform.position.Set(1200.f, -160.f, 0.f);
    coin23->physics.bodyType = Physics::STATIC;
    coin23->physics.Free();
    coin23->physics.GenerateBody(GetPhysicsWorld(), &coin23->transform);
    coin23->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin23);

    // Coin
    ScoreObject* coin24 = new ScoreObject;
    coin24->SetPhysicsWorld(GetPhysicsWorld());
    coin24->SetScale(b2Vec2{ 50.f, 40.f });
    coin24->SetSource("texture/Score_Object.png");
    coin24->SetName("coin1");
    coin24->transform.position.Set(1300.f, -160.f, 0.f);
    coin24->physics.bodyType = Physics::STATIC;
    coin24->physics.Free();
    coin24->physics.GenerateBody(GetPhysicsWorld(), &coin24->transform);
    coin24->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin24);

    mg.AddMap(m1);

    // Second Map
    Map* m2 = new Map;

    ScoreObject* Object_E = new ScoreObject;
    Object_E->SetPhysicsWorld(GetPhysicsWorld());
    Object_E->SetScale(b2Vec2{ 50.f, 50.f });
    Object_E->SetSource("texture/Object_E.png");
    Object_E->SetName("Object_E");
    Object_E->transform.position.Set(600.f, 50.f, 0.f);
    Object_E->physics.bodyType = Physics::STATIC;
    Object_E->physics.Free();
    Object_E->physics.GenerateBody(GetPhysicsWorld(), &Object_E->transform);
    Object_E->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(Object_E);

    // Why distance changed so dynamically? I should find the reasons why.
    MyObject* normalPlatform = new MyObject;
    normalPlatform->SetPhysicsWorld(GetPhysicsWorld());
    normalPlatform->SetSource("texture/City_Tile_New2.png");
    normalPlatform->SetScale(b2Vec2{ 1200.f, 320.f });
    normalPlatform->SetName("Normal Platform");
    normalPlatform->transform.position.Set(600.f, -400.f, 0.f);
    normalPlatform->physics.bodyType = Physics::STATIC;
    m2->AddObject(normalPlatform);

    // Coin
    ScoreObject* coin2 = new ScoreObject;
    coin2->SetPhysicsWorld(GetPhysicsWorld());
    coin2->SetScale(b2Vec2{ 50.f, 50.f });
    coin2->SetSource("texture/Score_Object.png");
    coin2->SetName("coin1");
    coin2->transform.position.Set(900.f, -160.f, 0.f);
    coin2->physics.bodyType = Physics::STATIC;
    coin2->physics.Free();
    coin2->physics.GenerateBody(GetPhysicsWorld(), &coin2->transform);
    coin2->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin2);

    // Coin
    ScoreObject* coin13 = new ScoreObject;
    coin13->SetPhysicsWorld(GetPhysicsWorld());
    coin13->SetScale(b2Vec2{ 50.f, 50.f });
    coin13->SetSource("texture/Score_Object.png");
    coin13->SetName("coin1");
    coin13->transform.position.Set(1300.f, 0.f, 0.f);
    coin13->physics.bodyType = Physics::STATIC;
    coin13->physics.Free();
    coin13->physics.GenerateBody(GetPhysicsWorld(), &coin13->transform);
    coin13->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin13);

    // Coin
    ScoreObject* coin12 = new ScoreObject;
    coin12->SetPhysicsWorld(GetPhysicsWorld());
    coin12->SetScale(b2Vec2{ 50.f, 50.f });
    coin12->SetSource("texture/Score_Object.png");
    coin12->SetName("coin1");
    coin12->transform.position.Set(1200.f, -100.f, 0.f);
    coin12->physics.bodyType = Physics::STATIC;
    coin12->physics.Free();
    coin12->physics.GenerateBody(GetPhysicsWorld(), &coin12->transform);
    coin12->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin12);

    // Coin
    ScoreObject* coin4 = new ScoreObject;
    coin4->SetPhysicsWorld(GetPhysicsWorld());
    coin4->SetScale(b2Vec2{ 50.f, 50.f });
    coin4->SetSource("texture/Score_Object.png");
    coin4->SetName("coin1");
    coin4->transform.position.Set(1100.f, -160.f, 0.f);
    coin4->physics.bodyType = Physics::STATIC;
    coin4->physics.Free();
    coin4->physics.GenerateBody(GetPhysicsWorld(), &coin4->transform);
    coin4->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin4);

    // Coin
    ScoreObject* coin3 = new ScoreObject;
    coin3->SetPhysicsWorld(GetPhysicsWorld());
    coin3->SetScale(b2Vec2{ 50.f, 50.f });
    coin3->SetSource("texture/Score_Object.png");
    coin3->SetName("coin1");
    coin3->transform.position.Set(1000.f, -160.f, 0.f);
    coin3->physics.bodyType = Physics::STATIC;
    coin3->physics.Free();
    coin3->physics.GenerateBody(GetPhysicsWorld(), &coin3->transform);
    coin3->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin3);

    // Coin
    ScoreObject* coin1 = new ScoreObject;
    coin1->SetPhysicsWorld(GetPhysicsWorld());
    coin1->SetScale(b2Vec2{ 50.f, 50.f });
    coin1->SetSource("texture/Score_Object.png");
    coin1->SetName("coin1");
    coin1->transform.position.Set(800.f, -130.f, 0.f);
    coin1->physics.bodyType = Physics::STATIC;
    coin1->physics.Free();
    coin1->physics.GenerateBody(GetPhysicsWorld(), &coin1->transform);
    coin1->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin1);

    // Coin
    ScoreObject* coin5 = new ScoreObject;
    coin5->SetPhysicsWorld(GetPhysicsWorld());
    coin5->SetScale(b2Vec2{ 50.f, 50.f });
    coin5->SetSource("texture/Score_Object.png");
    coin5->SetName("coin1");
    coin5->transform.position.Set(700.f, -30.f, 0.f);
    coin5->physics.bodyType = Physics::STATIC;
    coin5->physics.Free();
    coin5->physics.GenerateBody(GetPhysicsWorld(), &coin5->transform);
    coin5->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin5);

    // Coin
    ScoreObject* coin6 = new ScoreObject;
    coin6->SetPhysicsWorld(GetPhysicsWorld());
    coin6->SetScale(b2Vec2{ 50.f, 50.f });
    coin6->SetSource("texture/Score_Object.png");
    coin6->SetName("coin1");
    coin6->transform.position.Set(500.f, -30.f, 0.f);
    coin6->physics.bodyType = Physics::STATIC;
    coin6->physics.Free();
    coin6->physics.GenerateBody(GetPhysicsWorld(), &coin6->transform);
    coin6->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin6);

    // Coin
    ScoreObject* coin8 = new ScoreObject;
    coin8->SetPhysicsWorld(GetPhysicsWorld());
    coin8->SetScale(b2Vec2{ 50.f, 50.f });
    coin8->SetSource("texture/Score_Object.png");
    coin8->SetName("coin1");
    coin8->transform.position.Set(400.f, -130.f, 0.f);
    coin8->physics.bodyType = Physics::STATIC;
    coin8->physics.Free();
    coin8->physics.GenerateBody(GetPhysicsWorld(), &coin8->transform);
    coin8->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin8);

    // Coin
    ScoreObject* coin9 = new ScoreObject;
    coin9->SetPhysicsWorld(GetPhysicsWorld());
    coin9->SetScale(b2Vec2{ 50.f, 50.f });
    coin9->SetSource("texture/Score_Object.png");
    coin9->SetName("coin1");
    coin9->transform.position.Set(300.f, -160.f, 0.f);
    coin9->physics.bodyType = Physics::STATIC;
    coin9->physics.Free();
    coin9->physics.GenerateBody(GetPhysicsWorld(), &coin9->transform);
    coin9->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin9);

    // Coin
    ScoreObject* coin10 = new ScoreObject;
    coin10->SetPhysicsWorld(GetPhysicsWorld());
    coin10->SetScale(b2Vec2{ 50.f, 50.f });
    coin10->SetSource("texture/Score_Object.png");
    coin10->SetName("coin1");
    coin10->transform.position.Set(200.f, -160.f, 0.f);
    coin10->physics.bodyType = Physics::STATIC;
    coin10->physics.Free();
    coin10->physics.GenerateBody(GetPhysicsWorld(), &coin10->transform);
    coin10->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin10);

    // Coin
    ScoreObject* coin11 = new ScoreObject;
    coin11->SetPhysicsWorld(GetPhysicsWorld());
    coin11->SetScale(b2Vec2{ 50.f, 50.f });
    coin11->SetSource("texture/Score_Object.png");
    coin11->SetName("coin1");
    coin11->transform.position.Set(100.f, -160.f, 0.f);
    coin11->physics.bodyType = Physics::STATIC;
    coin11->physics.Free();
    coin11->physics.GenerateBody(GetPhysicsWorld(), &coin11->transform);
    coin11->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin11);

    m2->ToggleRopeAvailable();

    mg.AddMap(m2);

}

void Level1::MapGeneratorInitCountryRoad()
{
    m_backgroundColor = BLACK;
    background.sprite.Free();
    background.sprite.LoadImage("texture/CountryBackGround.png", State::m_renderer);
    background.transform.SetScale(2000.f, 724.f);
    background.sprite.activeAnimation = true;
    background.sprite.SetFrame(5);
    background.sprite.SetSpeed(2.f);
    AddObject(&background);

    // Map 2 with 1 paltforms.
    Map* m2 = new Map;

    // Constructing Platform
    MyObject* pattern1Platform0 = new MyObject;
    pattern1Platform0->SetSource("texture/Country_Tile_New.png");
    pattern1Platform0->SetPhysicsWorld(GetPhysicsWorld());
    pattern1Platform0->SetScale(b2Vec2{ 600.f, 320.f });
    pattern1Platform0->SetName("pattern1Platform0");
    pattern1Platform0->transform.position.Set(300.f, -400.f, 0.f);
    pattern1Platform0->transform.rotation = 0;
    m2->AddObject(pattern1Platform0);

    ScoreObject* Object_I_2 = new ScoreObject;
    Object_I_2->SetPhysicsWorld(GetPhysicsWorld());
    Object_I_2->SetScale(b2Vec2{ 50.f, 50.f });
    Object_I_2->SetSource("texture/Object_I_2.png");
    Object_I_2->SetName("Object_I_2");
    Object_I_2->transform.position.Set(50, 50.f, 0.f);
    Object_I_2->physics.bodyType = Physics::STATIC;
    Object_I_2->physics.Free();
    Object_I_2->physics.GenerateBody(GetPhysicsWorld(), &Object_I_2->transform);
    Object_I_2->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(Object_I_2);

    // Constructing Platform
    MyObject* pattern1Platform1 = new MyObject;
    pattern1Platform1->SetSource("texture/Country_Tile_New.png");
    pattern1Platform1->SetPhysicsWorld(GetPhysicsWorld());
    pattern1Platform1->SetScale(b2Vec2{ 600.f, 320.f });
    pattern1Platform1->SetName("pattern3Platform1");
    pattern1Platform1->transform.position.Set(1200.f, -400.f, 0.f);
    pattern1Platform1->transform.rotation = 0;
    m2->AddObject(pattern1Platform1);
    m2->ToggleRopeAvailable();

    // Coin
    ScoreObject* coin1 = new ScoreObject;
    coin1->SetPhysicsWorld(GetPhysicsWorld());
    coin1->SetScale(b2Vec2{ 50.f, 50.f });
    coin1->SetSource("texture/Score_Object.png");
    coin1->SetName("coin1");
    coin1->transform.position.Set(100.f, -160.f, 0.f);
    coin1->physics.bodyType = Physics::STATIC;
    coin1->physics.Free();
    coin1->physics.GenerateBody(GetPhysicsWorld(), &coin1->transform);
    coin1->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin1);

    // Coin
    ScoreObject* coin2 = new ScoreObject;
    coin2->SetPhysicsWorld(GetPhysicsWorld());
    coin2->SetScale(b2Vec2{ 50.f, 50.f });
    coin2->SetSource("texture/Score_Object.png");
    coin2->SetName("coin2");
    coin2->transform.position.Set(200.f, -160.f, 0.f);
    coin2->physics.bodyType = Physics::STATIC;
    coin2->physics.Free();
    coin2->physics.GenerateBody(GetPhysicsWorld(), &coin2->transform);
    coin2->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin2);

    // Coin
    ScoreObject* coin3 = new ScoreObject;
    coin3->SetPhysicsWorld(GetPhysicsWorld());
    coin3->SetScale(b2Vec2{ 50.f, 50.f });
    coin3->SetSource("texture/Score_Object.png");
    coin3->SetName("coin3");
    coin3->transform.position.Set(300.f, -160.f, 0.f);
    coin3->physics.bodyType = Physics::STATIC;
    coin3->physics.Free();
    coin3->physics.GenerateBody(GetPhysicsWorld(), &coin3->transform);
    coin3->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin3);

    // Coin
    ScoreObject* coin4 = new ScoreObject;
    coin4->SetPhysicsWorld(GetPhysicsWorld());
    coin4->SetScale(b2Vec2{ 50.f, 50.f });
    coin4->SetSource("texture/Score_Object.png");
    coin4->SetName("coin4");
    coin4->transform.position.Set(400.f, -160.f, 0.f);
    coin4->physics.bodyType = Physics::STATIC;
    coin4->physics.Free();
    coin4->physics.GenerateBody(GetPhysicsWorld(), &coin4->transform);
    coin4->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin4);

    // Coin
    ScoreObject* coin5 = new ScoreObject;
    coin5->SetPhysicsWorld(GetPhysicsWorld());
    coin5->SetScale(b2Vec2{ 50.f, 50.f });
    coin5->SetSource("texture/Score_Object.png");
    coin5->SetName("coin5");
    coin5->transform.position.Set(500.f, -160.f, 0.f);
    coin5->physics.bodyType = Physics::STATIC;
    coin5->physics.Free();
    coin5->physics.GenerateBody(GetPhysicsWorld(), &coin5->transform);
    coin5->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin5);

    // Coin
    ScoreObject* coin6 = new ScoreObject;
    coin6->SetPhysicsWorld(GetPhysicsWorld());
    coin6->SetScale(b2Vec2{ 50.f, 50.f });
    coin6->SetSource("texture/Score_Object.png");
    coin6->SetName("coin6");
    coin6->transform.position.Set(600.f, -40.f, 0.f);
    coin6->physics.bodyType = Physics::STATIC;
    coin6->physics.Free();
    coin6->physics.GenerateBody(GetPhysicsWorld(), &coin6->transform);
    coin6->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin6);

    // Coin
    ScoreObject* coin7 = new ScoreObject;
    coin7->SetPhysicsWorld(GetPhysicsWorld());
    coin7->SetScale(b2Vec2{ 50.f, 50.f });
    coin7->SetSource("texture/Score_Object.png");
    coin7->SetName("coin7");
    coin7->transform.position.Set(700.f, 80.f, 0.f);
    coin7->physics.bodyType = Physics::STATIC;
    coin7->physics.Free();
    coin7->physics.GenerateBody(GetPhysicsWorld(), &coin7->transform);
    coin7->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin7);

    // Coin
    ScoreObject* coin8 = new ScoreObject;
    coin8->SetPhysicsWorld(GetPhysicsWorld());
    coin8->SetScale(b2Vec2{ 50.f, 50.f });
    coin8->SetSource("texture/Score_Object.png");
    coin8->SetName("coin8");
    coin8->transform.position.Set(800.f, 120.f, 0.f);
    coin8->physics.bodyType = Physics::STATIC;
    coin8->physics.Free();
    coin8->physics.GenerateBody(GetPhysicsWorld(), &coin8->transform);
    coin8->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin8);

    // Coin
    ScoreObject* coin9 = new ScoreObject;
    coin9->SetPhysicsWorld(GetPhysicsWorld());
    coin9->SetScale(b2Vec2{ 50.f, 50.f });
    coin9->SetSource("texture/Score_Object.png");
    coin9->SetName("coin9");
    coin9->transform.position.Set(900.f, 80.f, 0.f);
    coin9->physics.bodyType = Physics::STATIC;
    coin9->physics.Free();
    coin9->physics.GenerateBody(GetPhysicsWorld(), &coin9->transform);
    coin9->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin9);

    // Coin
    ScoreObject* coin10 = new ScoreObject;
    coin10->SetPhysicsWorld(GetPhysicsWorld());
    coin10->SetScale(b2Vec2{ 50.f, 50.f });
    coin10->SetSource("texture/Score_Object.png");
    coin10->SetName("coin10");
    coin10->transform.position.Set(1000.f, -40.f, 0.f);
    coin10->physics.bodyType = Physics::STATIC;
    coin10->physics.Free();
    coin10->physics.GenerateBody(GetPhysicsWorld(), &coin10->transform);
    coin10->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin10);

    // Coin
    ScoreObject* coin11 = new ScoreObject;
    coin11->SetPhysicsWorld(GetPhysicsWorld());
    coin11->SetScale(b2Vec2{ 50.f, 50.f });
    coin11->SetSource("texture/Score_Object.png");
    coin11->SetName("coin11");
    coin11->transform.position.Set(1100.f, -160.f, 0.f);
    coin11->physics.bodyType = Physics::STATIC;
    coin11->physics.Free();
    coin11->physics.GenerateBody(GetPhysicsWorld(), &coin11->transform);
    coin11->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin11);

    // Coin
    ScoreObject* coin12 = new ScoreObject;
    coin12->SetPhysicsWorld(GetPhysicsWorld());
    coin12->SetScale(b2Vec2{ 50.f, 50.f });
    coin12->SetSource("texture/Score_Object.png");
    coin12->SetName("coin12");
    coin12->transform.position.Set(1200.f, -160.f, 0.f);
    coin12->physics.bodyType = Physics::STATIC;
    coin12->physics.Free();
    coin12->physics.GenerateBody(GetPhysicsWorld(), &coin12->transform);
    coin12->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin12);

    // Coin
    ScoreObject* coin13 = new ScoreObject;
    coin13->SetPhysicsWorld(GetPhysicsWorld());
    coin13->SetScale(b2Vec2{ 50.f, 50.f });
    coin13->SetSource("texture/Score_Object.png");
    coin13->SetName("coin13");
    coin13->transform.position.Set(1300.f, -160.f, 0.f);
    coin13->physics.bodyType = Physics::STATIC;
    coin13->physics.Free();
    coin13->physics.GenerateBody(GetPhysicsWorld(), &coin13->transform);
    coin13->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin13);

    // Coin
    ScoreObject* coin14 = new ScoreObject;
    coin14->SetPhysicsWorld(GetPhysicsWorld());
    coin14->SetScale(b2Vec2{ 50.f, 50.f });
    coin14->SetSource("texture/Score_Object.png");
    coin14->SetName("coin14");
    coin14->transform.position.Set(1400.f, -160.f, 0.f);
    coin14->physics.bodyType = Physics::STATIC;
    coin14->physics.Free();
    coin14->physics.GenerateBody(GetPhysicsWorld(), &coin14->transform);
    coin14->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin14);

    // Coin
    ScoreObject* coin15 = new ScoreObject;
    coin15->SetPhysicsWorld(GetPhysicsWorld());
    coin15->SetScale(b2Vec2{ 50.f, 50.f });
    coin15->SetSource("texture/Score_Object.png");
    coin15->SetName("coin15");
    coin15->transform.position.Set(1500.f, -40.f, 0.f);
    coin15->physics.bodyType = Physics::STATIC;
    coin15->physics.Free();
    coin15->physics.GenerateBody(GetPhysicsWorld(), &coin15->transform);
    coin15->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin15);

    // Coin
    ScoreObject* coin16 = new ScoreObject;
    coin16->SetPhysicsWorld(GetPhysicsWorld());
    coin16->SetScale(b2Vec2{ 50.f, 50.f });
    coin16->SetSource("texture/Score_Object.png");
    coin16->SetName("coin16");
    coin16->transform.position.Set(1600.f, 80.f, 0.f);
    coin16->physics.bodyType = Physics::STATIC;
    coin16->physics.Free();
    coin16->physics.GenerateBody(GetPhysicsWorld(), &coin16->transform);
    coin16->physics.ActiveGhostCollision(true);
    m2->AddScoreObject(coin16);

    mg.AddMap(m2);

    // Map 1 with 2 paltforms and 4 rope joint boxes.
    Map* m1 = new Map;

    // Constructing Collision box for rope joint
    MyObject* collisionBox0 = new MyObject;
    collisionBox0->SetName("CollisionBox0");
    collisionBox0->transform.position.Set(550.f, 30.f, 0.f);
    collisionBox0->transform.SetScale(b2Vec2{ 80.f, 160.f });
    collisionBox0->SetScale(b2Vec2{ 80.f, 80.f });
    collisionBox0->sprite.color.a = 0;
    collisionBox0->SetSource("texture/rect.png");

    collisionBox0->physics.bodyType = Physics::STATIC;
    collisionBox0->physics.Free();
    collisionBox0->physics.GenerateBody(GetPhysicsWorld(), &collisionBox0->transform);
    collisionBox0->SetPhysicsWorld(GetPhysicsWorld());
    collisionBox0->physics.ActiveGhostCollision(true);
    m1->AddObject(collisionBox0);

    // Constructing Collision box for rope joint
    MyObject* collisionBox1 = new MyObject;
    collisionBox1->SetName("CollisionBox1");
    collisionBox1->transform.position.Set(1200.f, 30.f, 0.f);
    collisionBox1->transform.SetScale(b2Vec2{ 80.f, 160.f });
    collisionBox1->SetScale(b2Vec2{ 80.f, 80.f });
    collisionBox1->SetSource("texture/rect.png");
    collisionBox1->sprite.color.a = 0;

    collisionBox1->physics.bodyType = Physics::STATIC;
    collisionBox1->physics.Free();
    collisionBox1->physics.GenerateBody(GetPhysicsWorld(), &collisionBox1->transform);
    collisionBox1->SetPhysicsWorld(GetPhysicsWorld());
    collisionBox1->physics.ActiveGhostCollision(true);
    m1->AddObject(collisionBox1);

    // Constructing Collision box for rope joint
    MyObject* collisionBox2 = new MyObject;
    collisionBox2->SetName("CollisionBox2");
    collisionBox2->transform.position.Set(1850.f, 30.f, 0.f);
    collisionBox2->transform.SetScale(b2Vec2{ 80.f, 160.f });
    collisionBox2->SetScale(b2Vec2{ 80.f, 80.f });
    collisionBox2->SetSource("texture/rect.png");
    collisionBox2->sprite.color.a = 0;

    collisionBox2->physics.bodyType = Physics::STATIC;
    collisionBox2->physics.Free();
    collisionBox2->physics.GenerateBody(GetPhysicsWorld(), &collisionBox2->transform);
    collisionBox2->SetPhysicsWorld(GetPhysicsWorld());
    collisionBox2->physics.ActiveGhostCollision(true);
    m1->AddObject(collisionBox2);

    // Constructing Collision box for rope joint
    MyObject* collisionBox3 = new MyObject;
    collisionBox3->SetName("CollisionBox3");
    collisionBox3->transform.position.Set(2500.f, 30.f, 0.f);
    collisionBox3->transform.SetScale(b2Vec2{ 80.f, 160.f });
    collisionBox3->SetScale(b2Vec2{ 80.f, 80.f });
    collisionBox3->SetSource("texture/rect.png");
    collisionBox3->sprite.color.a = 0;

    collisionBox3->physics.bodyType = Physics::STATIC;
    collisionBox3->physics.Free();
    collisionBox3->physics.GenerateBody(GetPhysicsWorld(), &collisionBox3->transform);
    collisionBox3->SetPhysicsWorld(GetPhysicsWorld());
    collisionBox3->physics.ActiveGhostCollision(true);
    m1->AddObject(collisionBox3);
    m1->SetNumOfCollisionBox(4);

    // Constructing Rope Joint
    MyObject* objectForRope0 = new MyObject;
    objectForRope0->SetName("objectForRope0");
    objectForRope0->transform.position.Set(800.f, 200.f, 0.f);
    objectForRope0->SetScale(b2Vec2{ 75, 400 });
    objectForRope0->SetSource("texture/Charge_Object.png");
    objectForRope0->physics.bodyType = Physics::STATIC;
    objectForRope0->physics.Free();
    objectForRope0->physics.GenerateBody(GetPhysicsWorld(), &objectForRope0->transform);
    objectForRope0->SetPhysicsWorld(GetPhysicsWorld());
    objectForRope0->physics.ActiveGhostCollision(true);
    m1->AddObject(objectForRope0);

    // Constructing Rope Joint
    MyObject* objectForRope1 = new MyObject;
    objectForRope1->SetName("objectForRope1");
    objectForRope1->transform.position.Set(1450.f, 200.f, 0.f);
    objectForRope1->SetScale(b2Vec2{ 75, 400 });
    objectForRope1->SetSource("texture/Charge_Object.png");
    objectForRope1->physics.bodyType = Physics::STATIC;
    objectForRope1->physics.Free();
    objectForRope1->physics.GenerateBody(GetPhysicsWorld(), &objectForRope1->transform);
    objectForRope1->SetPhysicsWorld(GetPhysicsWorld());
    objectForRope1->physics.ActiveGhostCollision(true);
    m1->AddObject(objectForRope1);

    // Constructing Rope Joint
    MyObject* objectForRope2 = new MyObject;
    objectForRope2->SetName("objectForRope2");
    objectForRope2->transform.position.Set(2100.f, 200.f, 0.f);
    objectForRope2->SetScale(b2Vec2{ 75, 400 });
    objectForRope2->SetSource("texture/Charge_Object.png");
    objectForRope2->physics.bodyType = Physics::STATIC;
    objectForRope2->physics.Free();
    objectForRope2->physics.GenerateBody(GetPhysicsWorld(), &objectForRope2->transform);
    objectForRope2->SetPhysicsWorld(GetPhysicsWorld());
    objectForRope2->physics.ActiveGhostCollision(true);
    m1->AddObject(objectForRope2);


    // Constructing Rope Joint
    MyObject* objectForRope3 = new MyObject;
    objectForRope3->SetName("objectForRope3");
    objectForRope3->transform.position.Set(2750.f, 200.f, 0.f);
    objectForRope3->SetScale(b2Vec2{ 75, 400 });
    objectForRope3->SetSource("texture/Charge_Object.png");
    objectForRope3->physics.bodyType = Physics::STATIC;
    objectForRope3->physics.Free();
    objectForRope3->physics.GenerateBody(GetPhysicsWorld(), &objectForRope3->transform);
    objectForRope3->SetPhysicsWorld(GetPhysicsWorld());
    objectForRope3->physics.ActiveGhostCollision(true);
    m1->AddObject(objectForRope3);

    ScoreObject* Object_N = new ScoreObject;
    Object_N->SetPhysicsWorld(GetPhysicsWorld());
    Object_N->SetScale(b2Vec2{ 50.f, 50.f });
    Object_N->SetSource("texture/Object_N.png");
    Object_N->SetName("Object_N");
    Object_N->transform.position.Set(collisionBox1->transform.position.x, collisionBox1->transform.position.y, collisionBox1->transform.position.z);
    Object_N->physics.bodyType = Physics::STATIC;
    Object_N->physics.Free();
    Object_N->physics.GenerateBody(GetPhysicsWorld(), &Object_N->transform);
    Object_N->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(Object_N);

    // Constructing Platform
    MyObject* pattern0Platform0 = new MyObject;
    pattern0Platform0->SetSource("texture/Country_Tile_New.png");
    pattern0Platform0->SetPhysicsWorld(GetPhysicsWorld());
    pattern0Platform0->SetScale(b2Vec2{ 600.f, 320.f });
    pattern0Platform0->SetName("pattern0Platform0");
    pattern0Platform0->transform.position.Set(300.f, -400.f, 0.f);
    pattern0Platform0->transform.rotation = 0;
    m1->AddObject(pattern0Platform0);

    // Constructing Platform
    MyObject* pattern0Platform1 = new MyObject;
    pattern0Platform1->SetSource("texture/Country_Tile_New.png");
    pattern0Platform1->SetPhysicsWorld(GetPhysicsWorld());
    pattern0Platform1->SetScale(b2Vec2{ 600.f, 320.f });
    pattern0Platform1->SetName("pattern0Platform1");
    pattern0Platform1->transform.position.Set(3200.f, -400.f, 0.f);
    pattern0Platform1->transform.rotation = 0;
    m1->AddObject(pattern0Platform1);

    // Coin
    ScoreObject* coin17 = new ScoreObject;
    coin17->SetPhysicsWorld(GetPhysicsWorld());
    coin17->SetScale(b2Vec2{ 50.f, 50.f });
    coin17->SetSource("texture/Score_Object.png");
    coin17->SetName("coin17");
    coin17->transform.position.Set(100.f, -160.f, 0.f);
    coin17->physics.bodyType = Physics::STATIC;
    coin17->physics.Free();
    coin17->physics.GenerateBody(GetPhysicsWorld(), &coin17->transform);
    coin17->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin17);

    // Coin
    ScoreObject* coin18 = new ScoreObject;
    coin18->SetPhysicsWorld(GetPhysicsWorld());
    coin18->SetScale(b2Vec2{ 50.f, 50.f });
    coin18->SetSource("texture/Score_Object.png");
    coin18->SetName("coin18");
    coin18->transform.position.Set(200.f, -160.f, 0.f);
    coin18->physics.bodyType = Physics::STATIC;
    coin18->physics.Free();
    coin18->physics.GenerateBody(GetPhysicsWorld(), &coin18->transform);
    coin18->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin18);

    // Coin
    ScoreObject* coin19 = new ScoreObject;
    coin19->SetPhysicsWorld(GetPhysicsWorld());
    coin19->SetScale(b2Vec2{ 50.f, 50.f });
    coin19->SetSource("texture/Score_Object.png");
    coin19->SetName("coin19");
    coin19->transform.position.Set(300.f, -160.f, 0.f);
    coin19->physics.bodyType = Physics::STATIC;
    coin19->physics.Free();
    coin19->physics.GenerateBody(GetPhysicsWorld(), &coin19->transform);
    coin19->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin19);

    // Coin
    ScoreObject* coin20 = new ScoreObject;
    coin20->SetPhysicsWorld(GetPhysicsWorld());
    coin20->SetScale(b2Vec2{ 50.f, 50.f });
    coin20->SetSource("texture/Score_Object.png");
    coin20->SetName("coin20");
    coin20->transform.position.Set(400.f, -160.f, 0.f);
    coin20->physics.bodyType = Physics::STATIC;
    coin20->physics.Free();
    coin20->physics.GenerateBody(GetPhysicsWorld(), &coin20->transform);
    coin20->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin20);

    // Coin
    ScoreObject* coin21 = new ScoreObject;
    coin21->SetPhysicsWorld(GetPhysicsWorld());
    coin21->SetScale(b2Vec2{ 50.f, 50.f });
    coin21->SetSource("texture/Score_Object.png");
    coin21->SetName("coin21");
    coin21->transform.position.Set(500.f, -40.f, 0.f);
    coin21->physics.bodyType = Physics::STATIC;
    coin21->physics.Free();
    coin21->physics.GenerateBody(GetPhysicsWorld(), &coin21->transform);
    coin21->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin21);

    // Coin
    ScoreObject* coin22 = new ScoreObject;
    coin22->SetPhysicsWorld(GetPhysicsWorld());
    coin22->SetScale(b2Vec2{ 50.f, 50.f });
    coin22->SetSource("texture/Score_Object.png");
    coin22->SetName("coin22");
    coin22->transform.position.Set(600.f, 60.f, 0.f);
    coin22->physics.bodyType = Physics::STATIC;
    coin22->physics.Free();
    coin22->physics.GenerateBody(GetPhysicsWorld(), &coin22->transform);
    coin22->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin22);

    // Coin
    ScoreObject* coin23 = new ScoreObject;
    coin23->SetPhysicsWorld(GetPhysicsWorld());
    coin23->SetScale(b2Vec2{ 50.f, 50.f });
    coin23->SetSource("texture/Score_Object.png");
    coin23->SetName("coin23");
    coin23->transform.position.Set(700.f, 80.f, 0.f);
    coin23->physics.bodyType = Physics::STATIC;
    coin23->physics.Free();
    coin23->physics.GenerateBody(GetPhysicsWorld(), &coin23->transform);
    coin23->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin23);

    // Coin
    ScoreObject* coin36 = new ScoreObject;
    coin36->SetPhysicsWorld(GetPhysicsWorld());
    coin36->SetScale(b2Vec2{ 50.f, 50.f });
    coin36->SetSource("texture/Score_Object.png");
    coin36->SetName("coin36");
    coin36->transform.position.Set(850.f, 50.f, 0.f);
    coin36->physics.bodyType = Physics::STATIC;
    coin36->physics.Free();
    coin36->physics.GenerateBody(GetPhysicsWorld(), &coin36->transform);
    coin36->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin36);

    // Coin
    ScoreObject* coin35 = new ScoreObject;
    coin35->SetPhysicsWorld(GetPhysicsWorld());
    coin35->SetScale(b2Vec2{ 50.f, 50.f });
    coin35->SetSource("texture/Score_Object.png");
    coin35->SetName("coin35");
    coin35->transform.position.Set(950.f, 40.f, 0.f);
    coin35->physics.bodyType = Physics::STATIC;
    coin35->physics.Free();
    coin35->physics.GenerateBody(GetPhysicsWorld(), &coin35->transform);
    coin35->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin35);

    // Coin
    ScoreObject* coin37 = new ScoreObject;
    coin37->SetPhysicsWorld(GetPhysicsWorld());
    coin37->SetScale(b2Vec2{ 50.f, 50.f });
    coin37->SetSource("texture/Score_Object.png");
    coin37->SetName("coin37");
    coin37->transform.position.Set(1050.f, -10.f, 0.f);
    coin37->physics.bodyType = Physics::STATIC;
    coin37->physics.Free();
    coin37->physics.GenerateBody(GetPhysicsWorld(), &coin37->transform);
    coin37->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin37);

    // Coin
    ScoreObject* coin38 = new ScoreObject;
    coin38->SetPhysicsWorld(GetPhysicsWorld());
    coin38->SetScale(b2Vec2{ 50.f, 50.f });
    coin38->SetSource("texture/Score_Object.png");
    coin38->SetName("coin38");
    coin38->transform.position.Set(1150.f, 20.f, 0.f);
    coin38->physics.bodyType = Physics::STATIC;
    coin38->physics.Free();
    coin38->physics.GenerateBody(GetPhysicsWorld(), &coin38->transform);
    coin38->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin38);

    // Coin
    ScoreObject* coin39 = new ScoreObject;
    coin39->SetPhysicsWorld(GetPhysicsWorld());
    coin39->SetScale(b2Vec2{ 50.f, 50.f });
    coin39->SetSource("texture/Score_Object.png");
    coin39->SetName("coin39");
    coin39->transform.position.Set(1250.f, 30.f, 0.f);
    coin39->physics.bodyType = Physics::STATIC;
    coin39->physics.Free();
    coin39->physics.GenerateBody(GetPhysicsWorld(), &coin39->transform);
    coin39->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin39);

    ScoreObject* coin40 = new ScoreObject;
    coin40->SetPhysicsWorld(GetPhysicsWorld());
    coin40->SetScale(b2Vec2{ 50.f, 50.f });
    coin40->SetSource("texture/Score_Object.png");
    coin40->SetName("coin40");
    coin40->transform.position.Set(1350.f, 30.f, 0.f);
    coin40->physics.bodyType = Physics::STATIC;
    coin40->physics.Free();
    coin40->physics.GenerateBody(GetPhysicsWorld(), &coin40->transform);
    coin40->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin40);

    ScoreObject* coin41 = new ScoreObject;
    coin41->SetPhysicsWorld(GetPhysicsWorld());
    coin41->SetScale(b2Vec2{ 50.f, 50.f });
    coin41->SetSource("texture/Score_Object.png");
    coin41->SetName("coin41");
    coin41->transform.position.Set(1450.f, 30.f, 0.f);
    coin41->physics.bodyType = Physics::STATIC;
    coin41->physics.Free();
    coin41->physics.GenerateBody(GetPhysicsWorld(), &coin41->transform);
    coin41->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin41);

    ScoreObject* coin42 = new ScoreObject;
    coin42->SetPhysicsWorld(GetPhysicsWorld());
    coin42->SetScale(b2Vec2{ 50.f, 50.f });
    coin42->SetSource("texture/Score_Object.png");
    coin42->SetName("coin42");
    coin42->transform.position.Set(1550.f, 30.f, 0.f);
    coin42->physics.bodyType = Physics::STATIC;
    coin42->physics.Free();
    coin42->physics.GenerateBody(GetPhysicsWorld(), &coin42->transform);
    coin42->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin42);

    ScoreObject* coin43 = new ScoreObject;
    coin43->SetPhysicsWorld(GetPhysicsWorld());
    coin43->SetScale(b2Vec2{ 50.f, 50.f });
    coin43->SetSource("texture/Score_Object.png");
    coin43->SetName("coin43");
    coin43->transform.position.Set(1650.f, 30.f, 0.f);
    coin43->physics.bodyType = Physics::STATIC;
    coin43->physics.Free();
    coin43->physics.GenerateBody(GetPhysicsWorld(), &coin43->transform);
    coin43->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin43);

    ScoreObject* coin44 = new ScoreObject;
    coin44->SetPhysicsWorld(GetPhysicsWorld());
    coin44->SetScale(b2Vec2{ 50.f, 50.f });
    coin44->SetSource("texture/Score_Object.png");
    coin44->SetName("coin44");
    coin44->transform.position.Set(1750.f, 30.f, 0.f);
    coin44->physics.bodyType = Physics::STATIC;
    coin44->physics.Free();
    coin44->physics.GenerateBody(GetPhysicsWorld(), &coin44->transform);
    coin44->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin44);

    ScoreObject* coin45 = new ScoreObject;
    coin45->SetPhysicsWorld(GetPhysicsWorld());
    coin45->SetScale(b2Vec2{ 50.f, 50.f });
    coin45->SetSource("texture/Score_Object.png");
    coin45->SetName("coin45");
    coin45->transform.position.Set(1850.f, 30.f, 0.f);
    coin45->physics.bodyType = Physics::STATIC;
    coin45->physics.Free();
    coin45->physics.GenerateBody(GetPhysicsWorld(), &coin45->transform);
    coin45->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin45);

    ScoreObject* coin46 = new ScoreObject;
    coin46->SetPhysicsWorld(GetPhysicsWorld());
    coin46->SetScale(b2Vec2{ 50.f, 50.f });
    coin46->SetSource("texture/Score_Object.png");
    coin46->SetName("coin46");
    coin46->transform.position.Set(1950.f, 30.f, 0.f);
    coin46->physics.bodyType = Physics::STATIC;
    coin46->physics.Free();
    coin46->physics.GenerateBody(GetPhysicsWorld(), &coin46->transform);
    coin46->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin46);



    ScoreObject* coin47 = new ScoreObject;
    coin47->SetPhysicsWorld(GetPhysicsWorld());
    coin47->SetScale(b2Vec2{ 50.f, 50.f });
    coin47->SetSource("texture/Score_Object.png");
    coin47->SetName("coin47");
    coin47->transform.position.Set(2050.f, 30.f, 0.f);
    coin47->physics.bodyType = Physics::STATIC;
    coin47->physics.Free();
    coin47->physics.GenerateBody(GetPhysicsWorld(), &coin47->transform);
    coin47->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin47);



    ScoreObject* coin48 = new ScoreObject;
    coin48->SetPhysicsWorld(GetPhysicsWorld());
    coin48->SetScale(b2Vec2{ 50.f, 50.f });
    coin48->SetSource("texture/Score_Object.png");
    coin48->SetName("coin48");
    coin48->transform.position.Set(2150.f, 30.f, 0.f);
    coin48->physics.bodyType = Physics::STATIC;
    coin48->physics.Free();
    coin48->physics.GenerateBody(GetPhysicsWorld(), &coin48->transform);
    coin48->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin48);



    ScoreObject* coin49 = new ScoreObject;
    coin49->SetPhysicsWorld(GetPhysicsWorld());
    coin49->SetScale(b2Vec2{ 50.f, 50.f });
    coin49->SetSource("texture/Score_Object.png");
    coin49->SetName("coin49");
    coin49->transform.position.Set(2250.f, 30.f, 0.f);
    coin49->physics.bodyType = Physics::STATIC;
    coin49->physics.Free();
    coin49->physics.GenerateBody(GetPhysicsWorld(), &coin49->transform);
    coin49->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin49);

    ScoreObject* coin50 = new ScoreObject;
    coin50->SetPhysicsWorld(GetPhysicsWorld());
    coin50->SetScale(b2Vec2{ 50.f, 50.f });
    coin50->SetSource("texture/Score_Object.png");
    coin50->SetName("coin50");
    coin50->transform.position.Set(2350.f, 30.f, 0.f);
    coin50->physics.bodyType = Physics::STATIC;
    coin50->physics.Free();
    coin50->physics.GenerateBody(GetPhysicsWorld(), &coin50->transform);
    coin50->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin50);

    ScoreObject* coin51 = new ScoreObject;
    coin51->SetPhysicsWorld(GetPhysicsWorld());
    coin51->SetScale(b2Vec2{ 50.f, 50.f });
    coin51->SetSource("texture/Score_Object.png");
    coin51->SetName("coin51");
    coin51->transform.position.Set(2450.f, 30.f, 0.f);
    coin51->physics.bodyType = Physics::STATIC;
    coin51->physics.Free();
    coin51->physics.GenerateBody(GetPhysicsWorld(), &coin51->transform);
    coin51->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin51);

    ScoreObject* coin52 = new ScoreObject;
    coin52->SetPhysicsWorld(GetPhysicsWorld());
    coin52->SetScale(b2Vec2{ 50.f, 50.f });
    coin52->SetSource("texture/Score_Object.png");
    coin52->SetName("coin52");
    coin52->transform.position.Set(2550.f, 30.f, 0.f);
    coin52->physics.bodyType = Physics::STATIC;
    coin52->physics.Free();
    coin52->physics.GenerateBody(GetPhysicsWorld(), &coin52->transform);
    coin52->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin52);

    ScoreObject* coin53 = new ScoreObject;
    coin53->SetPhysicsWorld(GetPhysicsWorld());
    coin53->SetScale(b2Vec2{ 50.f, 50.f });
    coin53->SetSource("texture/Score_Object.png");
    coin53->SetName("coin53");
    coin53->transform.position.Set(2650.f, 30.f, 0.f);
    coin53->physics.bodyType = Physics::STATIC;
    coin53->physics.Free();
    coin53->physics.GenerateBody(GetPhysicsWorld(), &coin53->transform);
    coin53->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin53);

    ScoreObject* coin54 = new ScoreObject;
    coin54->SetPhysicsWorld(GetPhysicsWorld());
    coin54->SetScale(b2Vec2{ 50.f, 50.f });
    coin54->SetSource("texture/Score_Object.png");
    coin54->SetName("coin54");
    coin54->transform.position.Set(2750.f, 80.f, 0.f);
    coin54->physics.bodyType = Physics::STATIC;
    coin54->physics.Free();
    coin54->physics.GenerateBody(GetPhysicsWorld(), &coin54->transform);
    coin54->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin54);

    ScoreObject* coin55 = new ScoreObject;
    coin55->SetPhysicsWorld(GetPhysicsWorld());
    coin55->SetScale(b2Vec2{ 50.f, 50.f });
    coin55->SetSource("texture/Score_Object.png");
    coin55->SetName("coin55");
    coin55->transform.position.Set(2850.f, 80.f, 0.f);
    coin55->physics.bodyType = Physics::STATIC;
    coin55->physics.Free();
    coin55->physics.GenerateBody(GetPhysicsWorld(), &coin55->transform);
    coin55->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin55);

    ScoreObject* coin56 = new ScoreObject;
    coin56->SetPhysicsWorld(GetPhysicsWorld());
    coin56->SetScale(b2Vec2{ 50.f, 50.f });
    coin56->SetSource("texture/Score_Object.png");
    coin56->SetName("coin56");
    coin56->transform.position.Set(2950.f, 0.f, 0.f);
    coin56->physics.bodyType = Physics::STATIC;
    coin56->physics.Free();
    coin56->physics.GenerateBody(GetPhysicsWorld(), &coin56->transform);
    coin56->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin56);

    ScoreObject* coin57 = new ScoreObject;
    coin57->SetPhysicsWorld(GetPhysicsWorld());
    coin57->SetScale(b2Vec2{ 50.f, 50.f });
    coin57->SetSource("texture/Score_Object.png");
    coin57->SetName("coin57");
    coin57->transform.position.Set(3050.f, -40.f, 0.f);
    coin57->physics.bodyType = Physics::STATIC;
    coin57->physics.Free();
    coin57->physics.GenerateBody(GetPhysicsWorld(), &coin57->transform);
    coin57->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin57);

    ScoreObject* coin58 = new ScoreObject;
    coin58->SetPhysicsWorld(GetPhysicsWorld());
    coin58->SetScale(b2Vec2{ 50.f, 50.f });
    coin58->SetSource("texture/Score_Object.png");
    coin58->SetName("coin58");
    coin58->transform.position.Set(3150.f, -160.f, 0.f);
    coin58->physics.bodyType = Physics::STATIC;
    coin58->physics.Free();
    coin58->physics.GenerateBody(GetPhysicsWorld(), &coin58->transform);
    coin58->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin58);

    ScoreObject* coin59 = new ScoreObject;
    coin59->SetPhysicsWorld(GetPhysicsWorld());
    coin59->SetScale(b2Vec2{ 50.f, 50.f });
    coin59->SetSource("texture/Score_Object.png");
    coin59->SetName("coin59");
    coin59->transform.position.Set(3250.f, -160.f, 0.f);
    coin59->physics.bodyType = Physics::STATIC;
    coin59->physics.Free();
    coin59->physics.GenerateBody(GetPhysicsWorld(), &coin59->transform);
    coin59->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin59);

    ScoreObject* coin60 = new ScoreObject;
    coin60->SetPhysicsWorld(GetPhysicsWorld());
    coin60->SetScale(b2Vec2{ 50.f, 50.f });
    coin60->SetSource("texture/Score_Object.png");
    coin60->SetName("coin60");
    coin60->transform.position.Set(3350.f, -160.f, 0.f);
    coin60->physics.bodyType = Physics::STATIC;
    coin60->physics.Free();
    coin60->physics.GenerateBody(GetPhysicsWorld(), &coin60->transform);
    coin60->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin60);

    ScoreObject* coin61 = new ScoreObject;
    coin61->SetPhysicsWorld(GetPhysicsWorld());
    coin61->SetScale(b2Vec2{ 50.f, 50.f });
    coin61->SetSource("texture/Score_Object.png");
    coin61->SetName("coin61");
    coin61->transform.position.Set(3450.f, -160.f, 0.f);
    coin61->physics.bodyType = Physics::STATIC;
    coin61->physics.Free();
    coin61->physics.GenerateBody(GetPhysicsWorld(), &coin61->transform);
    coin61->physics.ActiveGhostCollision(true);
    m1->AddScoreObject(coin61);


    mg.AddMap(m1);
}

void Level1::MapGeneratorInitBeach()
{
    m_backgroundColor = BROWN;
    background.sprite.Free();
    background.sprite.LoadImage("texture/BeachBackGround.png", State::m_renderer);
    background.transform.SetScale(1792.f, 756.f);
    AddObject(&background);
    // Map 1 with 2 paltforms and 2 rope joint boxes.
    Map* m1 = new Map;

    // Constructing Collision box for rope joint
    MyObject* collisionBox0 = new MyObject;
    collisionBox0->SetName("CollisionBox0");
    collisionBox0->transform.position.Set(550.f, 30.f, 0.f);
    collisionBox0->transform.SetScale(b2Vec2{ 80.f, 160.f });
    collisionBox0->SetScale(b2Vec2{ 80.f, 80.f });
    collisionBox0->SetSource("texture/rect.png");
    collisionBox0->sprite.color.a = 0;
    collisionBox0->physics.bodyType = Physics::STATIC;
    collisionBox0->physics.Free();
    collisionBox0->physics.GenerateBody(GetPhysicsWorld(), &collisionBox0->transform);
    collisionBox0->SetPhysicsWorld(GetPhysicsWorld());
    collisionBox0->physics.ActiveGhostCollision(true);
    m1->AddObject(collisionBox0);

    // Constructing Collision box for rope joint
    MyObject* collisionBox1 = new MyObject;
    collisionBox1->SetName("CollisionBox1");
    collisionBox1->transform.position.Set(1000.f, 30.f, 0.f);
    collisionBox1->transform.SetScale(b2Vec2{ 80.f, 160.f });
    collisionBox1->SetScale(b2Vec2{ 80.f, 80.f });
    collisionBox1->SetSource("texture/rect.png");
    collisionBox1->sprite.color.a = 0;
    collisionBox1->physics.bodyType = Physics::STATIC;
    collisionBox1->physics.Free();
    collisionBox1->physics.GenerateBody(GetPhysicsWorld(), &collisionBox1->transform);
    collisionBox1->SetPhysicsWorld(GetPhysicsWorld());
    collisionBox1->physics.ActiveGhostCollision(true);
    m1->AddObject(collisionBox1);
    m1->SetNumOfCollisionBox(2);

    // Constructing Rope Joint
    MyObject* objectForRope0 = new MyObject;
    objectForRope0->SetName("objectForRope0");
    objectForRope0->transform.position.Set(800.f, 200.0f, 0.0f);
    objectForRope0->SetScale(b2Vec2{ 75, 400 });
    objectForRope0->SetSource("texture/Charge_Object.png");
    objectForRope0->physics.bodyType = Physics::STATIC;
    objectForRope0->physics.Free();
    objectForRope0->physics.GenerateBody(GetPhysicsWorld(), &objectForRope0->transform);
    objectForRope0->SetPhysicsWorld(GetPhysicsWorld());
    objectForRope0->physics.ActiveGhostCollision(true);
    m1->AddObject(objectForRope0);

    // Constructing Rope Joint
    MyObject* objectForRope1 = new MyObject;
    objectForRope1->SetName("objectForRope1");
    objectForRope1->transform.position.Set(1250.f, 200.0f, 0.0f);
    objectForRope1->SetScale(b2Vec2{ 75, 400 });
    objectForRope1->SetSource("texture/Charge_Object.png");
    objectForRope1->physics.bodyType = Physics::STATIC;
    objectForRope1->physics.Free();
    objectForRope1->physics.GenerateBody(GetPhysicsWorld(), &objectForRope1->transform);
    objectForRope1->SetPhysicsWorld(GetPhysicsWorld());
    objectForRope1->physics.ActiveGhostCollision(true);
    m1->AddObject(objectForRope1);

    // Constructing Platform
    MyObject* pattern0Platform0 = new MyObject;
    pattern0Platform0->SetSource("texture/BeachTile.png");
    pattern0Platform0->SetPhysicsWorld(GetPhysicsWorld());
    pattern0Platform0->SetScale(b2Vec2{ 600.f, 320.f });
    pattern0Platform0->SetName("pattern0Platform0");
    pattern0Platform0->transform.position.Set(300.f, -400.f, 0.f);
    pattern0Platform0->transform.rotation = 0;
    m1->AddObject(pattern0Platform0);

    // Constructing Platform
    MyObject* pattern0Platform1 = new MyObject;
    pattern0Platform1->SetSource("texture/BeachTile2.png");
    pattern0Platform1->SetPhysicsWorld(GetPhysicsWorld());
    pattern0Platform1->SetScale(b2Vec2{ 600.f, 320.f });
    pattern0Platform1->SetName("pattern0Platform1");
    pattern0Platform1->transform.position.Set(1600.f, -400.f, 0.f);
    pattern0Platform1->transform.rotation = 0;
    m1->AddObject(pattern0Platform1);


    mg.AddMap(m1);


    // Map 2 with 2 paltforms and 1 rope joint boxes.
    Map* m2 = new Map;

    // Constructing Collision box for rope joint
    MyObject* collisionBox2 = new MyObject;
    collisionBox2->SetName("CollisionBox2");
    collisionBox2->transform.position.Set(550.f, 30.f, 0.f);
    collisionBox2->transform.SetScale(b2Vec2{ 80.f, 160.f });
    collisionBox2->SetScale(b2Vec2{ 80.f, 80.f });
    collisionBox2->SetSource("texture/rect.png");
    collisionBox2->sprite.color.a = 0;
    collisionBox2->physics.bodyType = Physics::STATIC;
    collisionBox2->physics.Free();
    collisionBox2->physics.GenerateBody(GetPhysicsWorld(), &collisionBox2->transform);
    collisionBox2->SetPhysicsWorld(GetPhysicsWorld());
    collisionBox2->physics.ActiveGhostCollision(true);
    m2->AddObject(collisionBox2);
    m2->SetNumOfCollisionBox(1);

    // Constructing Rope Joint
    MyObject* objectForRope2 = new MyObject;
    objectForRope2->SetName("objectForRope3");
    objectForRope2->transform.position.Set(800.f, 200.0f, 0.0f);
    objectForRope2->SetScale(b2Vec2{ 75, 400 });
    objectForRope2->SetSource("texture/Charge_Object.png");
    objectForRope2->physics.bodyType = Physics::STATIC;
    objectForRope2->physics.Free();
    objectForRope2->physics.GenerateBody(GetPhysicsWorld(), &objectForRope2->transform);
    objectForRope2->SetPhysicsWorld(GetPhysicsWorld());
    objectForRope2->physics.ActiveGhostCollision(true);
    m2->AddObject(objectForRope2);

    // Constructing Platform
    MyObject* pattern1Platform0 = new MyObject;
    pattern1Platform0->SetSource("texture/BeachTile2.png");
    pattern1Platform0->SetPhysicsWorld(GetPhysicsWorld());
    pattern1Platform0->SetScale(b2Vec2{ 1000.f, 320.f });
    pattern1Platform0->SetName("pattern1Platform0");
    pattern1Platform0->transform.position.Set(300.f, -400.f, 0.f);
    pattern1Platform0->transform.rotation = 0;
    m2->AddObject(pattern1Platform0);

    // Constructing Platform
    MyObject* pattern1Platform1 = new MyObject;
    pattern1Platform1->SetSource("texture/BeachTile.png");
    pattern1Platform1->SetPhysicsWorld(GetPhysicsWorld());
    pattern1Platform1->SetScale(b2Vec2{ 300.f, 320.f });
    pattern1Platform1->SetName("pattern3Platform1");
    pattern1Platform1->transform.position.Set(1200.f, -400.f, 0.f);
    pattern1Platform1->transform.rotation = 0;
    m2->AddObject(pattern1Platform1);

    // Not added Map 2
    mg.AddMap(m2);


    // Map 3 with 3 paltforms and 2 rope joint boxes.
    Map* m3 = new Map;

    // Constructing Collision box for rope joint
    MyObject* collisionBox3 = new MyObject;
    collisionBox3->SetName("CollisionBox3");
    collisionBox3->transform.position.Set(550.f, 30.f, 0.f);
    collisionBox3->transform.SetScale(b2Vec2{ 80.f, 160.f });
    collisionBox3->SetScale(b2Vec2{ 80.f, 80.f });
    collisionBox3->SetSource("texture/rect.png");
    collisionBox3->sprite.color.a = 0;
    collisionBox3->physics.bodyType = Physics::STATIC;
    collisionBox3->physics.Free();
    collisionBox3->physics.GenerateBody(GetPhysicsWorld(), &collisionBox3->transform);
    collisionBox3->SetPhysicsWorld(GetPhysicsWorld());
    collisionBox3->physics.ActiveGhostCollision(true);
    m3->AddObject(collisionBox3);

    // Constructing Collision box for rope joint
    MyObject* collisionBox4 = new MyObject;
    collisionBox4->SetName("CollisionBox1");
    collisionBox4->transform.position.Set(1300.f, 30.f, 0.f);
    collisionBox4->transform.SetScale(b2Vec2{ 80.f, 160.f });
    collisionBox4->SetScale(b2Vec2{ 80.f, 80.f });
    collisionBox4->SetSource("texture/rect.png");
    collisionBox4->sprite.color.a = 0;
    collisionBox4->physics.bodyType = Physics::STATIC;
    collisionBox4->physics.Free();
    collisionBox4->physics.GenerateBody(GetPhysicsWorld(), &collisionBox4->transform);
    collisionBox4->SetPhysicsWorld(GetPhysicsWorld());
    collisionBox4->physics.ActiveGhostCollision(true);
    m3->AddObject(collisionBox4);
    m3->SetNumOfCollisionBox(2);

    // Constructing Rope Joint
    MyObject* objectForRope3 = new MyObject;
    objectForRope3->SetName("objectForRope3");
    objectForRope3->transform.position.Set(800.f, 200.0f, 0.0f);
    objectForRope3->SetScale(b2Vec2{ 75, 400 });
    objectForRope3->SetSource("texture/Charge_Object.png");
    objectForRope3->physics.bodyType = Physics::STATIC;
    objectForRope3->physics.Free();
    objectForRope3->physics.GenerateBody(GetPhysicsWorld(), &objectForRope3->transform);
    objectForRope3->SetPhysicsWorld(GetPhysicsWorld());
    objectForRope3->physics.ActiveGhostCollision(true);
    m3->AddObject(objectForRope3);

    // Constructing Rope Joint
    MyObject* objectForRope4 = new MyObject;
    objectForRope4->SetName("objectForRope4");
    objectForRope4->transform.position.Set(1550.f, 200.0f, 0.0f);
    objectForRope4->SetScale(b2Vec2{ 75, 400 });
    objectForRope4->SetSource("texture/Charge_Object.png");
    objectForRope4->physics.bodyType = Physics::STATIC;
    objectForRope4->physics.Free();
    objectForRope4->physics.GenerateBody(GetPhysicsWorld(), &objectForRope4->transform);
    objectForRope4->SetPhysicsWorld(GetPhysicsWorld());
    objectForRope4->physics.ActiveGhostCollision(true);
    m3->AddObject(objectForRope4);

    // Constructing Platform
    MyObject* pattern2Platform0 = new MyObject;
    pattern2Platform0->SetSource("texture/BeachTile2.png");
    pattern2Platform0->SetPhysicsWorld(GetPhysicsWorld());
    pattern2Platform0->SetScale(b2Vec2{ 900.f, 320.f });
    pattern2Platform0->SetName("pattern2Platform0");
    pattern2Platform0->transform.position.Set(300.f, -400.f, 0.f);
    pattern2Platform0->transform.rotation = 0;
    m3->AddObject(pattern2Platform0);

    // Constructing Platform
    MyObject* pattern2Platform1 = new MyObject;
    pattern2Platform1->SetSource("texture/BeachTile.png");
    pattern2Platform1->SetPhysicsWorld(GetPhysicsWorld());
    pattern2Platform1->SetScale(b2Vec2{ 450.f, 320.f });
    pattern2Platform1->SetName("pattern2Platform1");
    pattern2Platform1->transform.position.Set(1900.f, -400.f, 0.f);
    pattern2Platform1->transform.rotation = 0;
    m3->AddObject(pattern2Platform1);

    // Constructing Platform
    MyObject* pattern2Platform2 = new MyObject;
    pattern2Platform2->SetSource("texture/BeachTile.png");
    pattern2Platform2->SetPhysicsWorld(GetPhysicsWorld());
    pattern2Platform2->SetScale(b2Vec2{ 450.f, 320.f });
    pattern2Platform2->SetName("pattern2Platform2");
    pattern2Platform2->transform.position.Set(1200.f, -400.f, 0.f);
    pattern2Platform2->transform.rotation = 0;
    m3->AddObject(pattern2Platform2);

    mg.AddMap(m3);
}

void Level1::MapGeneratorInitPrepare()
{
    Map* m1 = new Map;

    MyObject* preparePlatform1 = new MyObject;
    preparePlatform1->SetName("preparePlatform");
    preparePlatform1->transform.position.Set(0.f, -400.f, 0.f);
    preparePlatform1->SetScale(b2Vec2{ 600.f, 320.f });
    preparePlatform1->sprite.Free();
    preparePlatform1->SetSource("texture/prepare_platform.png");
    preparePlatform1->sprite.color.a = 150;
    preparePlatform1->physics.bodyType = Physics::STATIC;
    m1->AddObject(preparePlatform1);

    mg.AddMap(m1);
}

void Level1::StageChange(Stage stage)
{
    // MapGenerator should be Updated..
    // Clean Up, Init..

    background.transform.position = b2Vec3{ 0.f, 0.f, 0.f };

    if (rope != nullptr)
    {
        // Rope Clean Up?
        RemoveObject(rope);
        delete rope;
        rope = nullptr;
        JointBox = nullptr;
        collisionBox = nullptr;
        rope_joint_def.collideConnected = false;
        GetPhysicsWorld()->DestroyJoint(rope_joint);
    }

    mapGeneratorMakeEmpty();

    switch (stage)
    {
    case Factory:
        currLevel = Factory;
        backgroundMusic.LoadMusic("sound/Purple.mp3");
        m_pBGM = &backgroundMusic;
        backgroundMusic.Play();
        MapGeneratorInitFactory();
        playerVelocity = b2Vec2{ 25.f, animation.physics.GetVelocity().y };
        hpDecrease = 4.f;
        break;
    case HighWay:
        currLevel = HighWay;
        backgroundMusic.Free();
        backgroundMusic.LoadMusic("sound/Slip.mp3");
        m_pBGM = &backgroundMusic;
        backgroundMusic.Play();
        MapGeneratorInitHighWay();
        playerVelocity = b2Vec2{ 25.f + amountSpeedUP + amountSpeedUP * 3 - 3, animation.physics.GetVelocity().y };
        hpDecrease = 10.f;
        break;
    case DownTown:
        currLevel = DownTown;
        backgroundMusic.Free();
        backgroundMusic.LoadMusic("sound/Bouncey.mp3");
        m_pBGM = &backgroundMusic;
        backgroundMusic.Play();
        MapGeneratorInitDownTown();
        playerVelocity = b2Vec2{ 25.f + amountSpeedUP , animation.physics.GetVelocity().y };
        hpDecrease = 8.f;
        break;
    case CountryRoad:
        currLevel = CountryRoad;
        backgroundMusic.Free();
        backgroundMusic.LoadMusic("sound/We_Share_This.mp3");
        m_pBGM = &backgroundMusic;
        backgroundMusic.Play();
        MapGeneratorInitCountryRoad();
        playerVelocity = b2Vec2{ playerVelocity.x + amountSpeedUP * 5, animation.physics.GetVelocity().y };
        hpDecrease = 12.f;
        break;
    case Beach:
        currLevel = Beach;
        backgroundMusic.Free();
        backgroundMusic.LoadMusic("sound/School_Bus_Shuffle.mp3");
        m_pBGM = &backgroundMusic;
        backgroundMusic.Play();
        MapGeneratorInitBeach();
        playerVelocity = b2Vec2{ playerVelocity.x + amountSpeedUP, animation.physics.GetVelocity().y };
        hpDecrease = 14.f;
        break;
    case numOfStages:
        break;
    default:;
    }
    SettingPreparePlatform();
    animation.transform.position.y = 500.f;
    camera.position = b2Vec3(animation.transform.position.x + 350.f, 0, currentCameraZoomIn_Z);
    PrepareMapGenerator();
}

void Level1::PrepareMapGenerator()
{
    mg.AddMapToGame(0);
    for (int i = 0; i < mg.GetBackFromGame()->GetObjectSize(); i++)
    {
        MyObject* object = mg.GetBackFromGame()->GetObject(i);
        // 400.f is arbitary number
        object->transform.position.x -= 400.f;
        object->physics.Free();
        object->physics.GenerateBody(GetPhysicsWorld(), &object->transform);
        object->physics.ActiveGhostCollision(object->physics.IsActiveGhost());
        AddObject(object);
    }
    for (int i = 0; i < mg.GetBackFromGame()->GetScoreObjectSize(); i++)
    {
        // Time to add ScoreObject
        ScoreObject* initialScoreObject = mg.GetBackFromGame()->GetScoreObject(i);
        // 400.f is arbitary number
        initialScoreObject->transform.position.x -= 400.f;
        initialScoreObject->physics.Free();
        initialScoreObject->physics.GenerateBody(GetPhysicsWorld(), &initialScoreObject->transform);
        initialScoreObject->physics.ActiveGhostCollision(initialScoreObject->physics.IsActiveGhost());
        AddObject(initialScoreObject);
    }
    mg.GetBackFromGame()->Calculate();
}

void Level1::SettingPreparePlatform(void)
{
    if (preparePlatform.physics.HasBody())
    {
        preparePlatform.physics.Free();
    }
    preparePlatform.transform.position.x = camera.position.x;
    preparePlatform.physics.Free();
    preparePlatform.physics.GenerateBody(GetPhysicsWorld(), &preparePlatform.transform);
}

void Level1::FilterFadeOut(const SDL_Color& color)
{
    Filter.sprite.color = color;
    Filter.sprite.color.a = 255;

}

std::string Level1::GetFeverTypeSource(FeverObjects type)
{
    switch (type) {
    case D:
        return "texture/Object_D.png";
    case I_1:
        return "texture/Object_I_1.png";
    case G:
        return "texture/Object_G.png";
    case I_2:
        return "texture/Object_I_2.png";
    case P:
        return "texture/Object_P.png";
    case E:
        return "texture/Object_E.png";
    case N:
        return "texture/Object_N.png";
    case NumOfFeverObjects: break;
    default:;
    }
    // default return
    return "texture/rect.png";
}

std::string Level1::GetFeverTypeName(FeverObjects type)
{
    switch (type) {
    case D:
        return "Object_D";
    case I_1:
        return "Object_I_1";
    case G:
        return "Object_G";
    case I_2:
        return "Object_I_2";
    case P:
        return "Object_P";
    case E:
        return "Object_E";
    case N:
        return "Object_N";
    case NumOfFeverObjects: break;
    default:;
    }
    return "Unnamed Fever Object.png";
}

void Level1::RemoveRope_RopeTransformUpdate(MyObject *JointBox_)
{
    //The ro  pe which is connected to JointBox will rotate and scale properly   
    if (rope_joint_def.collideConnected == true) {
        auto ropeJoint = JointBox_->transform.position; /*mg.GetMapFromGame(0)->GetObject(2)->transform.position;*/
        rope_position = animation.transform.position + ropeJoint;
        rope->transform.position.Set(rope_position.x / 2.0f, rope_position.y / 2.0f, rope_position.z);
        rope->transform.rotation = (180.0f*GetAngle(JointBox_)) / 3.14f;

        float ropeX = ropeJoint.x - animation.transform.position.x;
        float ropeY = ropeJoint.y - animation.transform.position.y;

        float ropeSize = sqrt((ropeX*ropeX) + (ropeY*ropeY));

        rope->transform.position.Set(rope_position.x / 2.0f, rope_position.y / 2.0f, rope_position.z);
        rope->transform.SetScale(ropeSize, 40);

        rope->transform.rotation = (180.0f*GetAngle(JointBox_)) / 3.14f;

        animation.sprite.Free();
        animation.sprite.LoadImage("texture/Rope_Pong.png", m_renderer);
    }
    //Rope has created but no jointed and will be removed
    if (rope != nullptr && timer > 0.002f && rope_joint_def.collideConnected == false) {
        RemoveObject(rope);
        delete rope;
        JointBox = nullptr;
        collisionBox = nullptr;
        rope = nullptr;
    }
    //Rope has created and jointed and remove after that
    else if (rope != nullptr && 125.f < (180.0f*GetAngle(JointBox_)) / 3.14f && rope_joint_def.collideConnected == true) {
        RemoveObject(rope);
        delete rope;
        rope = nullptr;
        JointBox = nullptr;
        collisionBox = nullptr;
        rope_joint_def.collideConnected = false;
        // currentCameraZoomIn_Z = 0;
        GetPhysicsWorld()->DestroyJoint(rope_joint);
    }

    if (currentCameraZoomIn_Z > sizeofZoomin && rope_joint_def.collideConnected == true)
    {
        //The speed of the Zoomin
        currentCameraZoomIn_Z -= speedofZoomin;
        controlCamera_Zoom_X += speedofZoomout_X;
    }
    if (currentCameraZoomIn_Z < 0 && rope_joint_def.collideConnected == false) {
        //The speed of the ZoomOut
        currentCameraZoomIn_Z += speedofZoomout_Z;
    }

    if (controlCamera_Zoom_X > 0 && rope_joint_def.collideConnected == false) {
        //The speed of the ZoomOut
        controlCamera_Zoom_X -= speedofZoomout_X;
    }
}

//function for initializing battery UI
void Level1::InitializeBatteryUI()
{
    healthBar.SetName("healthBar");
    healthBar.sprite.Free();
    healthBar.sprite.LoadImage("texture/Healthbar.png", State::m_renderer);
    healthBar.transform.position.Set(0.0f, -200.0f, 500.0f);
    healthBar.transform.SetScale(Health, 70.0f);
    healthBar.sprite.isHud = true;
    AddObject(&healthBar);

    battery.SetName("battery");
    battery.sprite.Free();
    battery.sprite.LoadImage("texture/battery.png", State::m_renderer);
    battery.transform.position.Set(0.0f, -320.0f, 0.0f);
    battery.transform.SetScale(270.0f, 120.0f);
    battery.sprite.isHud = true;
    AddObject(&battery);
}

//function for initializing alphabet UI
void Level1::InitializeAlphabetUI()
{
    alphabet_D.SetName("D");
    alphabet_D.sprite.Free();
    alphabet_D.sprite.LoadImage("texture/UI_D_FALSE.png", State::m_renderer);
    alphabet_D.transform.position.Set(-600.0f, 300.0f, 0.0f);
    alphabet_D.transform.SetScale(40.0f, 40.0f);
    alphabet_D.sprite.isHud = true;
    AddObject(&alphabet_D);

    alphabet_I_1.SetName("I_1");
    alphabet_I_1.sprite.Free();
    alphabet_I_1.sprite.LoadImage("texture/UI_I_1_FALSE.png", State::m_renderer);
    alphabet_I_1.transform.position.Set(-550.0f, 300.0f, 0.0f);
    alphabet_I_1.transform.SetScale(40.0f, 40.0f);
    alphabet_I_1.sprite.isHud = true;
    AddObject(&alphabet_I_1);

    alphabet_G.SetName("G");
    alphabet_G.sprite.Free();
    alphabet_G.sprite.LoadImage("texture/UI_G_FALSE.png", State::m_renderer);
    alphabet_G.transform.position.Set(-500.0f, 300.0f, 0.0f);
    alphabet_G.transform.SetScale(40.0f, 40.0f);
    alphabet_G.sprite.isHud = true;
    AddObject(&alphabet_G);

    alphabet_I_2.SetName("I_2");
    alphabet_I_2.sprite.Free();
    alphabet_I_2.sprite.LoadImage("texture/UI_I_2_FALSE.png", State::m_renderer);
    alphabet_I_2.transform.position.Set(-450.0f, 300.0f, 0.0f);
    alphabet_I_2.transform.SetScale(40.0f, 40.0f);
    alphabet_I_2.sprite.isHud = true;
    AddObject(&alphabet_I_2);

    alphabet_P.SetName("P");
    alphabet_P.sprite.Free();
    alphabet_P.sprite.LoadImage("texture/UI_P_FALSE.png", State::m_renderer);
    alphabet_P.transform.position.Set(-400.0f, 300.0f, 0.0f);
    alphabet_P.transform.SetScale(40.0f, 40.0f);
    alphabet_P.sprite.isHud = true;
    AddObject(&alphabet_P);

    alphabet_E.SetName("E");
    alphabet_E.sprite.Free();
    alphabet_E.sprite.LoadImage("texture/UI_E_FALSE.png", State::m_renderer);
    alphabet_E.transform.position.Set(-350.0f, 300.0f, 0.0f);
    alphabet_E.transform.SetScale(40.0f, 40.0f);
    alphabet_E.sprite.isHud = true;
    AddObject(&alphabet_E);

    alphabet_N.SetName("N");
    alphabet_N.sprite.Free();
    alphabet_N.sprite.LoadImage("texture/UI_N_FALSE.png", State::m_renderer);
    alphabet_N.transform.position.Set(-300.0f, 300.0f, 0.0f);
    alphabet_N.transform.SetScale(40.0f, 40.0f);
    alphabet_N.sprite.isHud = true;
    AddObject(&alphabet_N);
}

//function for initializing score UI
void Level1::InitializeScoreUI()
{
    scoreText.transform.position.Set(500.f, 300.f, 0.f);
    scoreText.transform.SetScale(200.f, 40.f);
    scoreText.text.font = mainFont;
    scoreText.text.isHud = true;
    AddObject(&scoreText);
}

void Level1::LoadLevelInfo()
{
    FILE* levelInfo = fopen("LevelInfo.txt", "r+");

    // If there is no text file, make new text file.
    if (levelInfo == nullptr)
    {
        return;
    }

    fscanf(levelInfo, "%f %d %d %d", &Health, &score, &currLevel, reinterpret_cast<int*>(&timer2));
    fclose(levelInfo);
}

void Level1::SaveLevelInfo()
{
    FILE* levelInfo = fopen("LevelInfo.txt", "w");

    if (levelInfo != nullptr)
    {
        fprintf(levelInfo, "%f %d %d %f", Health, score, currLevel, (timer2));
        fclose(levelInfo);
    }
}

void Level1::DeleteLevelInfo()
{
    FILE* levelInfo = fopen("LevelInfo.txt", "w");

    if (levelInfo != nullptr)
    {
        fprintf(levelInfo, "");
        fclose(levelInfo);
    }
}

// Clean up MapGenerator everything
void Level1::mapGeneratorMakeEmpty()
{
    while (mg.GameMapSize() != 0)
    {
        RemoveFirstMapFromLevel();
        mg.RemoveGameMap();
    }
    mg.RemoveMap();

}

float Level1::GetAngle(MyObject * jointBox_)
{
    if (jointBox_ == nullptr)
    {
        return 0.f;
    }

    angle_vector = jointBox_->transform.position - animation.transform.position;
    angle = atan2(angle_vector.y, angle_vector.x);

    return  angle;
}

//function for checking collision and execute event
void Level1::FeverObjectEvent()
{

    if (fever_object_count >= fever_count_togo_Fever)
    {
        nextStage = true;
        animation.physics.SetVelocity(b2Vec2{ 0, 0 });
        if (isFeverEffect == false) {

            FeverEffect.SetName("FeverEffect");
            FeverEffect.transform.position.Set(0.f, 0.f, 0.f);
            FeverEffect.transform.SetScale(b2Vec2{ 1100.f, 700.f });
            FeverEffect.sprite.Free();
            FeverEffect.sprite.LoadImage("texture/Fever_Screen.png", State::m_renderer);

            FeverEffect.sprite.isHud = true;
            AddObject(&FeverEffect);

            isFeverEffect = true;
            checkLoadFeverTime = static_cast<float>(timer2);
        }
        if (FeverEffect.sprite.color.a > 10) {
            FeverEffect.sprite.color.a -= 3;
        }

        if (timer2 > checkLoadFeverTime + 1.3) {
            isGoingFever = true;
            isFeverEffect = false;
            FeverEffect.sprite.Free();
            FeverEffect.sprite.color.a = 255;
            m_game->Change(LV_FEVER);
        }
    }
}

void Level1::InitializeVariables()
{
    //Initialize for Fadein&out
    hpDecrease = 4.f;
    deadCount = 2.f;
    deadCheck = false;
    ropeCheck = true;
    jumpCheck = true;
    fallCheck = false;
    hpDecrease = 4.f;
    Health = 200;
    
    isStageChange = false;
    isTimetoAlphaDown = false;
    isTimetoAlphaUp = true;
    prevAlphaValue = 6;
    prevTimeforFadeinOut = 0;
    checkLoadFeverTime = 0.f;
    isGoingFever = false;
    timer2 = 0;
    isStageChange = !(timer2 < 5.f);
    checkSecond = 20.0f;
    checkPrevSecond = 0.0f;
    onSpeedUp = true;

    //function for initializing battery UI
    InitializeBatteryUI();

    //function for initializing alphabet UI
    InitializeAlphabetUI();

    //function for initializing score UI
    InitializeScoreUI();

    fever_object_count = 0;

    d_get_check = false;
    i_1_get_check = false;
    g_get_check = false;
    i_2_get_check = false;
    p_get_check = false;
    e_get_check = false;
    n_get_check = false;


    isHighway = false;
    isDowntown = false;
    isCountryRoad = false;

    playerVelocity = b2Vec2{ 25, 0.f };
    isWarningFilterOn = false;
    isBrighter = true;
    isGoingFever = false;

    canJump = true;
    nextStage = false;
    currLevel = Stage::Factory;

    background.transform.position.SetZero();
}
void Level1::setMusic()
{
    if (m_input->IsTriggered(SDL_SCANCODE_UP)) {
        volume += 8;
        if (volume > 128)
            volume = 128;
    }

    else if (m_input->IsTriggered(SDL_SCANCODE_DOWN)) {
        volume -= 8;
        if (volume < 0)
            volume = 0;
    }
    backgroundMusic.SetVolume(volume);
}

