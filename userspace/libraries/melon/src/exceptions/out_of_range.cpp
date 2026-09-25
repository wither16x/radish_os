#include <internal/exceptions/out_of_range.hpp>
#include <typing.hpp>

#include <stdio.h>

namespace Melon::Exceptions
{
        OutOfRange::OutOfRange(Typing::USize index, Typing::USize highest_index)
                : index(index), highest_index(highest_index)
        {}

        const char *OutOfRange::what() const noexcept
        {
                static char message[256];
                
                snprintf(message, sizeof(message),
                        "index %lu out of range: highest index is %lu",
                        this->index, this->highest_index
                );

                return message;
        }
} // namespace Melon::Exceptions