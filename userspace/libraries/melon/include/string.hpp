#pragma once

#include "vector.hpp"
#include "typing.hpp"

/// @brief Contains dynamic strings related features.
namespace Melon::String
{
        /// @brief A dynamic string.
        class String
        {
                Vector::Vector<char> chars;

        public:
                String();

                /// @brief Constructs the string from a raw char pointer.
                /// @param s raw char pointer
                String(const char *s);

                /// @brief Copy constructor.
                /// @param other string to copy
                String(const String &other);

                /// @brief Move constructor.
                /// @param other string to move
                String(String &&other);

                /// @brief Returns a substring from this string.
                /// @param chars number of characters to substract
                /// @return substring
                String substract(this String &self, Typing::USize chars);

                /// @brief Gets a raw buffer which contains the string characters.
                /// @return constant char pointer
                const char *raw(this const String &self);

                /// @brief Gets the length of the string.
                /// @return internal vector length
                Typing::USize length(this const String &self);

                /// @brief Gets the base of the string vector.
                /// @return pointer to base
                const char *begin(this const String &self);

                /// @brief Gets the end of the string vector.
                /// @return pointer to end
                const char *end(this const String &self);

                /// @brief Appends a character to the string.
                /// @param ch character to append
                void appendChar(this String &self, char ch);

                /// @brief Reverse the string.
                /// @return self
                String reverse(this String &self);

                /// @brief Removes all occurences of ch at the left of the string.
                /// @param ch character to remove, =' ' by default
                /// @return self  
                String ltrim(this String &self, char ch = ' ');

                /// @brief Removes all occurences of ch at the right of the string.
                /// @param ch character to remove, =' ' by default
                /// @return self
                String rtrim(this String &self, char ch = ' ');

                /// @brief Removes all occurences of ch at the left and the right of the string.
                /// @param ch character to remove, =' ' by default
                /// @return self
                String trim(this String &self, char ch = ' ');

                /// @brief Gets a character from its index.
                /// @param index position of the character
                /// @return character
                char &operator [](this String &self, Typing::USize index);

                /// @brief Gets a character from its index.
                /// @param index position of the character
                /// @return character
                const char &operator [](this const String &self, Typing::USize index);

                /// @brief Checks if both strings are the same.
                /// @param other string to compare
                /// @return boolean
                bool operator ==(this const String &self, const String &other);

                /// @brief Checks if both strings are different.
                /// @param other string to compare
                /// @return boolean
                bool operator !=(this const String &self, const String &other);

                /// @brief Checks if this string is smaller than another.
                /// @param other string to compare
                /// @return boolean
                bool operator <(this const String &self, const String &other);

                /// @brief Checks if this string is bigger than another.
                /// @param other string to compare
                /// @return boolean
                bool operator >(this const String &self, const String &other);

                /// @brief Checks if this string is smaller than another or if both have the same size.
                /// @param other string to compare
                /// @return boolean
                bool operator <=(this const String &self, const String &other);

                /// @brief Checks if this string is bigger than another or if both have the same size.
                /// @param other string to compare
                /// @return boolean
                bool operator >=(this const String &self, const String &other);

                /// @brief Creates a new string by concatenating this string and another.
                /// @param other string to concatenate with
                /// @return new string
                String operator +(this const String &self, const String &other);

                /// @brief Copy assignment operator.
                /// @param other string to copy
                /// @return self
                String &operator =(this String &self, const String &other);

                /// @brief Move assignment operator.
                /// @param other string to move
                /// @return self
                String &operator =(this String &self, String &&other);

                /// @brief Concatenate two strings.
                /// @param other string to add
                /// @return self
                String &operator +=(this String &self, const String &other);
        };
} // namespace Melon::String