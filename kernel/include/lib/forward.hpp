#pragma once

#include <lib/memory.hpp>

namespace Kiwi::Lib
{
        template<class T>
        T &&forward(remove_reference_t<T> &&param)
        {                                   
                return static_cast<T &&>(param);
        }

        template<class T>
        T &&forward(remove_reference_t<T> &param)
        {                                   
                return static_cast<T &&>(param);
        }
} // namespace Kiwi::Lib