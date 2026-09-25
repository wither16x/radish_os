#pragma once

#include "exception.hpp"

namespace Melon::Exceptions
{
        /// @brief Thrown if an IO stream is null.
        class NullStream : public Exception
        {
        public:
                /// @brief Main constructor for class NullStream.
                NullStream();

                /// @return a formatted message describing the error
                const char *what() const noexcept;
        };
} // namespace Melon::Exceptions