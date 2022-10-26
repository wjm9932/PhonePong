/******************************************************************************/
/*!
\file   CommandBuilder.h
\author Hun Yang
\par    email: hun.yang8456@gmail.com
\par    GAM150 demo
\date   2019/04/26

This is command builder for dynamic factory.

*****YOU CANNOT MODIFY THIS FILE!!*****
*/
/******************************************************************************/
#pragma once

class Command;

/**
* \brief
* Base builder class for Factory
*/
class CommandBuilder
{
public:
    ~CommandBuilder() {}
    virtual Command* Build(void) = 0;
};

/**
* \brief
* Templated builder derived class
*
* \tparam T
* Command type
*/
template <typename T>
class CommandTBuilder : public CommandBuilder
{
public:
    Command * Build(void) override;
};

/**
* \brief
* Create a new Command of type T
*
* \return
* allocated new Command
*/
template <typename T>
Command* CommandTBuilder<T>::Build()
{
    return new T();
}
