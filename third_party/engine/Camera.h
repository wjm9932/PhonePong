/******************************************************************************/
/*!
\file   Camera.h
\author Juyong Jeong
\par    email: jeykop14\@digipen.edu
\par    GAM150 demo
\date   2018/03/13

Using SDL2 for contriolling viewport

*****YOU CANNOT MODIFY THIS FILE!!*****
*/
/******************************************************************************/
#pragma once
#include "SDL2\SDL.h"
#include "Box2D\Common\b2Math.h"

class Transform;
class Camera
{
	friend class Text;
	friend class Sprite;
	friend class Emitter;

	// Set every graphics to the correct coordinates
	void Update(SDL_Rect* quad, Transform* pSpriteTransform, bool isHud);

	b2Vec3	m_realPos{ 0, 0, 0 };			// Camera's real position
	float	m_zFar = 500.f, m_zNear = 0.01f;// Camera's visible z area
	int		m_height, m_width;				// Screen size

public:
	Camera() {};
	~Camera() {};

	// Initialize camera info by passing window size
	// TODO: This function is deprecated. 
	// You should not use this function anymore!
	void Initialize(int width, int height);

	b2Vec3	position{ 0, 0, 0 };// Camera position
	float	rotation = 0.f;		// Camera rotation

	Camera(const Camera&) = delete;
	Camera(Camera&&) = delete;
	Camera& operator=(const Camera&) = delete;
	Camera& operator=(Camera&&) = delete;
};
