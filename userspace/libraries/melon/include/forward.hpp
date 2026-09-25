#pragma once

#include "typing.hpp"

namespace Melon::Forward
{
        template<class T>
        T &&forward(Typing::remove_reference_t<T> &&param)
        {                                   
                return static_cast<T &&>(param);
        }

        template<class T>
        T &&forward(Typing::remove_reference_t<T> &param)
        {                                   
                return static_cast<T &&>(param);
        }
} // namespace Melon::Forward