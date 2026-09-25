#pragma once

#include "string.hpp"

#include <type_traits>
#include <utility>

#include <stdlib.h>

/// @brief Contains functions to convert types into other types.
namespace Melon::Conversion
{
        /// @brief All conversion functions that take integers should support these bases.
        enum class Base
        {
                Binary          = 2,
                Octal           = 8,
                Decimal         = 10,
                Hexadecimal     = 16
        };

        /// @brief Used to treat values as binary values.
        template<typename T>
        struct BinaryValue
        {
                T value;
                static constexpr Base base = Base::Binary;
        };

        /// @brief Used to treat values as octal values.
        template<typename T>
        struct OctalValue
        {
                T value;
                static constexpr Base base = Base::Octal;
        };

        /// @brief Used to treat values as hexadecimal values.
        template<typename T>
        struct HexadecimalValue
        {
                T value;
                static constexpr Base base = Base::Hexadecimal;
        };

        /// @brief Convert a value to a BinaryValue.
        template<typename T>
        BinaryValue<T> bin(T value)
        {
                return {value};
        }

        /// @brief Convert a value to an OctalValue.
        template<typename T>
        OctalValue<T> oct(T value)
        {
                return {value};
        }

        /// @brief Convert a value to a HexadecimalValue.
        template<typename T>
        HexadecimalValue<T> hex(T value)
        {
                return {value};
        }

        /// @brief Convert a base to a raw string.
        /// @param base base to convert
        /// @return corresponding string
        const char *baseToString(Base base);

        /// @brief Convert an integer to a String::String.
        /// @param value integer to convert
        /// @param base base
        /// @return new string
        template<typename T>
                requires std::is_signed_v<T>
        String::String intToString(T value, Base base)
        {
                String::String str;
                String::String digits = "0123456789abcdef";

                bool is_negative = false;

                if (value < 0) {
                        is_negative = true;
                        value = -value;
                }

                if (value == 0) {
                        str.appendChar('0');
                        return str;
                }

                while (value != 0) {
                        T remaining = value % std::to_underlying(base);
                        str.appendChar(digits[remaining]);
                        value = value / std::to_underlying(base);
                }

                if (is_negative)
                        str.appendChar('-');

                return str.reverse();
        }

        /// @brief Convert an unsigned integer to a String::String.
        /// @param value unsigned integer to convert
        /// @return corresponding string
        template<typename T>
                requires std::is_unsigned_v<T>
        String::String uintToString(T value, Base base)
        {
                String::String str;
                String::String digits = "0123456789abcdef";

                if (value == 0) {
                        str.appendChar('0');
                        return str;
                }

                while (value != 0) {
                        T remaining = value % std::to_underlying(base);
                        str.appendChar(digits[remaining]);
                        value = value / std::to_underlying(base);
                }

                return str.reverse();
        }

        /// @brief Convert a string into a signed integer.
        /// @param str string to convert
        /// @return corresponding integer
        template<typename T>
                requires std::is_signed_v<T>
        T stringToInt(const String::String &str)
        {
                return atoi(str.raw());
        }
} // namespace Melon::Conversion