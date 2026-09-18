#pragma once

#include <lib/string.hpp>
#include <lib/typing.hpp>

namespace Kiwi::Lib
{
        template<typename T, usize N>
        concept FormattableStatic = is_same_type<remove_const_volatile_ref_t<T>, String<N>>
        ;

        template<typename T>
        concept FormattableDynamic = is_same_type<remove_const_volatile_ref_t<T>, String<>>
        ;

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
                                if constexpr (
                                        is_same_type<remove_const_volatile_ref_t<T>, String<>>
                                ) {
                                        out += arg;
                                }

                                ++i;
                                if constexpr (sizeof...(ARGS) > 0) {
                                        formatArgs(fmt, static_cast<ARGS &&>(args)...);
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
                                if constexpr (
                                        is_same_type<remove_const_volatile_ref_t<T>, String<N>>
                                ) {
                                        out += arg;
                                }

                                ++i;
                                if constexpr (sizeof...(ARGS) > 0) {
                                        formatArgs(fmt, static_cast<ARGS &&>(args)...);
                                } else {
                                        for (; i < fmt.length(); i++)
                                                out.appendChar(fmt[i]);
                                }
                                return;
                        }
                }
        }

        constexpr void formatArgs(String<> &out, const String<> &fmt, usize i)
        {
                for (; i < fmt.length(); i++)
                        out.appendChar(fmt[i]);
        }

        template<usize N>
        constexpr void formatArgs(String<N> &out, const String<> &fmt, usize i)
        {
                for (; i < fmt.length(); i++)
                        out.appendChar(fmt[i]);
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