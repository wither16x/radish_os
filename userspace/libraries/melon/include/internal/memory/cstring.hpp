#pragma once

#include "../../typing.hpp"

namespace Melon::Memory
{
        /// @brief A null-terminated string.
        ///
        /// This kind of string is not intended to be extended or shortened.
        /// Its size should be always the same.
        class CString
        {
                char *data;

        public:
                /// @brief Constructs the object from a given null-terminated string.
                ///
                /// The internal buffer is allocated on the heap.
                /// @param s null-terminated string
                CString(const char *s);

                /// @brief Free the internal buffer since it is allocated on the heap.
                ~CString();

                /// @brief Copies somes bytes from the internal buffer to another string's
                /// internal buffer.
                /// @param other destination string
                /// @param chars character count
                /// @return destination string
                CString &copy(this const CString &self, CString &dest, Typing::USize chars);

                /// @brief Checks if this string contains a given character.
                /// @param ch character to find
                /// @return boolean
                bool has(this const CString &self, char ch);

                /// @brief Computes the length of this string.
                ///
                /// The null-terminating character is not treated as a part of the string.
                /// @return string length
                Typing::USize length(this const CString &self);

                /// @brief Gets a pointer to the base of the internal buffer.
                const char *begin(this const CString &self);

                /// @brief Gets a pointer to the end of the internal buffer.
                const char *end(this const CString &self);

                /// @brief Gets the internal buffer.
                /// @return internal buffer (constant)
                const char *get(this const CString &self);

                /// @brief Checks if a string is the same than this one.
                /// @param other string to compare
                /// @return boolean
                bool operator ==(this const CString &self, const CString &other);

                /// @brief Checks if a string is different to this one.
                /// @param other string to compare
                /// @return boolean
                bool operator !=(this const CString &self, const CString &other);

                /// @brief Checks if this string is smaller than another.
                /// @param other string to compare
                /// @return boolean
                bool operator <(this const CString &self, const CString &other);

                /// @brief Checks if this string is bigger than another.
                /// @param other string to compare
                /// @return boolean
                bool operator >(this const CString &self, const CString &other);

                /// @brief Checks if this string is smaller than or equals another.
                /// @param other string to compare
                /// @return boolean
                bool operator <=(this const CString &self, const CString &other);

                /// @brief Checks if this string is bigger than or equals another.
                /// @param other string to compare
                /// @return boolean
                bool operator >=(this const CString &self, const CString &other);

                /// @brief Gets a character by its index in the string.
                /// @param index character position
                /// @return character at given position
                char &operator [](this CString &self, Typing::USize index);
        };
} // namespace Melon::Memory