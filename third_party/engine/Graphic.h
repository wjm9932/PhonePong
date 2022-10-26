/******************************************************************************/
/*!
\file   Graphic.h
\author Juyong Jeong
\par    email: jeykop14\@digipen.edu
\par    GAM150 demo
\date   2018/03/15

This is virtual base graphic object class for derived classes(Text and Sprite)

*****YOU CANNOT MODIFY THIS FILE!!*****
*/
/******************************************************************************/
#pragma once
#include "SDL2\SDL.h"

class Camera;
class Transform;

class Graphic {

protected:

	friend class State;

	Graphic();
	virtual ~Graphic();

	SDL_Rect        m_quad{ 0, 0, 64, 64 }; // Quad where texture to be rendered
    SDL_Texture*    m_texture = nullptr;	// Texture to store image
    SDL_Texture*    m_original_texture = nullptr;	// Original Texture 
	SDL_Renderer*	m_renderer;				// Pointer to renderer

public:

	// Decallocate the texture
	virtual void Free();
	// Get texture as a pointer
	SDL_Texture* GetTexture();

	SDL_Color	color{ 255, 255, 255, 255 };	// Color of texture (0X00 - 0XFF)
	Transform*	pTransform = nullptr;			// Pointer to transform
	bool		isHud = false;					// Hud toggle
    bool        isVisible = true;               // Visible toggle
private:

	// Update the graphic with the viewport information and dt
	virtual void Update(Camera* camera, float dt) = 0;

	Graphic(const Graphic&) = delete;
	Graphic(Graphic&&) = delete;
	Graphic& operator=(const Graphic&) = delete;
	Graphic& operator=(Graphic&&) = delete;
};
