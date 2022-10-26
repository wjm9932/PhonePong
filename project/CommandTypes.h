#pragma once
#include <string>

enum class eCommandTypes
{
    INVALID = 0,
    ChangeStateCommand,
    QuitCommand,
    CheckPointCommand,
    DamageCommand,
    Count
};


inline eCommandTypes StringToCommand(const std::string& string)
{
    if (string == "ChangeStateCommand")				return eCommandTypes::ChangeStateCommand;
    if (string == "QuitCommand")				    return eCommandTypes::QuitCommand;
    if (string == "CheckPointCommand")				return eCommandTypes::CheckPointCommand;
    if (string == "DamageCommand")				    return eCommandTypes::DamageCommand;
    return eCommandTypes::INVALID;
}

inline std::string CommandToString(eCommandTypes type)
{
    if (type == eCommandTypes::ChangeStateCommand)				return "ChangeStateCommand";
    if (type == eCommandTypes::QuitCommand)				        return "QuitCommand";
    if (type == eCommandTypes::CheckPointCommand)				return "CheckPointCommand";
    if (type == eCommandTypes::DamageCommand)				    return "DamageCommand";

    return "Invalid";
}