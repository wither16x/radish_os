#pragma once

#include <lib/fmt.hpp>
#include <lib/string.hpp>

namespace Kiwi::Lib
{
        void putchar(int ch);
        void printString(const String<> &s);

        template<usize N>
        void printString(const String<N> &s)
        {
                for (auto &ch : s)
                        putchar(ch);
        }

        template<typename... ARGS>
        void print(const String<> &str, ARGS &&...args)
        {
                String formatted;
                usize idx = 0;
                formatArgs(formatted, str, idx, static_cast<ARGS &&>(args)...);
                printString(formatted);
        }

        template<typename... ARGS>
        void println(const String<> &str, ARGS &&...args)
        {
                print(str, static_cast<ARGS &&>(args)...);
                print("\r\n");
        }

        template<usize N, typename... ARGS>
        void print(const String<N> &str, ARGS &&...args)
        {
                String<N> formatted;
                usize idx = 0;
                formatArgs<N>(formatted, str, idx, static_cast<ARGS &&>(args)...);
                printString<N>(formatted);
        }

        template<usize N, typename... ARGS>
        void println(const String<N> &str, ARGS &&...args)
        {
                print<N>(str, static_cast<ARGS &&>(args)...);
                print<N>("\r\n");
        }
} // namespace Kiwi::Lib