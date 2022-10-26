#include "MyComponent.h"



MyComponent::MyComponent()
	: Component(eComponentTypes::MyComponent)
{
}


MyComponent::~MyComponent()
{
}

void MyComponent::Initialize()
{
}

void MyComponent::Update(float dt)
{
	if (character->physics.HasBody())
	{
		if (character->physics.IsColliding())
		{
			if (character->physics.IsCollidingWith(character))
			{
				(dt);
			}
		}
	}
}
