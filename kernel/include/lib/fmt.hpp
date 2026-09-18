#pragma once

#include <lib/string.hpp>
#include <lib/typing.hpp>

namespace Kiwi::Lib
{
        template<typename T>
        concept Formattable = is_same_type<remove_const_volatile_ref_t<T>, String<>>;

        template<Formattable T, typename... ARGS>
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
                // TODO
        }
} // namespace Kiwi::Lib