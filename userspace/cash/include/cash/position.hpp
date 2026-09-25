#pragma once

#include <melon/typing.hpp>

namespace Cash
{
        struct Position
        {
                int row;
                int column;
                Melon::Typing::USize offset;
        };
} // namespace Cash