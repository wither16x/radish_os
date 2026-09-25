#pragma once

#include "../../typing.hpp"

namespace Melon::Memory
{
        template<typename T>
        constexpr Typing::remove_reference_t<T> &&move(T &&value)
        {
                return static_cast<Typing::remove_reference_t<T> &&>(value);
        }
} // namespace Melon::Memory