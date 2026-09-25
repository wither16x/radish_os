#pragma once

#include "../../typing.hpp"

#include <exception>

namespace Melon::Exceptions
{
        /// @brief Thrown when an index is out of range.
        class OutOfRange : public std::exception
        {
                Typing::USize index;
                Typing::USize highest_index;

        public:
                /// @brief Main constructor for class OutOfRange.
                /// @param index index out of range
                /// @param highest_index limit of the array
                OutOfRange(Typing::USize index, Typing::USize highest_index);

                /// @return a formatted message describing the error
                const char *what() const noexcept;
        };
} // namespace Melon::Exceptions