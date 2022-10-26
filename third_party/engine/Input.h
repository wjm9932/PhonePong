/******************************************************************************/
/*!
\file   Input.cpp
\author David Ly
\par    email: dly\@digipen.edu
\par    GAM150 demo
\date   2018/03/15

Using SDL2 for interpreting input and checks key status.
Currently only handles keyboard and mouse, but not MouseWheel yet.

*****YOU CANNOT MODIFY THIS FILE!!*****
*/
/******************************************************************************/
#pragma once
#include <SDL2/SDL.h>
#include <bitset>		//using this to store key press status
#include "Box2D/Common/b2Math.h"

#define INVALID_MOUSECODE -1
#define NUM_MOUSECODE 10

typedef int MouseCode;

class Input
{
private:
	//Containers of keyboard key status
	static std::bitset<SDL_NUM_SCANCODES> k_pressed;
	static std::bitset<SDL_NUM_SCANCODES> k_triggered;
	static std::bitset<SDL_NUM_SCANCODES> k_released;

	static b2Vec2 mousePos;
	//Containers of mouse buttons status
	static std::bitset<NUM_MOUSECODE> m_pressed;
	static std::bitset<NUM_MOUSECODE> m_triggered;
	static std::bitset<NUM_MOUSECODE> m_doubleclick;
	static std::bitset<NUM_MOUSECODE> m_released;

public:
	Input();
	~Input();

	//This function resets my triggered and pressed data for keyboard and mouse. This is called once per frame before SDL events.
	static void ResetStatus(void);

	//Keyboard
	//Sets pressed, triggered, released, or unpressed status of a key
	void SetKeyPressed(SDL_Scancode key, SDL_EventType keyStatus);
	//Check if a specific key is held down
	bool IsPressed(SDL_Scancode key);
	//Check if a specific key is triggered
	bool IsTriggered(SDL_Scancode key);
	//Check if a specific key is released
	bool IsReleased(SDL_Scancode key);
	//Check if any key is pressed
	bool IsAnyPressed(void);
	//Check if any key is triggered
	bool IsAnyTriggered(void);
	//Check if any key is released
	bool IsAnyReleased(void);

	//Sets the mouse position as a vector 2D
	void SetMousePos(float32 x, float32 y);
	//Returns the mouse position as a vector 2D
	b2Vec2 GetMousePos();
    b2Vec2 GetMousePosInScreen() const;
	//Sets pressed, triggered, released, or unpressed status of a mouse button
	void SetMousePressed(SDL_MouseButtonEvent);
	//Check if a specific mouse button is held down
	bool IsPressed(MouseCode key);
	//Check if a specific mouse button is triggered
	bool IsTriggered(MouseCode key);
	//Check if a specific mouse button has double clicked
	bool IsDoubleClicked(MouseCode key);
	//Check if a specific mouse button is released
	bool IsReleased(MouseCode key);

public:
	Input(const Input&) = delete;
	Input(Input&&) = delete;
	Input& operator=(const Input&) = delete;
	Input& operator=(Input&&) = delete;

private:
	//Prevents additional instances to be created
	static bool m_instantiated;
};
