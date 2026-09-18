#pragma once

#include <lib/typing.hpp>
#include <lib/vector.hpp>
#include <lib/memory.hpp>
#include <lib/array.hpp>
#include <panic.hpp>

namespace Kiwi::Lib
{
        template<usize N = max_of<usize>>
        class _String
        {
                Array<char, N> arr;

        public:
                _String()
                {
                        this->arr.clear();
                }

                _String(const char *base)
                {
                        for (usize i = 0; i < strlen(base); i++)
                                this->arr[i] = base[i];
                }

                _String(const _String<N> &other)
                        : arr(other.arr)
                {}

                _String(_String<N> &&other)
                        : arr(move(other.arr))
                {}

                char *begin(this _String<N> &self)
                {
                        return self.arr.begin();
                }

                char *end(this _String<N> &self)
                {
                        return self.arr.begin() + strlen(self.arr.raw());
                }

                const char *begin(this const _String<N> &self)
                {
                        return self.arr.begin();
                }

                const char *end(this const _String<N> &self)
                {
                        return self.arr + strlen(self.arr.raw());
                }

                const char *raw(this const _String<N> &self)
                {
                        return self.arr.raw();
                }

                usize length(this const _String<N> &self)
                {
                        return strlen(self.arr.raw());
                }

                _String<N> subString(this const _String<N> &self, usize start)
                {
                        if (start >= N)
                                panic("index out of range");

                        _String<N> new_str;

                        for (usize i = 0; i < N - start; i++)
                                new_str.arr[i] = self.arr[start + i];

                        return new_str;
                }

                _String<N> operator +(this const _String<N> &self, const _String<N> &other)
                {
                        _String<N> new_str = self;
                        usize length = new_str.length();

                        for (usize i = 0; i < other.length(); i++)
                                new_str.arr[length + i] = other.arr[i];

                        return new_str;
                }

                _String<N> &operator =(this _String<N> &self, const _String<N> &other) = default;
                _String<N> &operator =(this _String<N> &self, _String<N> &&other) = default;

                char &operator [](this _String<N> &self, usize index)
                {
                        if (index >= N)
                                panic("index out of range");

                        return self.arr[index];
                }

                const char &operator [](this const _String<N> &self, usize index)
                {
                        if (index >= N)
                                panic("index out of range");

                        return self.arr[index];     
                }

                bool operator ==(this const _String<N> &self, const _String<N> &other)
                {
                        return strcmp(self.arr.raw(), other.arr.raw()) == 0;
                }

                bool operator !=(this const _String<N> &self, const _String<N> &other)
                {
                        return strcmp(self.arr.raw(), other.arr.raw()) != 0;
                }

                bool operator <(this const _String<N> &self, const _String<N> &other)
                {
                        return strcmp(self.arr.raw(), other.arr.raw()) < 0;
                }

                bool operator >(this const _String<N> &self, const _String<N> &other)
                {
                        return strcmp(self.arr.raw(), other.arr.raw()) > 0;
                }

                bool operator <=(this const _String<N> &self, const _String<N> &other)
                {
                        return strcmp(self.arr.raw(), other.arr.raw()) <= 0;
                }

                bool operator >=(this const _String<N> &self, const _String<N> &other)
                {
                        return strcmp(self.arr.raw(), other.arr.raw()) >= 0;
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

                _String(const _String &other)
                        : vec(other.vec)
                {}

                _String(_String &&other) noexcept
                        : vec(move(other.vec))
                {}

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

                bool isEmpty(this const _String &self)
                {
                        return self.length() == 0;
                }

                char *begin(this _String &self)
                {
                        return self.vec.begin();
                }

                char *end(this _String &self)
                {
                        return self.vec.end() - 1;
                }

                const char *begin(this const _String &self)
                {
                        return self.vec.begin();
                }

                const char *end(this const _String &self)
                {
                        return self.vec.end() - 1;
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
                        for (auto &c : other)
                                new_str.vec.pushBack(c);
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
} // namespace Kiwi::Lib