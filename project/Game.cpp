#include "Game.h"
#include "engine/StateManager.h"

Game::Game()
{
    m_stateManager = new StateManager;
}

Game::~Game()
{
    if (m_stateManager) {
        delete m_stateManager;
        m_stateManager = nullptr;
    }
}

/**
 * \brief
 * Initialize game states and window size
 *
 * \return
 * True
 */
bool Game::Initialize()
{
    // Set window size
    State::m_width = SCREEN_WIDTH, State::m_height = SCREEN_HEIGHT;

    // Set state ids
    splash.SetId(LV_Splash);
    mainMenu.SetId(LV_MainMenu);
    yoonsooLevel.SetId(LV_LevelYoonsoo);
    feverLevel.SetId(LV_FEVER);
    pauseLevel.SetId(LV_Pause);
    MenuRanking.SetId(LV_MenuRanking);
    MenuHowToPlay.SetId(LV_MenuHowToPlay);
    scoreMenu.SetId(LV_ScoreMenu);
    MenuCredit.SetId(LV_MenuCredit);
    gameover.SetId(LV_DEADSCENE);
    input.SetId(LV_Input);
    // Register states
    RegisterState(&splash);
    RegisterState(&mainMenu);
    RegisterState(&yoonsooLevel);
    RegisterState(&feverLevel);
    RegisterState(&scoreMenu);
    RegisterState(&gameover);
    RegisterState(&input);
    RegisterState(&MenuRanking);
    RegisterState(&MenuHowToPlay);
    RegisterState(&MenuCredit);
    // Register pause state
    RegisterPauseState(&pauseLevel);

    // Set starting state
    SetFirstState(&splash);

    // Initlialize current state
    GetCurrentState()->Initialize();

    return true;
}

/**
 * \brief
 * Update state manager
 *
 * \param dt
 * Delta time
 */
void Game::Update(float dt)
{
    StateManager::SetWindowSize(m_width, m_height);
    m_stateManager->Update(dt);
}

/**
* \brief
* Wrap all state stacks up
*/
void Game::Close()
{
    m_stateManager->Close();
}

/**
* \brief
* Set a state as a pause state
*
* \param state
* Address of state to set as the pause state
*/
void Game::RegisterPauseState(State* state)
{
    m_stateManager->RegisterPauseState(state);
}

/**
* \brief
* Stop updating current state and move to pause state
*/
void Game::Pause()
{
    m_stateManager->Pause();
}

/**
* \brief
* Close pause state and move back to last state
*/
void Game::Resume()
{
    m_stateManager->Resume();
}

/**
* \brief
* Restart current state
*/
void Game::Restart()
{
    m_stateManager->Restart();
}

/**
* \brief
* Add a state
*
* \param state
* Address of state to add
*/
void Game::RegisterState(State* state)
{
    m_stateManager->RegisterState(state);
}

/**
* \brief
* Set a state to starting point
*
* \param state
* Address of state to set as the first state
*/
void Game::SetFirstState(State* state)
{
    m_stateManager->SetFirstState(state);
}

/**
* \brief
* Return the address of the current state
*
* \return m_stack.top()
*/
State* Game::GetCurrentState()
{
    return m_stateManager->GetCurrentState();
}

/**
* \brief
* Set game state manager to shutdown
*/
void Game::Quit()
{
    m_stateManager->Quit();
}

/**
* \brief
* Change current state to other state
*
* \param stateId
* Id of state to change
*/
void Game::Change(unsigned stateId)
{
    m_stateManager->Change(stateId);
}


/**
 * \brief
 * Check either if the app gets ready to quit
 *
 * \return m_stateManager->IsQuit()
 */
bool Game::IsQuit()
{
    return m_stateManager->IsQuit();
}
