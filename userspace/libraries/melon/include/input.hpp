#pragma once

#include "string.hpp"

/// @brief Contains functions to interact with the user from the standard input stream.
namespace Melon::Input
{
        /// @brief Asks the user to type a null-terminated string.
        /// @param str string to print before the user types
        /// @return user text
        String::String input(const String::String &str);
} // namespace Melon::Input