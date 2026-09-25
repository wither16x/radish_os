#pragma once

#include "../../typing.hpp"

#include <exception>

namespace Melon::Exceptions
{
        /// @brief Thrown when a buffer overflows.
        class BufferOverflow : public std::exception
        {
                Typing::USize requested_size;
                Typing::USize buffer_size;

        public:
                /// @brief Main constructor for class BufferOverflow.
                /// @param requested_size size requested by the user
                /// @param buffer_size size of the buffer
                BufferOverflow(Typing::USize requested_size, Typing::USize buffer_size);

                /// @return a formatted message describing the error
                const char *what() const noexcept;
        };
} // namespace Melon::Exceptions