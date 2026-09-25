#pragma once

namespace Melon::Memory
{
        template<typename T>
        T *copy(const T *first, const T *last, T *dest)
        {
                while (first != last)
                        *dest++ = *first++;

                return dest;
        }
} // namespace Melon::Memory