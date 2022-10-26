/******************************************************************************/
/*!
\file   StateManager.h
\author Juyong Jeong
\par    email: jeykop14\@digipen.edu
\par    GAM150 demo
\date   2018/04/16

Containing state manager class declaration

*****YOU CANNOT MODIFY THIS FILE!!*****
*/
/******************************************************************************/

#pragma once
#include <stack>
#include <vector>

class State;

class StateManager
{
	using States = std::vector<State*>;		
	using StateStack = std::stack<State*>;			

	States		m_states;	// State storage
	StateStack	m_stack;	// State to be updated (top)

	// Pointer to specific states
	State		*m_pNext = nullptr, *m_pFirst = nullptr, 
				*m_pPaused = nullptr, *m_pResume = nullptr;

	// State status
	bool		m_quit, m_restart;

public:

	StateManager() = default;
	~StateManager() = default;

	// Register state to vector storage
	void	RegisterState(State* state);
	// Register a state as a first state
	void	SetFirstState(State* state);
	// Get pointer to current state
	State*	GetCurrentState();
	// Quit application
	void	Quit();
	// Change to other state with state id
	void	Change(unsigned stateId);
	// Update state
	void	Update(float dt);
	// Close state
	void	Close();
	// Register pause state 
	void	RegisterPauseState(State *state);
	// Move to pause state
	void	Pause();
	// Move to last state from pause state
	void	Resume();
	// Restart current state
	void	Restart();
	// Check either if the quit toggle is true or not
	bool	IsQuit();
    // Check either if the restart toggle is true or not
    bool    IsRestart();
	// Convey the window size info
	static void	SetWindowSize(int w, int h);

private:

	StateManager(const StateManager&) = delete;
	StateManager(StateManager&&) = delete;
	StateManager& operator=(const StateManager&) = delete;
	StateManager& operator=(StateManager&&) = delete;
};