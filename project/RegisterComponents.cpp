#include "RegisterComponents.h"
#include "engine/ComponentBuilder.h"
#include "ComponentTypes.h"
#include "MyComponent.h"
#include <engine/State.h>

/**
 * \brief 
 * Register each component builder
 */
void RegisterComponents()
{
	State::AddComponentBuilder(eComponentTypes::MyComponent,
		new ComponentTBuilder<MyComponent>());
}