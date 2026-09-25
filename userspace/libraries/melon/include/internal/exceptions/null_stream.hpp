#pragma once

#include <exception>

namespace Melon::Exceptions
{
        /// @brief Thrown if an IO stream is null.
        class NullStream : public std::exception
        {
        public:
                /// @brief Main constructor for class NullStream.
                NullStream();

                /// @return a formatted message describing the error
                const char *what() const noexcept;
        };
} // namespace Melon::Exceptions