#include <internal/exceptions/buffer_overflow.hpp>
#include <string.hpp>

#include <stdio.h>

namespace Melon::Exceptions
{
        BufferOverflow::BufferOverflow(Typing::USize requested_size, Typing::USize buffer_size)
                : requested_size(requested_size), buffer_size(buffer_size)
        {}

        const char *BufferOverflow::what() const noexcept
        {
                static char message[256];
                
                snprintf(message, sizeof(message),
                        "buffer size %lu cannot handle requested size %lu",
                        this->buffer_size, this->requested_size
                );

                return message;
        }
} // namespace Melon::Exceptions