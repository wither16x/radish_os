#pragma once

#include <lib/memory.hpp>

namespace Kiwi::Lib
{
        template<class T>
        T&& forward(RemoveReferenceType<T> &&param)
        {                                   
                return static_cast<T&&>(param);
        }

        template<class T>
        T&& forward(RemoveReferenceType<T> &param)
        {                                   
                return static_cast<T&&>(param);
        }
} // namespace Kiwi::Lib