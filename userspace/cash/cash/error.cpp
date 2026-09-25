#include <cash/error.hpp>

#include <melon/print.hpp>

using namespace Melon;

namespace Cash
{
        void illegalCharacterError(char ch, Position pos)
        {
                Print::println("error: illegal character '{}' at line {} and column {}",
                        ch, pos.row, pos.column
                );
        }

        void syntaxError(const Melon::String::String &str, Position pos)
        {
                Print::println("error: invalid syntax at '{}' at line {} and column {}",
                        str, pos.row, pos.column
                );
        }

        void alreadyDeclaredError(const Melon::String::String &name)
        {
                Print::println("error: symbol \"{}\" is already declared",
                        name
                );
        }

        void isConstantError(const Melon::String::String &name)
        {
                Print::println("\"{}\" is a constant", name);
        }

        void notDeclaredError(const Melon::String::String &name)
        {
                Print::println("error: symbol \"{}\" is not declared",
                        name
                );
        }

        void indexOutOfRangeError(Melon::Typing::USize index, Melon::Typing::USize limit)
        {
                Print::println("error: index {} out of range, highest index is {}",
                        index, limit
                );
        }
} // namespace Cash