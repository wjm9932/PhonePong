/*
File name: MyObject.h
Project name: Phone! Pong!
Primary Author: Shinil Kang
Copyright information:
    "All content © 2019 DigiPen (USA) Corporation, all rights reserved."
*/

#pragma once
#include <engine/Object.h>


class MyObject : public Object
{
public:
	MyObject();
	virtual ~MyObject();

	MyObject(const MyObject*);
	MyObject(MyObject*);

	void SetSource(std::string str);
	std::string GetSource(void) const;

	void SetPhysicsWorld(b2World* tmp);
	b2World* GetPhysicsWorld(void) const;

	void SetScale(b2Vec2 vector);
	b2Vec2 GetScale(void)const;

	MyObject(MyObject&&) noexcept;
	MyObject& operator=(const MyObject&);
	MyObject& operator=(MyObject&&) noexcept = delete;

	virtual void Update(float dt);

private:
	std::string source;
	b2World* physicsWorld;
	b2Vec2 scale = b2Vec2{ 0.f, 0.f };
};
