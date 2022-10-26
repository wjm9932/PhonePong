#include "Component.h"


/**
 * \brief 
 * Set the type of component
 * 
 * \param type
 * The type of component
 */
Component::Component(eComponentTypes type):
	mOwner(nullptr),
	mType(type),
	mbActive(true)
{

}

Component::~Component()
{
}

/**
 * \brief 
 * Set the pointer to the parent object
 * 
 * \param owner 
 * the pointer to the parent object
 */
void Component::SetOwner(Object* owner)
{
	mOwner = owner;
}

/**
 * \brief 
 * Get the poniter to the parent object
 * 
 * \return mOwner
 * the pointer to the parent object
 */
Object* Component::GetOwner()
{
	return mOwner;
}

/**
 * \brief 
 * Get the type of component
 * 
 * \return mType
 * The type of component 
 */
eComponentTypes Component::GetType() const
{
	return mType;
}

/**
 * \brief 
 * Set active toggle of component
 * so object manager ether update thsi component or not
 * 
 * \param active 
 * Set true to activate or false to inactivate
 */
void Component::SetState(bool active)
{
	mbActive = active;
}

/**
 * \brief 
 * Check either if this component is activated or not
 * 
 * \return mbActive
 */
bool Component::GetState() const
{
	return mbActive;
}
