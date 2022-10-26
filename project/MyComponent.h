#pragma once
#include "Component.h"
#include "MyObject.h"

class MyComponent :
	public Component
{
public:
	MyComponent();
	~MyComponent();

	// Initialize the component info
	virtual void Initialize(void);    
	// per frame behavior of component
	virtual void Update(float dt);

private:
	MyObject* character;
	MyObject* it;
};

