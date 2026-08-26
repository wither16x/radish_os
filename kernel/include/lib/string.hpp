#pragma once

#include <lib/typing.hpp>
#include <lib/vector.hpp>

namespace Kiwi::Lib
{
        /// Use this class if a heap allocator is available and if you need
        /// dynamic strings.
        /// Otherwise, please keep it simple and use char pointers.
        class String
        {
                Vector<char> data;

        public:
                String();
                String(const char *buf);
                String(const String &other)     = default;
                String(String &&other)          = default;

                ~String()                       = default;

                /// Return a null-terminated string from this string.
                const char *raw(this const String &self);
                /// Return the length of the string.
                usize length(this const String &self);
                /// Split this string from `start`.
                String sub(this const String &self, usize start);

                String operator +(this const String &self, const String &other);
                String operator +(this const String &self, char ch);
                String &operator =(this String &self, const String &other)      = default;
                String &operator =(this String &self, String &&other)           = default;
                String &operator +=(this String &self, const String &other);
                String &operator +=(this String &self, char ch);
                char operator [](this String &self, usize index);
                char operator [](this const String &self, usize index);
                bool operator ==(this const String &self, const String &other);
        };
} // namespace Kiwi::Lib