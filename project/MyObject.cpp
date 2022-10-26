/*
File name: MyObject.cpp
Project name: Phone! Pong!
Primary Author: Shinil Kang
Copyright information:
    "All content © 2019 DigiPen (USA) Corporation, all rights reserved."
*/

#include "MyObject.h"
#include <engine/State.h>


MyObject::MyObject()
	:source(GetSource()), physicsWorld(nullptr), scale(0.f, 0.f)
{
}

MyObject::~MyObject()
{
}

MyObject::MyObject(const MyObject* object)
	:source(object->source), physicsWorld(object->physicsWorld), scale(object->scale)
{
	// reference constructor
	SetName(object->GetName().c_str());
	transform.position = object->transform.position;
	transform.SetScale(object->GetScale());
	transform.rotation = object->transform.rotation;

	transform.rotation = 0;
	sprite.color = SDL_Color{ 0, 255, 255, 255 };
    sprite.Free();
	sprite.LoadImage(object->GetSource().c_str(), State::m_renderer);
	sprite.color.r = sprite.color.g
		= sprite.color.b = 0X88;
	physics.bodyType = Physics::STATIC;

	if (GetPhysicsWorld() != nullptr)
	{
		physics.GenerateBody(GetPhysicsWorld(), &transform);
		physics.ActiveGhostCollision(object->physics.IsActiveGhost());
	}
}

MyObject::MyObject(MyObject* object)
	:source(object->source), physicsWorld(object->physicsWorld), scale(object->scale)
{
	// reference constructor
	SetName(object->GetName().c_str());
	transform.position = object->transform.position;
	transform.SetScale(object->GetScale());
	transform.rotation = object->transform.rotation;

	transform.rotation = 0;
	sprite.color = object->sprite.color;
    sprite.Free();
	sprite.LoadImage(object->GetSource().c_str(), State::m_renderer);
	physics.bodyType = Physics::STATIC;

	if (object->sprite.activeAnimation)
	{
		sprite.activeAnimation = true;
		sprite.SetFrame(object->sprite.GetFrame());
		sprite.SetSpeed(object->sprite.GetSpeed());
		sprite.isHud = false;
	}

	if (GetPhysicsWorld() != nullptr)
	{
		physics.GenerateBody(GetPhysicsWorld(), &transform);
		physics.ActiveGhostCollision(object->physics.IsActiveGhost());
	}
}

void MyObject::SetSource(std::string str)
{
	source = str;
}

std::string MyObject::GetSource() const
{
	return source;
}

void MyObject::SetPhysicsWorld(b2World* tmp)
{
	physicsWorld = tmp;
}

b2World* MyObject::GetPhysicsWorld() const
{
	return physicsWorld;
}

void MyObject::SetScale(b2Vec2 vector)
{
	scale = vector;
}

b2Vec2 MyObject::GetScale() const
{
	return scale;
}

MyObject::MyObject(MyObject&& source) noexcept
	:source(source.GetSource()), physicsWorld(source.GetPhysicsWorld()), scale(source.GetScale())
{
	// reference constructor
	SetName(source.GetName().c_str());
	transform.position = source.transform.position;
	transform.SetScale(source.transform.GetScale());
	transform.rotation = source.transform.rotation;


	physics.bodyType = Physics::STATIC;

	transform.rotation = 0;
	sprite.color = SDL_Color{ 0, 255, 255, 255 };
    sprite.Free();
	sprite.LoadImage(source.GetSource().c_str(), State::m_renderer);
	sprite.color.r = sprite.color.g
		= sprite.color.b = 0X88;
	physics.bodyType = Physics::STATIC;
}

MyObject& MyObject::operator=(const MyObject& object)
{
	// reference constructor
	SetName(object.GetName().c_str());
	transform.position = object.transform.position;
	transform.SetScale(object.GetScale());
	transform.rotation = object.transform.rotation;


	physics.bodyType = Physics::STATIC;

	transform.rotation = 0;
	sprite.color = SDL_Color{ 0, 255, 255, 255 };
    sprite.Free();
    sprite.LoadImage(object.GetSource().c_str(), State::m_renderer);
	sprite.color.r = sprite.color.g
		= sprite.color.b = 0X88;
	physics.bodyType = Physics::STATIC;

	// physics.GenerateBody(object.GetPhysicsWorld(), &transform);

	return *this;
}

void MyObject::Update(float /*dt*/)
{
}
