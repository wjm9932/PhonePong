/******************************************************************************/
/*!
\file   Particle.h
\author Juyong Jeong
\par    email: jeykop14\@digipen.edu
\par    GAM150 demo
\date   2018/04/25

Using SDL2 for rendering emitter/particle

*****YOU CANNOT MODIFY THIS FILE!!*****
*/
/******************************************************************************/

#pragma once

#include <vector>
#include "Graphic.h"
#include "Transform.h"
#include "Box2D/Box2D.h"

class Emitter : public Graphic
{
	friend class State;

	// Particle struct
	struct Particle
	{
		// Individual particle information
		bool		dead = false;
		b2Vec3		color;
		b2Vec2		velocity;
		Transform	transform;
		float		rotationSpeed = 0.f, life = 255.f, dtStack = 0.f;
		SDL_Rect    quad{ 0, 0, 64, 64 }; 
	};

	// Particle storage 
	using Particles = std::vector<Particle*>;

public:

	// Particle update type
	enum ParticleType { PARTICLE_NORMAL, PARTICLE_EXPLOSION, PARTICLE_SNOW };

	Emitter();
	~Emitter();

	// Load particle image by passing the file path
	// *Only takes .png files
	void LoadImage(const char* filePath, SDL_Renderer* renderer);
	// Set color of center and outside
	void SetColors(const b2Vec3& start, const b2Vec3& end);
	// Set range of starting position 
	void SetStartingRange(float xRange, float yRange);
	// Generate particles
	void GenerateParticles(unsigned size, Transform* transform);
	// Set active toggle
	void SetActive(bool active);
	// Check either if emitter is active 
	bool IsActive() const;
	// Set range of velocity
	void SetVelocity(const b2Vec2& xRange, const b2Vec2& yRange);
	// Set direction
	void SetDegree(float degree);
	// Get degree
	float GetDegree() const;

	float			rotationSpeed, life;
	ParticleType	type;
	SDL_BlendMode	blendMode = SDL_BLENDMODE_BLEND;

private: 

	b2Vec3		m_outsideColor, m_centerColor, m_colorDiff;
	b2Vec2		m_xVelocity, m_yVelocity, direction;
	float		m_xRange, m_yRange, m_degree;
	unsigned	m_size, deadCount;
	Particles	m_particles;
	bool		m_active;

	// Render individual particle
	void RenderParticles(Particle* particle, SDL_Renderer * renderer, Camera * camera);
	// Refresh particle by emitter's type
	void RefreshParticleByType(Particle* particle);
	// Refresh a particle when it is dead
	void RefreshParticle(Particle* particle);
	// Clear particles
	void Free() override;

	// Update the graphic with the viewport information and dt
	void Update(Camera* camera, float dt) override;

};