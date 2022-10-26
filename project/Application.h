/******************************************************************************/
/*!
\file   Application.h
\author David Ly
\par    email: dly\@digipen.edu
\par    GAM150 demo
\par	v0
\date   2018/03/11

Handles the creation of an application window and interpreting OS events.
*/
/******************************************************************************/
#pragma once
#include "SDL2/SDL.h"
#include "Game.h"
#include "engine/Input.h"

void CheckSDLError(int line = -1);

class Application
{
public:
	Application() = default;
	bool CanStartUp();	//Initializes the application if no errors
	bool IsDone() const { return isDone; }
	void Update();
	~Application();

	void CreateDebugConsole();
	void DeleteDebugConsole();
	void SetFullscreen(bool toggle);
	void GetScreenSize();

// This prevents new call on more than 1 instance.
public:
	Application(const Application&) = delete;
	Application(Application&&) = delete;
	Application& operator=(const Application&) = delete;
	Application& operator=(Application&&) = delete;

private:

	bool isFullscreen = false;

	SDL_Window *window = nullptr;
	SDL_Renderer *renderer = nullptr;

	bool isDone = true;	//If the application is done
	int  ratioWidth, ratioHeight;

	//For keeping track of time
	Uint32 ticks_last = 0;
	int frame_count = 0;
	float frame_time = 0.0f;

	// Screen size
	int m_screenWidth = 0, m_screenHeight = 0;
	
	Game game{};	//Holds one instance of our game
	Input m_input{};//Input class
};