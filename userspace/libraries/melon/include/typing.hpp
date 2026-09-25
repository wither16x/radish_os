
#pragma once

#include <stdint.h>
#include <sys/types.h>
#include <stddef.h>
#include <ctype.h>

/// @brief Contains fixed-size types.
///
/// The types defined in this namespace ensure the size of the data.
namespace Melon::Typing
{
        /// @brief Signed 8-bit integer
        using Int8              = int8_t;
        /// @brief Signed 16-bit integer
        using Int16             = int16_t;
        /// @brief Signed 32-bit integer
        using Int32             = int32_t;
        /// @brief Signed 64-bit integer
        using Int64             = int64_t;
        /// @brief Biggest signed pointer type
        using IntPointer        = intptr_t;
        /// @brief Biggest signed type
        using ISize             = ssize_t;

        /// @brief Unsigned 8-bit integer
        using Uint8             = uint8_t;
        /// @brief Unsigned 16-bit integer
        using Uint16            = uint16_t;
        /// @brief Unsigned 32-bit integer
        using Uint32            = uint32_t;
        /// @brief Unsigned 64-bit integer
        using Uint64            = uint64_t;
        /// @brief Biggest unsigned pointer type
        using UPointer          = uintptr_t;
        /// @brief Biggest unsigned type
        using USize             = size_t;

        inline constexpr bool isSpace(char ch)
        {
                return isspace(ch);
        }

        inline constexpr bool isDigit(char ch)
        {
                return isdigit(ch);
        }

        template<typename Sig>
        class Function;

        template<typename R>
        class Function<R()>
        {
                R (*fn)();

        public:
                Function(R (*f)())
                        : fn(f)
                {}

                R operator()(this Function &self)
                {
                        return self.fn();
                }
        };

        template<typename R, typename... A>
        class Function<R(A...)>
        {
                R (*fn)(A...);
        
        public:
                Function(R (*f)(A...))
                        : fn(f)
                {}

                R operator()(this Function &self, A... args)
                {
                        return self.fn(args...);
                }
        };

        template<typename T>
        struct RemoveReference
        {
                using type = T;
        };

        template<typename T>
        struct RemoveReference<T &>
        {
                using type = T;
        };

        template<typename T>
        struct RemoveReference<T &&>
        {
                using type = T;
        };

        template<typename T>
        struct RemoveConstVolatile
        {
                using type = T;
        };

        template<typename T>
        struct RemoveConstVolatile<const T>
        {
                using type = T;
        };

        template<typename T>
        struct RemoveConstVolatile<volatile T>
        {
                using type = T;
        };

        template<typename T>
        struct RemoveConstVolatile<const volatile T>
        {
                using type = T;
        };

        template<typename T>
        struct RemoveConstVolatileRef
        {
                using type = typename RemoveConstVolatile<typename RemoveReference<T>::type>::type;
        };

        template<typename T>
        struct TypeIdentity
        {
                using type = T;
        };

        template<typename T>
        using remove_reference_t = typename RemoveReference<T>::type;

        template<typename T>
        using remove_const_volatile_ref_t = typename RemoveConstVolatileRef<T>::type;

        template<typename T>
        using type_identity_t = typename TypeIdentity<T>::type;
} // namespace Melon::Typing