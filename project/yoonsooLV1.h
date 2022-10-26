
/*﻿﻿
 File name: yoonsooLV1.h
 Project name: Phone! Pong!
 Primary Author: Yoonsoo Kwon, Shinil Kang
 Secondary Author: Jaemin Woo, Minho Chae
 Copyright information:
 "All content © 2019 DigiPen (USA) Corporation, all rights reserved."
*/

#pragma once
#include "engine\State.h"
#include "engine\Object.h"
#include "List.h"

    extern int score;
extern bool nextStage;

class Level1 : public State
{
    enum FeverObjects
    {
        D,
        I_1,
        G,
        I_2,
        P,
        E,
        N,
        NumOfFeverObjects
    };

    enum Stage
    {
        Prepare,
        Factory,
        HighWay,
        DownTown,
        CountryRoad,
        Beach,
        numOfStages
    };
    friend class Game;

protected:
    Level1() : State() {};
    ~Level1() override {};

    void LoadLevelInfo();
    // Derived initialize function
    void Initialize() override;
    void UpdateWarningFilter();
    // Derived Update function
    void Update(float dt) override;
    void SaveLevelInfo();
    void DeleteLevelInfo();
    void mapGeneratorMakeEmpty(void);
    // Derived Close function
    void Close() override;

    void InitializeMapGenerator(void);
    void UpdateMapGenerator(float dt);

    void MapGeneratorAddObject(void);
    void MapGeneratorRemoveObject(void);
    void RemoveFirstMapFromLevel(void);

    // Function for score
    void Score(void);

    // Function for rope
    void makeNewRope();
    void RemoveRope_RopeTransformUpdate(MyObject *JointBox_);
    float GetAngle(MyObject *JointBox_);
    void Jumping(Object& object);
    void makeFadeOutAndIn(Stage);

    //function for initializing
    void InitializeAlphabetUI();
    void InitializeBatteryUI();
    void InitializeScoreUI();
    void InitializeVariables();

    //function for checking collision and execute event
    void FeverObjectEvent();

    //SpeedUp
    void SpeedUp();

    //music
    void setMusic();

    void MapGeneratorInitFactory();
    void MapGeneratorInitHighWay();
    void MapGeneratorInitDownTown();
    void MapGeneratorInitCountryRoad();
    void MapGeneratorInitBeach();
    void MapGeneratorInitPrepare();
    void StageChange(Stage stage);
    void PrepareMapGenerator();

    void SettingPreparePlatform(void);

    void FilterFadeOut(const SDL_Color& color);
    std::string GetFeverTypeSource(FeverObjects);
    std::string GetFeverTypeName(FeverObjects);

private:
    float hpDecrease;
    float deadCount;
    bool deadCheck;
    bool ropeCheck;
    bool jumpCheck;
    bool fallCheck;
    b2Vec2 playerVelocity{ 25, 0.f };
    float standardTime = 5.0f;
    float Health;

    float gravity_x = 0.f;
    float gravity_y = -19.6f;

    //enum Status state = Running;
    float betaspeed = 500.f;
    float target_dt = 0;
    float charged_dt = 0;
    int mapchange = 0;
    //variable for counting number of fever object
    int fever_object_count = 0;
    int fever_count_togo_Fever = 7;

    // bool for checking you got fever object
    bool d_get_check = false;
    bool i_1_get_check = false;
    bool g_get_check = false;
    bool i_2_get_check = false;
    bool p_get_check = false;
    bool e_get_check = false;
    bool n_get_check = false;
    bool isCountryRoad = false;

    void ControlCamera(float dt);
    void TransformObjects(float dt);
    const int period = 180;
    float ellapsedtime = 0.0f;

    // Flag variable to check new state is fever or not
    bool isGoingFever = false;
    int volume = 128;

    //magic numbers
    const float PONG_FALL_RANGE = -280.f;
    const float PONG_LANDING_RANGE_UP = -170.f;
    const float PONG_LANDING_RANGE_DOWN = -200.f;
    const float HEALTH_WARNING_RANGE = 70.f;
    const float HEALTH_WARNING_RANGE_UP = 70.2f;
    const float HEALTH_WARNING_RANGE_DOWN = 69.8f;
    const float HEALTH_CHARGE = 60.f;
    const float HEALTH_SUFFICIENT = 180.f;
    const float HEALTH_MAX = 200.f;
    const float ANGLE_180 = 180.f;
    const float PI = 3.14f;
    const int SCORE_UP = 100;
    const float ANGLE_45 = 45.f;
    const float CAMERA_OUTSIDE_POSITION = 600.f;

    // Objects
    Object wall, newTilePoint;
    Object animation, animationSensor, warningSoundEffect, jumpSoundEffect, ropeSoundEffect, feverSoundEffect, fallingSoundEffect, phoneOffSoundEffect, coinSoundEffect;

    Object titleText2;

    // Varialbes for rope

    Object *rope = nullptr;
    b2DistanceJoint * rope_joint = nullptr;
    b2DistanceJointDef rope_joint_def;
    MyObject* JointBox = nullptr;
    MyObject* collisionBox = nullptr;

    // Heath bar for HUD
    Object healthBar;
    Object battery;
    Object Filter;
    Object FeverEffect;

    // object for showing score 
    Object scoreText;

    // objects for showing fever alphabet UI
    Object alphabet_D;
    Object alphabet_I_1;
    Object alphabet_G;
    Object alphabet_I_2;
    Object alphabet_P;
    Object alphabet_E;
    Object alphabet_N;

    // Map Generator
    MapGenerator mg;
    Object preparePlatform;

    //To rotate rope
    b2Vec3 angle_vector;
    float angle;
    float distance_ObjectforRope = 650.0f;
    float basicDistance_ObjectforRope = -70.0f;
    b2Vec3 rope_position;

    //Speed up
    Object speedUpText;
    float checkSecond = 20.0f;
    float checkPrevSecond = 0.0f;
    bool  onSpeedUp = true;
    int amountSpeedUP = 4;
    float checkTimerforspeedUp = 20;

    //Timers
    double timer = 0;
    double timer2 = 0;

    MyObject background;

    //Stage Check bool
    bool isHighway = false;
    bool isDowntown = false;

    //FeverEffect check bool
    bool isFeverEffect = false;
    float checkLoadFeverTime = 0;

    Object warningFilter;
    bool isWarningFilterOn = false;
    bool isBrighter = true;

    //For stage change 
    bool isStageChange = false;
    bool isTimetoAlphaDown = false;
    bool isTimetoAlphaUp = true;
    Uint8 prevAlphaValue = 6;
    double prevTimeforFadeinOut = 0;

    bool canJump = true;
    //Camera Zooin&Out size
    float32 currentCameraZoomIn_Z = 0;
    float32 sizeofZoomin = -100;
    float32 speedofZoomin = 5;
    float32 speedofZoomout_X = 8;
    float32 speedofZoomout_Z = 10;
    bool isCameraZoom = false;

    float32 controlCamera_Zoom_X = 0;
    Stage currLevel = Stage::Factory;

};
