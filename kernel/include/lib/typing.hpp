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
} // namespace Kiwi::Lib