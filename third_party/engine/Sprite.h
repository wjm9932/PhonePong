/******************************************************************************/
/*!
\file   Sprite.h
\author Juyong Jeong
\par    email: jeykop14\@digipen.edu
\par    GAM150 demo
\date   2018/03/13

Using SDL2 for rendering sprite

*****YOU CANNOT MODIFY THIS FILE!!*****
*/
/******************************************************************************/
#pragma once
#include <vector>
#include "Graphic.h"

class CustomSprite;

class Sprite : public Graphic
{
	friend class State;

	using Frame = std::vector<SDL_Rect>;
    
    Frame	    m_frames;												// Vector of frames for animation
	int			m_imageWidth = 0, m_imageHeight = 0;					// Size of the miage
	unsigned	m_frameIndex = 0, m_numOfFrames = 0;					// Attributes of the animation
	float	    m_dtStack = 0.f, m_speed = 0.f, m_speedInverse = 0.f;   // Attributes of the animation
	bool		m_fixFrame = false;										// Toggle to fix specific frame

public:

    Sprite() {};
    ~Sprite() override {};

	// Load image by passing the file path
    // *Only takes .png files
	void LoadImage(const char* filePath, SDL_Renderer* renderer);
	// Set the number of animation frames
	void SetFrame(unsigned frame);
	// Get the number of frames
	unsigned GetFrame() const;
	// Set the speed of the animation
	void SetSpeed(float speed);
	// Get the speed of the animation
	float GetSpeed() const;
	// Get current frame index
	unsigned GetCurrentFrame() const;
	// Set current frame to specific index
	void SetCurrentFrame(bool fix, unsigned index);
	// Check either if the animation is fixed
	bool IsFixed() const;

	bool activeAnimation = false,	// Animation toggle 
		rewindAnimation = false,	// Rewind toggle
		stopAnimation = false;;		// Stop toggle

	// Flip toggle
	SDL_RendererFlip	flip = SDL_FLIP_NONE;
    Sprite& operator=(CustomSprite&& custom);

private:

	// Update the graphic with the viewport information and dt
	void Update(Camera* camera, float dt) override;

	Sprite(const Sprite&) = delete;
	Sprite(Sprite&&) = delete;
	Sprite& operator=(const Sprite&) = delete;
	Sprite& operator=(Sprite&&) = delete;

};
