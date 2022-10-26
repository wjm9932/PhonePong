/******************************************************************************/
/*!
\file   ContactListener.h
\author Juyong Jeong
\par    email: jeykop14\@gmail.com
\par    GAM150 demo
\date   2018/05/28

Containing contact listener class declaration

*****YOU CANNOT MODIFY THIS FILE!!*****
*/
/******************************************************************************/

#pragma once
#include "Box2D/Dynamics/b2WorldCallbacks.h"

class Physics;
class ContactListener : public b2ContactListener
{
	friend class State;

	ContactListener() {};
	~ContactListener() {};

	void BeginContact(b2Contact* contact) override;
	void EndContact(b2Contact* contact) override;

	void HandleContact(Physics* p1, Physics* p2);
};