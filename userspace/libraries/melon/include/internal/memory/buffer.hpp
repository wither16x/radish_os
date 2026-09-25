#pragma once

#include "../../typing.hpp"
#include "../exceptions/buffer_overflow.hpp"
#include "../exceptions/out_of_range.hpp"

#include <string.h>
#include <utility>
#include <new>

namespace Melon::Memory 
{
        /// @brief Container for raw memory.
        ///
        /// This container stores allocated raw memory and the size of the allocated area.
        /// If `T` is not trivially constructible, its constructor will never be called
        /// internally.
        template<typename T>
        class Buffer
        {
                T *data;
                Typing::USize __size;

                static T *allocate(Typing::USize size)
                {
                        if (size)
                                return static_cast<T *>(::operator new(size * sizeof(T)));
                        return nullptr;
                }

        public:
                Buffer()
                {
                        this->data = nullptr;
                        this->__size = 0;
                }

                Buffer(Typing::USize size)
                        : data(allocate(size)), __size(size)
                {}

                /// @brief Constructs the buffer from existing raw memory.
                /// @param data memory to construct the buffer from
                /// @param size maximum size of the buffer
                Buffer(const T *data, Typing::USize size)
                        : __size(size)
                {
                        this->data = new T[size];
                        memcpy(this->data, data, size);
                }
                
                /// @brief Copy constructor.
                /// @param other buffer to copy data to
                Buffer(const Buffer<T> &other)
                        : data(allocate(other.__size)), __size(other.__size)
                {}

                /// @brief Move constructor.
                /// @param other buffer to move data and size to
                Buffer(Buffer<T> &&other)
                        : data(other.data), __size(other.__size)
                {
                        other.__size = 0;
                        other.data = nullptr;
                }

                /// @brief Destructor.
                ~Buffer()
                {
                        ::operator delete(this->data);
                }

                template<typename... ARGS>
                void construct(this Buffer<T> &self, Typing::USize index, ARGS &&...args)
                {
                        if (index >= self.__size)
                                throw Exceptions::OutOfRange(index, self.__size);

                        new (&self.data[index]) T(std::forward<ARGS>(args)...);
                }

                void destroy(this Buffer<T> &self, Typing::USize index)
                {
                        if (index >= self.__size)
                                throw Exceptions::OutOfRange(index, self.__size);
                        
                        self.data[index].~T();
                }

                /// @brief Copies some objects from here to another buffer.
                /// @param dest destination buffer
                /// @param size object count
                /// @return destination buffer
                Buffer<T> &copy(this const Buffer<T> &self, Buffer<T> &dest, Typing::USize size)
                {
                        if (size > self.__size)
                                throw Exceptions::BufferOverflow(size, self.__size);
                        if (size > dest.__size)
                                throw Exceptions::BufferOverflow(size, dest.__size);

                        memcpy(dest.data, self.data, size);

                        return dest;
                }

                /// @brief Sets some objects from this buffer to a given value.
                /// @param c value to set
                /// @param size object count
                /// @return self
                Buffer<T> &set(this Buffer<T> &self, T c, Typing::USize size)
                {
                        if (size > self.__size)
                                throw Exceptions::BufferOverflow(size, self.__size);

                        memset(self.data, c, size);

                        return self;
                }

                /// @brief Moves some objects from here to another buffer.
                ///
                /// This method is similar to copy() but it prevents both buffers
                /// from overlapping.
                /// @param dest destination buffer
                /// @param size object count
                /// @return destination buffer
                Buffer<T> &move(this const Buffer<T> &self, Buffer<T> &dest, Typing::USize size)
                {
                        if (size > self.__size)
                                throw Exceptions::BufferOverflow(size, self.__size);
                        if (size > dest.__size)
                                throw Exceptions::BufferOverflow(size, dest.__size);

                        memmove(dest.data, self.data, size);

                        return dest;
                }

                /// @brief Compares some objects between this buffer and another one.
                /// @param other buffer to compare
                /// @param size object count
                /// @return <0 if self < dest, 0 if self == dest and >0 if self > dest
                int compare(this const Buffer<T> &self, const Buffer<T> &other, Typing::USize size)
                {
                        if (size > self.__size)
                                throw Exceptions::BufferOverflow(size, self.__size);
                        if (size > other.__size)
                                throw Exceptions::BufferOverflow(size, other.__size);

                        int result = memcmp(self.data, other.data, size);

                        return result;
                }

                /// @brief Gets a constant pointer to the raw memory.
                /// @return constant raw buffer
                const T *get(this const Buffer<T> &self)
                {
                        return self.data;
                }

