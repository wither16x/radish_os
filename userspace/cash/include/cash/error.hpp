#pragma once

#include <cash/position.hpp>

#include <melon/string.hpp>
#include <melon/print.hpp>

namespace Cash
{
        void illegalCharacterError(char ch, Position pos);
        void syntaxError(const Melon::String::String &str, Position pos);
        void alreadyDeclaredError(const Melon::String::String &name);
        void notDeclaredError(const Melon::String::String &name);
        void isConstantError(const Melon::String::String &name);
        void indexOutOfRangeError(Melon::Typing::USize index, Melon::Typing::USize limit);

        template<typename T>
        void divisionByZeroError(T x)
        {
                Melon::Print::println("error: cannot divide {} by zero",
                        x
                );
        }
} // namespace Cash