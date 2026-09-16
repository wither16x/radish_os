#pragma once

#include <stdint.h>
#include <stddef.h>

namespace Kiwi::Lib
{
        // signed types
        using i8                = int8_t;
        using i16               = int16_t;
        using i32               = int32_t;
        using i64               = int64_t;
        using isize             = i64;
        using iptr              = i64;

        // unsigned types
        using u8                = uint8_t;
        using u16               = uint16_t;
        using u32               = uint32_t;
        using u64               = uint64_t;
        using usize             = u64;
        using uptr              = u64;

        template<typename RET, typename... ARGS>
        using callable = RET (*)(ARGS...);

        struct false_t
        {
                static constexpr bool value = false;

                constexpr operator bool(this const false_t &self) noexcept
                {
                        return self.value;
                }
        };

        struct true_t
        {
                static constexpr bool value = true;

                constexpr operator bool(this const false_t &self) noexcept
                {
                        return self.value;
                }
        };

        template<typename T, typename U>
        struct IsSame : false_t
        {};

        template<typename T>
        struct IsSame<T, T> : true_t
        {};

        template<typename T, typename U>
        constexpr bool is_same_type = IsSame<T, U>::value;

        namespace Limits
        {
                constexpr i8 I8_MIN             = INT8_MIN;
                constexpr i16 I16_MIN           = INT16_MIN;
                constexpr i32 I32_MIN           = INT32_MIN;
                constexpr i64 I64_MIN           = INT64_MIN;

                constexpr i8 I8_MAX             = INT8_MAX;
                constexpr i16 I16_MAX           = INT16_MAX;
                constexpr i32 I32_MAX           = INT32_MAX;
                constexpr i64 I64_MAX           = INT64_MAX;

                constexpr u8 U8_MAX             = UINT8_MAX;
                constexpr u16 U16_MAX           = UINT16_MAX;
                constexpr u32 U32_MAX           = UINT32_MAX;
                constexpr u64 U64_MAX           = UINT64_MAX;
        } // namespace Limits

        template<typename T>
        concept SignedInteger = is_same_type<T, i8>
                or is_same_type<T, i16>
                or is_same_type<T, i32>
                or is_same_type<T, i64>;

        template<typename T>
        concept UnsignedInteger = is_same_type<T, u8>
                or is_same_type<T, u16>
                or is_same_type<T, u32>
                or is_same_type<T, u64>;

        template<typename T>
        concept Integer = SignedInteger<T> or UnsignedInteger<T>;

        template<Integer T>
        constexpr T getMinOf()
        {
                if constexpr (is_same_type<T, i8>)
                        return Limits::I8_MIN;
                else if constexpr (is_same_type<T, i16>)
                        return Limits::I16_MIN;
                else if constexpr (is_same_type<T, i32>)
                        return Limits::I32_MIN;
                else if constexpr (is_same_type<T, i64>)
                        return Limits::I64_MIN;
                else
                        return 0;
        }

        template<typename T>
        constexpr T min_of = getMinOf<T>();

        template<Integer T>
        constexpr T getMaxOf()
        {
                if constexpr (is_same_type<T, i8>)
                        return Limits::I8_MAX;
                else if constexpr (is_same_type<T, i16>)
                        return Limits::I16_MAX;
                else if constexpr (is_same_type<T, i32>)
                        return Limits::I32_MAX;
                else if constexpr (is_same_type<T, i64>)
                        return Limits::I64_MAX;
                if constexpr (is_same_type<T, u8>)
                        return Limits::U8_MAX;
                else if constexpr (is_same_type<T, u16>)
                        return Limits::U16_MAX;
                else if constexpr (is_same_type<T, u32>)
                        return Limits::U32_MAX;
                else if constexpr (is_same_type<T, u64>)
                        return Limits::U64_MAX;
                else
                        return 0;
        }

        template<typename T>
        constexpr T max_of = getMaxOf<T>();
} // namespace Kiwi::Lib