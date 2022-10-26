/******************************************************************************/
/*!
\file   Component.h
\author Hun Yang
\par    email: hun.yang8456@gmail.com
\par    GAM150 demo
\date   2019/03/25

This is base component class.

*****YOU CANNOT MODIFY THIS FILE!!*****
*/
/******************************************************************************/
#pragma once
#include "ComponentTypes.h"


class Object;

class Component
{
public:
	Component(eComponentTypes type);
	virtual ~Component(void);
    // Initialize the component info
	virtual void    Initialize  (void)      = 0;
    // per frame behavior of component
	virtual void    Update      (float dt)  = 0;

    // Set pointer to the parent object
	void                SetOwner    (Object* owner);
    // Get pointer to the parent object
	Object*             GetOwner    (void);
    // Get component type
	eComponentTypes		GetType		(void) const;
    // Set component state (either it is active or not)
	void				SetState	(bool active);
    // Get component state
	bool			    GetState	(void) const;
private:
    Component(const Component& rhs)             = delete;
    Component& operator=(const Component& rhs)  = delete;
protected:
	Object*			    mOwner;         // Pointer to the parent object
private:
	eComponentTypes		mType;          // Component type
	bool				mbActive;       // Component active toggle
};
