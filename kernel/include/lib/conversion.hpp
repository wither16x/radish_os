#pragma once

#include <lib/typing.hpp>
#include <lib/string.hpp>

namespace Kiwi::Lib
{
        enum class Base
        {
                Binary          = 2,
                Octal           = 8,
                Decimal         = 10,
                Hexadecimal     = 16
        };

        const char *baseToString(Base base);

        template<typename T>
        struct BinaryValue
        {
                T value;
                static constexpr Base base = Base::Binary;
        };

        template<typename T>
        struct OctalValue
        {
                T value;
                static constexpr Base base = Base::Octal;
        };

        template<typename T>
        struct HexadecimalValue
        {
                T value;
                static constexpr Base base = Base::Hexadecimal;
        };

        template<typename T>
        BinaryValue<T> bin(T value)
        {
                return {value};
        }

        template<typename T>
        OctalValue<T> oct(T value)
        {
                return {value};
        }

        template<typename T>
        HexadecimalValue<T> hex(T value)
        {
                return {value};
        }

        template<SignedInteger T>
        String<> intToString(T value, Base base)
        {
                String str;
                String digits = "0123456789abcdef";

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
                        T remaining = value % toUnderlying(base);
                        str.appendChar(digits[remaining]);
                        value = value / toUnderlying(base);
                }

                if (is_negative)
                        str.appendChar('-');

                return str.reverse();
        }

        template<UnsignedInteger T>
        String<> uintToString(T value, Base base)
        {
                String str;
                String digits = "0123456789abcdef";

                if (value == 0) {
                        str.appendChar('0');
                        return str;
                }

                while (value != 0) {
                        T remaining = value % toUnderlying(base);
                        str.appendChar(digits[remaining]);
                        value = value / toUnderlying(base);
                }

                return str.reverse();
        }

        template<Integer T>
        T stringToInt(const String<> &str, Base base)
        {
                T res = 0;

                for (T i = 0; i < str.length() && str[i] != '\0' && str[i] != ' '; ++i) {
                        T digit = 0;

                        if (str[i] >= '0' && str[i] <= '9')
                                digit = str[i] - '0';
                        else if (str[i] >= 'a' && str[i] <= 'z')
                                digit = str[i] - 'a' + 10;
                        else
                                digit = str[i] - 'A' + 10;

                        res = res * toUnderlying(base) + static_cast<T>(digit);
                }

                return res;
        }

        template<SignedInteger T, usize N>
        String<N> intToString(T value, Base base)
        {
                String<N> str;
                String<N> digits = "0123456789abcdef";

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
                        T remaining = value % toUnderlying(base);
                        str.appendChar(digits[remaining]);
                        value = value / toUnderlying(base);
                }

                if (is_negative)
                        str.appendChar('-');

                return str.reverse();
        }

        template<UnsignedInteger T, usize N>
        String<N> uintToString(T value, Base base)
        {
                String<N> str;
                String<N> digits = "0123456789abcdef";

                if (value == 0) {
                        str.appendChar('0');
                        return str;
                }

                while (value != 0) {
                        T remaining = value % toUnderlying(base);
                        str.appendChar(digits[remaining]);
                        value = value / toUnderlying(base);
                }

                return str.reverse();
        }

        template<Integer T, usize N>
        T stringToInt(const String<N> &str, Base base)
        {
                T res = 0;

                for (T i = 0; i < str.length() && str[i] != '\0' && str[i] != ' '; ++i) {
                        T digit = 0;

                        if (str[i] >= '0' && str[i] <= '9')
                                digit = str[i] - '0';
                        else if (str[i] >= 'a' && str[i] <= 'z')
                                digit = str[i] - 'a' + 10;
                        else
                                digit = str[i] - 'A' + 10;

                        res = res * toUnderlying(base) + static_cast<T>(digit);
                }

                return res;
        }

        template<UnsignedInteger T>
        T charToInt(char ch)
        {
                return ch - '0';
        }
} // namespace Kiwi::Lib