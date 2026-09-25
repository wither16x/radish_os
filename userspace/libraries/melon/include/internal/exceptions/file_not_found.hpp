#pragma once

#include "../../string.hpp"

#include <exception>

/// @brief Contains custom exceptions.
///
/// This namespace contains a bunch of useful exceptions to handle most errors
/// as possible.
namespace Melon::Exceptions
{
        /// @brief Thrown when a file was not found.
        class FileNotFound: public std::exception
        {
                char *path;

        public:
                /// @brief Main constructor.
                /// @param path path to the file
                FileNotFound(const String::String &path);

                /// @return a formatted message describing the error
                const char *what() const noexcept;
        };
} // namespace Melon::Exceptions