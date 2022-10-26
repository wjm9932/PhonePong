/******************************************************************************/
/*!
\file   Application.cpp
\author David Ly
\par    email: dly\@digipen.edu
\par    GAM150 demo
\par	v0
\date   2018/03/11

Handles the creation of an application window and handling OS events.
*/
/******************************************************************************/

#include "Application.h"
#include <sstream>
#include "SDL2/sdl_mixer.h"
#include "SDL2\SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include "engine\Debug.h"

#include "iostream"

/**
 * \brief
 * Initializes the application and all systems that the game requires
 * 
 * \return
 * TRUE if successful, FALSE otherwise
 */
bool Application::CanStartUp()
{
	//SDL initialization, create window, create render
	auto init_result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER );	// https://wiki.libsdl.org/SDL_Init SDL_INIT_JOYSTICK SDL_INIT_EVERYTHING
	if (init_result < 0)
	{
		CheckSDLError(__LINE__);
		return false;
	}

	// https://wiki.libsdl.org/SDL_SetHint, https://wiki.libsdl.org/CategoryHints#Hints
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"), CheckSDLError(__LINE__);
	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1"), CheckSDLError(__LINE__);

	// https://wiki.libsdl.org/SDL_CreateWindow
	window = SDL_CreateWindow("GAM150 Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE),
		CheckSDLError(__LINE__);
	if (window == nullptr)
		return false;

	// https://wiki.libsdl.org/SDL_CreateRenderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC), CheckSDLError(__LINE__);
	if (renderer == nullptr)
		return false;

	// Pass SDL_Renderer to the base State class
	State::m_renderer = renderer;
	// Pass Input to the base State class
	State::m_input = &m_input;
	// Pass Game to the base State class
	State::m_game = &game;

	isDone = false;
	ticks_last = SDL_GetTicks();

	// Initialize png loading
	int imgFlag = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlag) & imgFlag)) 
	{
		CheckSDLError(__LINE__);
		return false;
	}

	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) 
	{
		CheckSDLError(__LINE__);
		return false;
	}

	//Initializes text rendering
	if (TTF_Init() == -1) 
	{
		CheckSDLError(__LINE__);
		return false;
	}

	// Get display screen size
	GetScreenSize();

	return game.Initialize();
}

/**
 * \brief 
 * Polling the events of the OS is done here and passed on to the game.
 * No game logic should be done here.
 */
void Application::Update()
{
	if (isDone)	//checks if the application has quit.
		return;

	Input::ResetStatus(); // This must be called to properly update the store status of keys

	SDL_GetWindowSize(window, &game.m_width, &game.m_height);

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			isDone = true;
			break;
		case SDL_KEYDOWN:
			m_input.SetKeyPressed(event.key.keysym.scancode, SDL_KEYDOWN);
			break;
		case SDL_KEYUP:
			m_input.SetKeyPressed(event.key.keysym.scancode, SDL_KEYUP);
			break;
		case SDL_WINDOWEVENT:
			break;
		case SDL_MOUSEBUTTONDOWN:
			//fall through to next case
		case SDL_MOUSEBUTTONUP:
			m_input.SetMousePressed(event.button);
			break;
		case SDL_MOUSEMOTION:
			m_input.SetMousePos(static_cast<float32>(event.button.x), static_cast<float32>(event.button.y));
			break;
		}
	}		

	//Update toggle form game 
	isDone = game.IsQuit();

	// Still app is turned off by esc key
	if (m_input.IsPressed(SDL_SCANCODE_ESCAPE))
		isDone = true;

	else if (m_input.IsTriggered(SDL_SCANCODE_F)) {
		isFullscreen = !isFullscreen;
		SetFullscreen(isFullscreen);
	}

	else if (m_input.IsTriggered(SDL_SCANCODE_G))
		SDL_SetWindowSize(window, game.m_width, game.m_height);

	//Clock
	Uint32 ticks = SDL_GetTicks();
	float dt = 0.001f * (ticks - ticks_last);
	ticks_last = ticks;

	// frame rate
	++frame_count;
	frame_time += dt;
	if (frame_time >= 1.0)
	{
		auto fps = frame_count / frame_time;
		frame_count = 0;
		frame_time = 0;
		std::stringstream ss;
		ss << "SDL Example [fps=" << static_cast<int>(fps) << "]";
		SDL_SetWindowTitle(window, ss.str().c_str());
	}

	//This is where our game update() gets called
	game.Update(dt);
	SDL_Delay(1);
}

//Cleans up the systems that application was using
Application::~Application()
{
	game.Close();

	// Wrap up sounds
	Mix_Quit();

	// Wrap up graphics
	TTF_Quit();
	IMG_Quit();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Application::CreateDebugConsole()
{
	// Pop console window 
	DEBUG_CREATE_CONSOLE();

	// Check memory leak
	DEBUG_LEAK_CHECKS(-1);
}

void Application::DeleteDebugConsole()
{
	// Delete console window
	DEBUG_DESTROY_CONSOLE();
}

void Application::SetFullscreen(bool toggle)
{
	isFullscreen = toggle;
	SDL_SetWindowFullscreen(window, isFullscreen);
}

int gcd(int a, int b)
{
	// Using euclidean algorithm
	if (a == 0)
		return b;
	else if (b == 0)
		return a;

	if (a < b)
		return gcd(a, b % a);
	else
		return gcd(b, a % b);
}

void Application::GetScreenSize()
{
	SDL_DisplayMode displayMode;

	if (SDL_GetDesktopDisplayMode(0, &displayMode) != 0)
		SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());

	m_screenWidth = displayMode.w;
	m_screenHeight = displayMode.h;

	int denominator = gcd(m_screenWidth, m_screenHeight);
	ratioWidth = m_screenWidth / denominator;
	ratioHeight = m_screenHeight / denominator;

	std::cout << "Screen size: " << m_screenWidth << " " << m_screenHeight << std::endl;
	std::cout << "Ratio: " << ratioWidth << " " << ratioHeight << std::endl;
}