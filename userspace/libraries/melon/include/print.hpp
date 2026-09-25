#pragma once

#include "string.hpp"
#include "fmt.hpp"

#include <stdio.h>

/// @brief Contains functions to display text.
namespace Melon::Print
{
        /// @brief Print a formatted string to the standard output stream.
        /// @param str string to format
        /// @param args format values
        template<typename... ARGS>
        void print(const String::String &str, ARGS &&...args)
        {
                String::String formatted;
                Typing::USize idx = 0;
                Fmt::formatArgs(formatted, str, idx, static_cast<ARGS &&>(args)...);
                printf("%s", formatted.raw());
        }

        /// @brief Print a newline-terminated formatted string to the standard output stream.
        /// @param str string to format
        /// @param args format values
        template<typename... ARGS>
        void println(const String::String &str, ARGS &&...args)
        {
                print(str, static_cast<ARGS &&>(args)...);
                print("\n");
        }
} // namespace Melon::Print