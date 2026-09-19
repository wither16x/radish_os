#pragma once

#include "lib/conversion.hpp"
#include <lib/string.hpp>
#include <lib/typing.hpp>

namespace Kiwi::Lib
{
        template<typename T>
        concept CharPtr = is_same_type<remove_const_volatile_ref_t<T>, const char *>
                or is_same_type<remove_const_volatile_ref_t<T>, char *>
        ;

        template<typename T>
        struct IsSpecialValue : false_t
        {};

        template<typename T>
        struct IsSpecialValue<BinaryValue<T>> : true_t
        {};

        template<typename T>
        struct IsSpecialValue<OctalValue<T>> : true_t
        {};

        template<typename T>
        struct IsSpecialValue<HexadecimalValue<T>> : true_t
        {};

        template<typename T>
        inline constexpr bool is_special_value = IsSpecialValue<remove_const_volatile_ref_t<T>>::value;

        template<typename T>
        concept FormattableCommon = is_same_type<remove_const_volatile_ref_t<T>, char>
                or CharPtr<T>
                or SignedInteger<remove_const_volatile_ref_t<T>>
                or UnsignedInteger<remove_const_volatile_ref_t<T>>
                or is_special_value<T>
        ;

        template<typename T, usize N>
        concept FormattableStatic = is_same_type<remove_const_volatile_ref_t<T>, String<N>>
                or FormattableCommon<T>
        ;

        template<typename T>
        concept FormattableDynamic = is_same_type<remove_const_volatile_ref_t<T>, String<>>
                or FormattableCommon<T>
        ;

        constexpr void formatArgs(String<> &out, const String<> &fmt, usize i)
        {
                for (; i < fmt.length(); i++)
                        out.appendChar(fmt[i]);
        }

        template<usize N>
        constexpr void formatArgs(String<N> &out, const String<N> &fmt, usize i)
        {
                for (; i < fmt.length(); i++)
                        out.appendChar(fmt[i]);
        }

        template<FormattableDynamic T, typename... ARGS>
        constexpr void formatArgs(String<> &out, const String<> &fmt, usize i, T &&arg, ARGS &&...args)
        {
                for (; i < fmt.length(); i++) {
                        if (fmt[i] != '{') {
                                out.appendChar(fmt[i]);
                                continue;
                        }

                        ++i;
                        if (i >= fmt.length())
                                return;

                        if (fmt[i] == '}') {
                                if constexpr (is_same_type<remove_const_volatile_ref_t<T>, String<>>)
                                        out += arg;
                                else if constexpr (is_same_type<remove_const_volatile_ref_t<T>, char>)
                                        out.appendChar(arg);
                                else if constexpr (CharPtr<T>)
                                        out += arg;
                                else if constexpr (SignedInteger<remove_const_volatile_ref_t<T>>)
                                        out += intToString<remove_const_volatile_ref_t<T>>(arg, Base::Decimal);
                                else if constexpr (UnsignedInteger<remove_const_volatile_ref_t<T>>)
                                        out += uintToString<remove_const_volatile_ref_t<T>>(arg, Base::Decimal);
                                else if (is_special_value<T>)
                                        out += uintToString<remove_const_volatile_ref_t<decltype(arg.value)>>(arg.value, remove_const_volatile_ref_t<T>::base);

                                ++i;
                                if constexpr (sizeof...(ARGS) > 0) {
                                        formatArgs(out, fmt, i, static_cast<ARGS &&>(args)...);
                                } else {
                                        for (; i < fmt.length(); i++)
                                                out.appendChar(fmt[i]);
                                }
                                return;
                        }
                }
        }

        template<usize N, FormattableStatic<N> T, typename... ARGS>
        constexpr void formatArgs(String<N> &out, const String<N> &fmt, usize i, T &&arg, ARGS &&...args)
        {
                for (; i < fmt.length(); i++) {
                        if (fmt[i] != '{') {
                                out.appendChar(fmt[i]);
                                continue;
                        }

                        ++i;
                        if (i >= fmt.length())
                                return;

                        if (fmt[i] == '}') {
                                if constexpr (is_same_type<remove_const_volatile_ref_t<T>, String<N>>)
                                        out += arg;
                                else if constexpr (is_same_type<remove_const_volatile_ref_t<T>, char>)
                                        out.appendChar(arg);
                                else if constexpr (CharPtr<T>)
                                        out += arg;
                                else if constexpr (SignedInteger<remove_const_volatile_ref_t<T>>)
                                        out += intToString<remove_const_volatile_ref_t<T>, N>(arg, Base::Decimal);
                                else if constexpr (UnsignedInteger<remove_const_volatile_ref_t<T>>)
                                        out += uintToString<remove_const_volatile_ref_t<T>, N>(arg, Base::Decimal);
                                else if (is_special_value<T>)
                                        out += uintToString<remove_const_volatile_ref_t<decltype(arg.value)>, N>(arg.value, remove_const_volatile_ref_t<T>::base);

                                ++i;
                                if constexpr (sizeof...(ARGS) > 0) {
                                        formatArgs(out, fmt, i, static_cast<ARGS &&>(args)...);
                                } else {
                                        for (; i < fmt.length(); i++)
                                                out.appendChar(fmt[i]);
                                }
                                return;
                        }
                }
        }

        template<typename... ARGS>
        String<> formatString(const String<> &str, ARGS &&...args)
        {
                String<> new_str;
                usize i = 0;

                formatArgs(new_str, str, i, static_cast<ARGS &&>(args)...);
        
                return new_str;
        }

        template<usize N, typename... ARGS>
        String<N> formatString(const String<N> &str, ARGS &&...args)
        {
                String<N> new_str;
                usize i = 0;

                formatArgs<N>(new_str, str, i, static_cast<ARGS &&>(args)...);
        
                return new_str;
        }
} // namespace Kiwi::Lib