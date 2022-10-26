/******************************************************************************/
/*!
\file   ComponentTypes.h
\author Hun Yang
\par    email: hun.yang8456@gmail.com
\par    GAM150 demo
\date   2019/04/30


*/
/******************************************************************************/
#pragma once

#include <string>

/**
* \brief
* This is enum class for component types
*/
enum class eComponentTypes
{
    INVALID = 0,
    DemoLv1Controller,
	MyComponent,

    NUM_COMPONENTS
};

/**
* \brief
* Convert string to eComponentTypes's component
*
* \param string
* String to be  converted for component type
*
* \return
* Converted component type
*/
inline eComponentTypes StringToComponent(const std::string& string)
{
    if (string == "MyComponent")			    return eComponentTypes::MyComponent;
    return eComponentTypes::INVALID;
}

/**
* \brief
* Convert eComponentTypes's component to string
*
* \param type
* Component type to be converted for string
*
* \return
* Converted string
*/
inline std::string ComponentToString(eComponentTypes type)
{
	if (type == eComponentTypes::DemoLv1Controller)		        return "DemoLv1Controller";
	if (type == eComponentTypes::MyComponent)		        return "MyComponent";

    return "Invalid";
}