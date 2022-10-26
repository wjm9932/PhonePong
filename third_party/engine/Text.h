/******************************************************************************/
/*!
\file   Text.h
\author Juyong Jeong
\par    email: jeykop14\@digipen.edu
\par    GAM150 demo
\date   2018/03/13

Using SDL2 for rendering text

*****YOU CANNOT MODIFY THIS FILE!!*****
*/
/******************************************************************************/
#pragma once
#include <string>
#include "SDL2\SDL_ttf.h"
#include "Graphic.h"

class Text : public Graphic
{
	friend class State;

public:

    Text() {};
    ~Text() override {};

	// Set text to render with the renderer and pointer to the font
	// TODO: This function is deprecated.
	// Use SetText(const char* text, ...) function instead!
	void SetText(SDL_Renderer* renderer, const char* text, TTF_Font* font);
	// Set text (new method)
	void SetText(const char* text, ...);
	// Free text component
	void Free() override;
	TTF_Font* font = nullptr;

private:

	char		m_textStorage[1024];
	std::string m_text;

	// Update the graphic with the viewport information
	// *dt parameter in here is not used
	void Update(Camera* camera, float dt = 0.f) override;

	Text(const Text&) = delete;
	Text(Text&&) = delete;
	Text& operator=(const Text&) = delete;
	Text& operator=(Text&&) = delete;

};
