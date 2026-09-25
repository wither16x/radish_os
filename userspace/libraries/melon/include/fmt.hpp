#pragma once

#include "string.hpp"
#include "conversion.hpp"
#include "typing.hpp"

#include <concepts>
#include <type_traits>

/// @brief Contains functions for string formatting.
namespace Melon::Fmt
{
        /// @brief Default behaviour for every type T.
        ///
        /// By default, every type T is not a special value, so we inherit from
        /// std::false_type.
        template<typename T>
        struct IsSpecialValue : std::false_type
        {};

        /// @brief Partial template specialization for Conversion::BinaryValue<T>.
        template<typename T>
        struct IsSpecialValue<Conversion::BinaryValue<T>> : std::true_type
        {};

        /// @brief Partial template specialization for Conversion::OctalValue<T>.
        template<typename T>
        struct IsSpecialValue<Conversion::OctalValue<T>> : std::true_type
        {};

        /// @brief Partial template specialization for Conversion::HexadecimalValue<T>.
        template<typename T>
        struct IsSpecialValue<Conversion::HexadecimalValue<T>> : std::true_type
        {};

        /// @brief Alias to avoid typing IsSpecialValue<T>::value.
        template<typename T>
        inline constexpr bool IsSpecialValueV = IsSpecialValue<std::remove_cvref_t<T>>::value;

        /// @brief Makes sure that T is supported by formatArgs().
        template<typename T>
        concept Formattable =
                std::same_as<std::remove_cvref_t<T>, char>
                or std::same_as<std::remove_cvref_t<T>, String::String>
                or std::same_as<std::remove_cvref_t<T>, int>
                or std::same_as<std::remove_cvref_t<T>, long>
                or std::same_as<std::remove_cvref_t<T>, long long>
                or std::same_as<std::remove_cvref_t<T>, unsigned int>
                or std::same_as<std::remove_cvref_t<T>, unsigned long>
                or std::same_as<std::remove_cvref_t<T>, unsigned long long>
                or std::same_as<std::remove_cvref_t<T>, Typing::Int8>
                or std::same_as<std::remove_cvref_t<T>, Typing::Int16>
                or std::same_as<std::remove_cvref_t<T>, Typing::Int32>
                or std::same_as<std::remove_cvref_t<T>, Typing::Int64>
                or std::same_as<std::remove_cvref_t<T>, Typing::Uint8>
                or std::same_as<std::remove_cvref_t<T>, Typing::Uint16>
                or std::same_as<std::remove_cvref_t<T>, Typing::Uint32>
                or std::same_as<std::remove_cvref_t<T>, Typing::Uint64>
                or IsSpecialValueV<T>
                or std::same_as<std::remove_cvref_t<T>, std::nullptr_t>
                or std::same_as<std::remove_cvref_t<T>, char *>
        ;

        /// @brief Format a string.
        /// @param out output string
        /// @param fmt string to format
        /// @param idx index in the string
        /// @param arg argument
        /// @param args arguments
        template<Formattable T, typename... ARGS>
        constexpr void formatArgs(String::String &out, const String::String &fmt, Typing::USize &idx, T &&arg, ARGS &&...args)
        {
                for (; idx < fmt.length(); ++idx) {
                        if (fmt[idx] != '{') {
                                out.appendChar(fmt[idx]);
                                continue;
                        }

                        ++idx;
                        if (idx >= fmt.length())
                                return;

                        if (fmt[idx] == '}') {
                                // "{}" found, so we expect a parameter
                                if constexpr (std::is_same<std::remove_cvref_t<T>, char>::value) {
                                        out.appendChar(arg);
                                } else if constexpr (std::is_same<std::remove_cvref_t<T>, String::String>::value)
                                        out += arg;
                                else if constexpr (
                                        std::is_same<std::remove_cvref_t<T>, int>::value
                                        or std::is_same<std::remove_cvref_t<T>, long>::value
                                        or std::is_same<std::remove_cvref_t<T>, long long>::value
                                ) {
                                        out += Conversion::intToString<std::remove_cvref_t<T>>(arg, Conversion::Base::Decimal);
                                } else if constexpr (
                                        std::is_same<std::remove_cvref_t<T>, unsigned int>::value
                                        or std::is_same<std::remove_cvref_t<T>, unsigned long>::value
                                        or std::is_same<std::remove_cvref_t<T>, unsigned long long>::value
                                ) {
                                        out += Conversion::uintToString<std::remove_cvref_t<T>>(arg, Conversion::Base::Decimal);
                                } else if constexpr (IsSpecialValueV<T>) {
                                        using InnerType = std::remove_cvref_t<decltype(arg.value)>;
                                        out += Conversion::uintToString<InnerType>(arg.value, std::remove_cvref_t<T>::base);
                                } else if constexpr (
                                        std::is_same<std::remove_cvref_t<T>, std::nullptr_t>::value
                                ) {
                                        out += "(nullptr)";
                                } else if constexpr (
                                        std::is_same<std::remove_cvref_t<T>, char *>::value
                                ) {
                                        out += arg;
                                }

                                ++idx;
                                if constexpr (sizeof...(ARGS) > 0) {
                                        formatArgs(out, fmt, idx, static_cast<ARGS &&>(args)...);
                                } else {
                                        for (; idx < fmt.length(); ++idx)
                                                out.appendChar(fmt[idx]);
                                }
                                return;
                        }
                }
        }

        /// @brief Overload to allow formatString() to compile without format arguments.
        constexpr void formatArgs(String::String &out, const String::String &fmt, Typing::USize &idx)
        {
                for (; idx < fmt.length(); ++idx)
                        out.appendChar(fmt[idx]);
        }

        /// @brief Format a string.
        ///
        /// Expected format: "This is a parameter: {} and here is another one: {}", a, b
        /// @param str string to format
        /// @param args values
        template<typename... ARGS>
        String::String formatString(const String::String &str, ARGS &&...args)
        {
                String::String new_str;
                Typing::USize idx = 0;

                formatArgs(new_str, str, idx, static_cast<ARGS &&>(args)...);

                return new_str;
        }
} // namespace Melon::Fmt