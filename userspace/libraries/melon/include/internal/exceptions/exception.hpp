#pragma once

namespace Melon::Exceptions
{
        class Exception
        {
        public:
                Exception() noexcept = default;
                Exception(const Exception &) noexcept = default;
                
                virtual ~Exception() = default;

                virtual const char *what() const noexcept;

                Exception &operator =(const Exception &) noexcept = default;
        };
} // namespace Melon::Exceptions