                /// @brief Gets the size of the buffer.
                /// @return buffer size
                Typing::USize size(this const Buffer<T> &self)
                {
                        return self.__size;
                }

                /// @brief Gets a constant pointer to the base of the raw buffer.
                /// @return pointer to raw buffer base
                const T *begin(this const Buffer<T> &self)
                {
                        return self.data;
                }

                /// @brief Gets a constant pointer to the end of the raw buffer.
                /// @return pointer to raw buffer end
                const T *end(this const Buffer<T> &self)
                {
                        return self.data + self.__size;
                }

                /// @brief Gets a pointer to the base of the raw buffer.
                /// @return pointer to raw buffer base
                T *begin(this Buffer<T> &self)
                {
                        return self.data;
                }

                /// @brief Gets a pointer to the end of the raw buffer.
                /// @return pointer to raw buffer end
                T *end(this Buffer<T> &self)
                {
                        return self.data + self.__size;
                }

                /// @brief Resizes the buffer.
                ///
                /// This method gives a new size to this buffer and reallocates the
                /// data to match the new size.
                /// @param new_size new size of the buffer
                void resize(this Buffer<T> &self, Typing::USize new_size)
                {
                        T *new_data = new T[new_size];

                        for (Typing::USize i = 0; i < self.__size; ++i)
                                new_data[i] = self.data[i];

                        delete[] self.data;
                        self.data = new_data;
                        self.__size = new_size;
                }

                /// @brief Checks if a buffer has same size and same content.
                /// @param other other buffer
                /// @return comparison result
                bool operator ==(this const Buffer<T> &self, const Buffer<T> &other)
                {
                        return self.__size == other.__size and self.compare(other, self.__size) == 0;
                }

                /// @brief Checks if a buffer does not have same size or same content.
                /// @param other other buffer
                /// @return comparison result
                bool operator !=(this const Buffer<T> &self, const Buffer<T> &other)
                {
                        return not (self == other);
                }

                /// @brief Checks if this buffer is smaller than another.
                /// @param other other buffer
                /// @return comparison result
                bool operator <(this const Buffer<T> &self, const Buffer<T> &other)
                {
                        Typing::USize size = self.__size < other.__size ? self.__size : other.__size;
                        int result = self.compare(other, size);
                        if (result != 0)
                                return result < 0;
                        return self.__size < other.__size;
                }

                /// @brief Checks if this buffer is bigger than another.
                /// @param other other buffer
                /// @return comparison result
                bool operator >(this const Buffer<T> &self, const Buffer<T> &other)
                {
                        Typing::USize size = self.__size < other.__size ? self.__size : other.__size;
                        int result = self.compare(other, size);
                        if (result != 0)
                                return result > 0;
                        return self.__size > other.__size;
                }

                /// @brief Checks if this buffer is smaller than or equals another.
                /// @param other other buffer
                /// @return comparison result
                bool operator <=(this const Buffer<T> &self, const Buffer<T> &other)
                {
                        return self < other or self == other;
                }

                /// @brief Checks if this buffer is bigger than or equals another.
                /// @param other other buffer
                /// @return comparison result
                bool operator >=(this const Buffer<T> &self, const Buffer<T> &other)
                {
                        return self > other or self == other;
                }

                /// @brief Gets an object from the buffer.
                ///
                /// The object is returned by index. If the index is too high,
                /// Exceptions::OutOfRange will be thrown.
                /// @param index position of the object in the buffer
                T &operator [](this Buffer<T> &self, Typing::USize index)
                {
                        if (index >= self.__size)
                                throw Exceptions::OutOfRange(index, self.__size);

                        return self.data[index];
                }

                /// @brief Gets an object from the buffer.
                ///
                /// The object is returned by index. If the index is too high,
                /// Exceptions::OutOfRange will be thrown.
                /// @param index position of the object in the buffer
                const T &operator [](this const Buffer<T> &self, Typing::USize index)
                {
                        if (index >= self.__size)
                                throw Exceptions::OutOfRange(index, self.__size);

                        return self.data[index];
                }

                /// @brief Copy assignment operator.
                /// @param other buffer to copy data to
                Buffer<T> &operator =(this Buffer<T> &self, const Buffer<T> &other)
                {
                        if (&self != &other) {
                                ::operator delete(self.data);
                                self.data = allocate(other.__size);
                                self.__size = other.__size;
                        }

                        return self;
                }

                /// @brief Move assignment operator.
                /// @param other buffer to move data and size to
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
} // namespace Melon::Memory