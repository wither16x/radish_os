#pragma once

#include <lib/typing.hpp>
#include <lib/forward.hpp>
#include <lib/alloc.hpp>
#include <lib/result.hpp>
#include <panic.hpp>

namespace Kiwi::Lib
{
        template<typename T>
        class Buffer
        {
                T *data;
                usize __size;

                static T *allocate(usize size)
                {
                        if (size)
                                return static_cast<T *>(::operator new(size * sizeof(T)));
                        return nullptr;
                }

        public:
                Buffer()
                        : data(nullptr), __size(0)
                {}

                Buffer(usize size)
                        : data(allocate(size)), __size(size)
                {}

                Buffer(const T *data, usize size)
                        : __size(size)
                {
                        this->data = new T[size];
                        memcpy(this->data, data, size);
                }
                
                Buffer(const Buffer<T> &other)
                        : data(allocate(other.__size)), __size(other.__size)
                {}

                Buffer(Buffer<T> &&other)
                        : data(other.data), __size(other.__size)
                {
                        other.__size = 0;
                        other.data = nullptr;
                }

                ~Buffer()
                {
                        ::operator delete(this->data);
                }

                template<typename... ARGS>
                bool construct(this Buffer<T> &self, usize index, ARGS &&...args)
                {
                        if (index >= self.__size)
                                return false;

                        new (&self.data[index]) T(forward<ARGS>(args)...);
                        return true;
                }

                bool destroy(this Buffer<T> &self, usize index)
                {
                        if (index >= self.__size)
                                return false;
                        
                        self.data[index].~T();
                        return true;
                }

                Result<Buffer<T> &, bool> copy(this const Buffer<T> &self, Buffer<T> &dest, usize size)
                {
                        if (size > self.__size)
                                return Error(false);
                        if (size > dest.__size)
                                return Error(false);

                        memcpy(dest.data, self.data, size);

                        return dest;
                }

                Result<Buffer<T> &, bool>set(this Buffer<T> &self, T c, usize size)
                {
                        if (size > self.__size)
                                return Error(false);

                        memset(self.data, c, size);

                        return self;
                }

                Result<Buffer<T> &, bool>move(this const Buffer<T> &self, Buffer<T> &dest, usize size)
                {
                        if (size > self.__size)
                                return Error(false);
                        if (size > dest.__size)
                                return Error(false);

                        memmove(dest.data, self.data, size);

                        return dest;
                }

                Result<int, bool> compare(this const Buffer<T> &self, const Buffer<T> &other, usize size)
                {
                        if (size > self.__size)
                                return Error(false);
                        if (size > other.__size)
                                return Error(false);

                        int result = memcmp(self.data, other.data, size);

                        return result;
                }

                const T *get(this const Buffer<T> &self)
                {
                        return self.data;
                }

                usize size(this const Buffer<T> &self)
                {
                        return self.__size;
                }

                const T *begin(this const Buffer<T> &self)
                {
                        return self.data;
                }

                const T *end(this const Buffer<T> &self)
                {
                        return self.data + self.__size;
                }

                T *begin(this Buffer<T> &self)
                {
                        return self.data;
                }

                T *end(this Buffer<T> &self)
                {
                        return self.data + self.__size;
                }

                void resize(this Buffer<T> &self, usize new_size)
                {
                        T *new_data = new T[new_size];

                        for (usize i = 0; i < self.__size; ++i)
                                new_data[i] = self.data[i];

                        delete[] self.data;
                        self.data = new_data;
                        self.__size = new_size;
                }

                bool operator ==(this const Buffer<T> &self, const Buffer<T> &other)
                {
                        return self.__size == other.__size and self.compare(other, self.__size) == 0;
                }

                bool operator !=(this const Buffer<T> &self, const Buffer<T> &other)
                {
                        return not (self == other);
                }

                bool operator <(this const Buffer<T> &self, const Buffer<T> &other)
                {
                        usize size = self.__size < other.__size ? self.__size : other.__size;
                        int result = self.compare(other, size);
                        if (result != 0)
                                return result < 0;
                        return self.__size < other.__size;
                }

                bool operator >(this const Buffer<T> &self, const Buffer<T> &other)
                {
                        usize size = self.__size < other.__size ? self.__size : other.__size;
                        int result = self.compare(other, size);
                        if (result != 0)
                                return result > 0;
                        return self.__size > other.__size;
                }

                bool operator <=(this const Buffer<T> &self, const Buffer<T> &other)
                {
                        return self < other or self == other;
                }

                bool operator >=(this const Buffer<T> &self, const Buffer<T> &other)
                {
                        return self > other or self == other;
                }

                T &operator [](this Buffer<T> &self, usize index)
                {
                        if (index >= self.__size)
                                panic("index out of range");

                        return self.data[index];
                }

                const T &operator [](this const Buffer<T> &self, usize index)
                {
                        if (index >= self.__size)
                                panic("index out of range");

                        return self.data[index];
                }

                Buffer<T> &operator =(this Buffer<T> &self, const Buffer<T> &other)
                {
                        if (&self != &other) {
                                ::operator delete(self.data);
                                self.data = allocate(other.__size);
                                self.__size = other.__size;
                        }

                        return self;
                }

                Buffer<T> &operator =(this Buffer<T> &self, Buffer<T> &&other)
                {
                        if (&self != &other) {
                                ::operator delete(self.data);
                                self.data = other.data;
                                self.__size = other.__size;

                                other.data = nullptr;
                                other.__size = 0;
                        }

                        return self;
                }
        };
} // namespace Kiwi::Lib