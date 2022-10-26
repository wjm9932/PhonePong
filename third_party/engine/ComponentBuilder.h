/******************************************************************************/
/*!
\file   ComponentBuilder.h
\author Hun Yang
\par    email: hun.yang8456@gmail.com
\par    GAM150 demo
\date   2019/03/26

This is component builder for dynamic factory.

*****YOU CANNOT MODIFY THIS FILE!!*****
*/
/******************************************************************************/
#pragma once

class Component;

/**
 * \brief 
 * Base builder class for Factory
 */
class ComponentBuilder
{
public:
	~ComponentBuilder() {}
	virtual Component* Build(void) = 0;
};

/**
 * \brief 
 * Templated builder derived class
 * 
 * \tparam T
 * Component type 
 */
template <typename T>
class ComponentTBuilder : public ComponentBuilder
{
public:
	Component * Build(void) override;
};

/**
 * \brief 
 * Create a new component of type T
 * 
 * \return
 * allocated new component 
 */
template <typename T>
Component* ComponentTBuilder<T>::Build()
{
	return new T();
}
