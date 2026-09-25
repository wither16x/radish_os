#include <internal/exceptions/exception.hpp>

namespace Melon::Exceptions
{
        const char *Exception::what() const noexcept
        {
                return "Melon::Exception";
        }
} // namespace Melon::Exceptions