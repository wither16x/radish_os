#pragma once

#include <lib/typing.hpp>
#include <lib/vector.hpp>
#include <lib/memory.hpp>
#include <panic.hpp>

namespace Kiwi::Lib
{
        template<usize N = max_of<usize>>
        class _String
        {
                char buf[N];

        public:
                _String() = default;

                _String(const char *base)
                {
                        memcpy(this->buf, base, strlen(base) + 1);
                }

                _String(const _String<N> &other) = default;
                _String(_String<N> &&other) = default;

                const char *raw(this const _String<N> &self)
                {
                        return self.buf;
                }

                usize length(this const _String<N> &self)
                {
                        return strlen(self.buf);
                }

                _String<N> subString(this const _String<N> &self, usize start)
                {
                        if (start >= N)
                                panic("index out of range");

                        _String<N> new_str;

                        for (usize i = start; i < N; i++)
                                new_str.buf[i] = self.buf[i];

                        return new_str;
                }

                _String<N * 2> operator +(this const _String<N> &self, const _String<N> &other)
                {
                        _String<N * 2> new_str;

                        for (usize i = 0; i < N; i++)
                                new_str.buf[i] = self.buf[i];
                        for (usize i = 0; i < N; i++)
                                new_str.buf[N + i] = other.buf[i];

                        return new_str;
                }

                _String<N> &operator =(this _String<N> &self, const _String<N> &other) = default;
                _String<N> &operator =(this _String<N> &self, _String<N> &&other) = default;

                char &operator [](this _String<N> &self, usize index)
                {
                        if (index >= N)
                                panic("index out of range");

                        return self.buf[index];
                }

                const char &operator [](this const _String<N> &self, usize index)
                {
                        if (index >= N)
                                panic("index out of range");

                        return self.buf[index];     
                }

                bool operator ==(this const _String<N> &self, const _String<N> &other)
                {
                        return strcmp(self.buf, other.buf) == 0;
                }
        };

        template<>
        class _String<max_of<usize>>
        {
                Vector<char> vec;

        public:
                _String()
                {
                        this->vec.pushBack('\0');
                }

                _String(const char *base)
                {
                        while (*base)
                                this->vec.pushBack(*base++);
                        this->vec.pushBack('\0');
                }

                _String(const _String &other) = default;
                _String(_String &&other) = default;

                const char *raw(this const _String &self)
                {
                        return self.vec.data();
                }

                usize length(this const _String &self)
                {
                        if (self.vec.isEmpty())
                                return 0;

                        return self.vec.length() - 1;
                }

                _String subString(this const _String &self, usize start)
                {
                        _String new_str;

                        new_str.vec.popBack();

                        usize len = self.vec.length();

                        if (start >= len)
                                panic("index out of range");

                        for (usize i = start; i < len; ++i)
                                new_str.vec.pushBack(self.vec[i]);

                        new_str.vec.pushBack('\0');

                        return new_str;
                }

                void appendChar(this _String &self, char ch)
                {
                        self.vec.popBack();
                        self.vec.pushBack(ch);
                        self.vec.pushBack('\0');
                }

                char &operator [](this _String &self, usize index)
                {
                        if (index >= self.length())
                                panic("index out of range");

                        return self.vec[index];
                }

                const char &operator [](this const _String &self, usize index)
                {
                        if (index >= self.length())
                                panic("index out of range");

                        return self.vec[index];
                }

                bool operator ==(this const _String &self, const _String &other)
                {
                        return strcmp(self.vec.data(), other.vec.data()) == 0;
                }

                _String operator +(this const _String &self, const _String &other)
                {
                        _String new_str = self;

                        new_str.vec.popBack();
                        for (usize i = 0; i < other.vec.length(); ++i)
                                new_str.vec.pushBack(other.vec[i]);
                        new_str.vec.pushBack('\0');

                        return new_str;
                }

                _String &operator =(this _String &self, const _String &other) = default;
                _String &operator =(this _String &self, _String &&other) = default;

                _String &operator +=(this _String &self, const _String &other)
                {
                        self = self + other;
                        return self;
                }
        };

        _String() -> _String<max_of<usize>>;

        template<usize N = max_of<usize>>
        using String = _String<N>;
        
        // class _String
        // {
        //         Vector<char> data;

        // public:
        //         _String();
        //         _String(const char *buf);
        //         _String(const _String &other)     = default;
        //         _String(_String &&other)          = default;

        //         ~_String()                       = default;

        //         /// Return a null-terminated _String from this _String.
        //         const char *raw(this const _String &self);
        //         /// Return the length of the _String.
        //         usize length(this const _String &self);
        //         /// Split this _String from `start`.
        //         _String sub(this const _String &self, usize start);

        //         _String operator +(this const _String &self, const _String &other);
        //         _String operator +(this const _String &self, char ch);
        //         _String &operator =(this _String &self, const _String &other)      = default;
        //         _String &operator =(this _String &self, _String &&other)           = default;
        //         _String &operator +=(this _String &self, const _String &other);
        //         _String &operator +=(this _String &self, char ch);
        //         char operator [](this _String &self, usize index);
        //         char operator [](this const _String &self, usize index);
        //         bool operator ==(this const _String &self, const _String &other);
        // };
} // namespace Kiwi::Lib