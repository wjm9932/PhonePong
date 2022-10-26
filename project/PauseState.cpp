#include "CommonLevel.h"

void PauseState::Initialize()
{
	m_backgroundColor = {0,0,0,255};

	// Set Camera
	// You don't need to do this anymore
	//camera.Initialize(int(State::m_width), int(State::m_height));
	// Set position of the camera
	camera.position.Set(0, 0, 0);

	// Set main font
	// Load the font file to use as a main font
	// and set the default size of it
	mainFont = TTF_OpenFont("font/Default.ttf", 48);

	// Set the object's name
	text.SetName("Title");
	// Set the position of the object
	text.transform.position.Set(0.0f, 100.0f, 0.0f);
	// Set the scale of the object
	text.transform.SetScale(800, 130);
	// Set the text to render out
	text.text.SetText(State::m_renderer, "This is paused state", mainFont);
	// Set the colors r,g,b,a (0~255)
	text.text.color = SDL_Color{ 0, 255, 255, 255 };
	// Set either if the object to be hud or not
	text.text.isHud = true;

	text2.SetName("Instruction");
	text2.transform.position.Set(0.0f, -100.0f, 0.0f);
	text2.transform.SetScale(650, 130);
	text2.text.SetText(State::m_renderer, "Press R to go back", mainFont);
	text2.text.color = SDL_Color{ 0, 255, 255, 255 };
	text2.text.isHud = true;

	// Register the object to the state
	AddObject(&text);
	AddObject(&text2);
}

void PauseState::Update(float dt)
{
	/******************* State ******************/

	// Move to level1 by pressing 'R' key
	if (m_input->IsTriggered(SDL_SCANCODE_R))
		m_game->Resume();

	// Must be one of the last functions called at the end of State Update
	UpdatePhysics(dt);
	Render(dt);
}

void PauseState::Close()
{
	// Wrap up state
	ClearBaseState();
}
