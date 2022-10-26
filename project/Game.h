/******************************************************************************/
/*!
\file   Game.h
\author David Ly and Juyong Jeong
\par    email: dly\@digipen.edu
\par    GAM150 demo
\par	v0
\date   2018/03/11

Game will only handle the changes of the game states (Level switching)
ie. Main Menu, Levels, Pause, etc.
No game play logic should be added here.
*/
/******************************************************************************/

#pragma once

// Level headers
#include "MainMenu.h"
#include "PauseState.h"
#include "StageSinil.h"
#include "yoonsooLV1.h"
#include "Gameover.h"
#include "ScoreMenu.h"
#include "Input.h"
#include "LevelFever.h"
#include "Menu_Ranking.h"
#include "Menu_HowtoPlay.h"
#include "Menu_Credit.h"
#include "Splash.h"

//This is the list of levels the game has. When adding a new level(state)
//add it to this list. They are tagged with LV_ to enable better autocomplete 
typedef enum
{
    LV_Splash = 0,
    LV_MainMenu,
    LV_LevelYoonsoo,
    LV_FEVER,
    LV_DEADSCENE,
    LV_Pause,
    LV_ScoreMenu,
    LV_MenuRanking,
    LV_MenuHowToPlay,
    LV_MenuCredit,
    LV_Input,
    //etc.

    LV_NUM_LEVELS
}Level_ID;

class StateManager;

class Game
{
    friend class Application;

    StateManager *m_stateManager = nullptr;

public:

    Game();
    ~Game();

    void	RegisterState(State* state);
    void	SetFirstState(State* state);
    State*	GetCurrentState();

    void	Quit();
    void	Change(unsigned stateId);

    bool	Initialize();
    void	Update(float dt);
    void	Close();

    void	RegisterPauseState(State *state);
    void	Pause();
    void	Resume();
    void	Restart();
    bool	IsQuit();

private:

    // All the level(state) declarations are here
    Splash	splash;
    MainMenu	mainMenu;
    LevelSinil sinilLevel;
    LevelFever feverLevel;
    Level1 yoonsooLevel;
    PauseState	pauseLevel;
    ScoreMenu   scoreMenu;
    MenuRanking MenuRanking;
    MenuHowToPlay MenuHowToPlay;
    MenuCredit MenuCredit;
    Gameover gameover;
    Input2 input;
public:

    static constexpr int SCREEN_WIDTH = 1280, SCREEN_HEIGHT = 720;
    int	m_width = 0, m_height = 0;

    Game(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(const Game&) = delete;
    Game& operator=(Game&&) = delete;

};