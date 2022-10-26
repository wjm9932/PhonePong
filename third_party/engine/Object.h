/******************************************************************************/
/*!
\file   Object.h
\author Juyong Jeong
\par    email: jeykop14\@digipen.edu
\par    GAM150 demo
\date   2018/03/16

This is base object class containing component classes

*****YOU CANNOT MODIFY THIS FILE!!*****
*/
/******************************************************************************/
#pragma once
#include "SoundEffect.h"
#include "Text.h"
#include "Sprite.h"
#include "Transform.h"
#include "Physics.h"
#include "Particle.h"
#include "ComponentTypes.h"

class Component;
class Object {

	friend class State;
	using ComponentVector = std::vector<Component*>;

	unsigned    m_id = 0;	        // Object unique id - By Engine
	std::string m_name = "Unnamed";	// Object name - By User

public:

	Object();
	virtual ~Object();

	bool active = true;		// Object active toggle
    bool isDead = false;    // Object dead toggle

	// Components
	SoundEffect	sound;		// Sound component
	Text		text;		// Text component
	Sprite		sprite;		// Sprite component
	Transform	transform;	// Transform component
	Physics		physics;	// Physics component
	Emitter		emitter;	// Emitter component

	// Manage name and id
	void				SetName(const char* name);
	const std::string&	GetName() const;
	unsigned			GetId() const;

	template <typename T>
	void GetComponent(eComponentTypes type, T*& pComp);
	template <typename T>
	void GetAllComponents(eComponentTypes type, std::vector<T*>& comps);
    virtual void Shutdown();
protected:
	void			AddComponent(Component* component);
    virtual void Initialize();
private:

	// Virtual functions to be used for derived Object classes
	virtual void Update(float dt);
    void UpdateComponents(float dt);
	virtual void Close();

	Object(const Object&) = delete;
	Object(Object&&) = delete;
	Object& operator=(const Object&) = delete;
	Object& operator=(Object&&) = delete;

	void			RemoveAllComponents(void);
private:
	ComponentVector		mComponents;

};


template <typename T>
void Object::GetComponent(eComponentTypes type, T*& pComp)
{
	for (size_t i = 0; i < mComponents.size(); ++i)
	{
		if (mComponents[i]->GetType() == type)
		{
			pComp = dynamic_cast<T*>(mComponents[i]);
			return;
		}
	}
	pComp = nullptr;
}

template <typename T>
void Object::GetAllComponents(eComponentTypes type, std::vector<T*>& comps)
{
	for (size_t i = 0; i < mComponents.size(); ++i)
	{
		if (mComponents[i]->GetType() == type)
		{
			comps.push_back(dynamic_cast<T*>(mComponents[i]));
		}
	}
}