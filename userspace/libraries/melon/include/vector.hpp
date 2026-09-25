#pragma once

#include "internal/exceptions/out_of_range.hpp"
#include "internal/memory/buffer.hpp"
#include "internal/memory/move.hpp"
#include "typing.hpp"

/// @brief Contains dynamic vector manipulation features.
namespace Melon::Vector
{
        /// @brief A linear dynamic generic container.
        template<typename T>
        class Vector
        {
                Memory::Buffer<T> buf;
                Typing::USize obj_count;
                Typing::USize __capacity;

                void extend(this Vector<T> &self)
                {
                        Typing::USize new_capacity = self.__capacity * 2;
                        Memory::Buffer<T> new_buf(new_capacity);

                        for (Typing::USize i = 0; i < self.obj_count; ++i) {
                                new_buf.construct(i, Memory::move(self.buf[i]));
                                self.buf.destroy(i);
                        }

                        self.__capacity = new_capacity;
                        self.buf = Memory::move(new_buf);
                }

        public:
                /// @brief Default constructor.
                ///
                /// Allocates minimal space to store the buffer.
                Vector()
                        : buf(1), obj_count(0), __capacity(1)
                {}

                /// @brief Constructs the vector from existing objects.
                /// @param objects array of objects
                template<Typing::USize N>
                Vector(const T (&objects)[N])
                        : buf(N), obj_count(0), __capacity(N)
                {
                        for (Typing::USize i = 0; i < N; i++)
                                this->buf.construct(i, objects[i]);
                        this->obj_count = N;
                }

                /// @brief Copy constructor.
                /// @param other vector to copy
                Vector(const Vector<T> &other)
                        : buf(other.__capacity), obj_count(0), __capacity(other.__capacity)
                {
                        for (Typing::USize i = 0; i < other.obj_count; ++i)
                                this->buf.construct(i, other.buf[i]);
                        this->obj_count = other.obj_count;
                }

                /// @brief Move constructor.
                /// @param other vector to move
                Vector(Vector<T> &&other)
                        : buf(Memory::move(other.buf)), obj_count(other.obj_count), __capacity(other.__capacity)
                {
                        other.__capacity = 0;
                        other.obj_count = 0;
                }

                ~Vector()
                {
                        this->clear();
                }

                /// @brief Gets a pointer to the base of the data.
                /// @return constant pointer to base of data
                const T *begin(this const Vector<T> &self)
                {
                        return self.buf.begin();
                }

                /// @brief Gets a pointer to the end of the data.
                /// @return constant pointer to end of data
                const T *end(this const Vector<T> &self)
                {
                        return self.buf.begin() + self.obj_count;
                }

                /// @brief Gets a pointer to the base of the data.
                /// @return pointer to base of data
                T *begin(this Vector<T> &self)
                {
                        return self.buf.begin();
                }

                /// @brief Gets a pointer to the end of the data.
                /// @return pointer to end of data
                T *end(this Vector<T> &self)
                {
                        return self.buf.begin() + self.obj_count;
                }

                /// @brief Add an object to the end of the vector.
                ///
                /// If the number of objects in the vector exceeds the vector's capacity,
                /// the buffer is extended to two times the current capacity.
                /// @param object object to add
                void pushBack(this Vector<T> &self, const T &object)
                {
                        if (self.obj_count >= self.__capacity)
                                self.extend();

                        self.buf.construct(self.obj_count, object);
                        ++self.obj_count;
                }

                /// @brief Constructs an object and add it at the end of the vector.
                ///
                /// If the number of objects in the vector exceeds the vector's capacity,
                /// the buffer is extended to two times the current capacity.
                /// @param args constructor arguments
                template<typename... ARGS>
                void emplaceBack(this Vector<T> &self, ARGS &&...args)
                {
                        if (self.obj_count >= self.__capacity)
                                self.extend();

                        self.buf.construct(self.obj_count, Forward::forward<ARGS>(args)...);
                        ++self.obj_count;
                }

                /// @brief Removes the last object from the vector.
                ///
                /// Actually, the object is not removed. The object count is basically
                /// decremented, making the last object unaccessible.
                /// @return last object
                T popBack(this Vector<T> &self)
                {
                        if (self.obj_count > 0) {
                                --self.obj_count;
                                T obj = Memory::move(self.buf[self.obj_count]);
                                self.buf.destroy(self.obj_count);
                                return obj;
                        } else {
                                throw Exceptions::OutOfRange(self.obj_count, 0);
                        }
                }

                /// @brief Erases object at a given position in the vector.
                /// @param index object position
                void erase(this Vector<T> &self, Typing::USize index)
                {
                        if (index >= self.obj_count)
                                throw Exceptions::OutOfRange(index, self.obj_count);

                        for (Typing::USize i = index; i < self.obj_count - 1; i++)
                                self.buf[i] = Memory::move(self.buf[i + 1]);

                        --self.obj_count;
                        self.buf.destroy(self.obj_count);
                }

                /// @brief Clears the vector.
                ///
                /// Destroys all objects in the vector and set its size to 0.
                void clear(this Vector<T> &self)
                {
                        for (Typing::USize i = 0; i < self.obj_count; i++)
                                self.buf.destroy(i);
                        self.obj_count = 0;
                }

                /// @brief Gets a pointer to the internal buffer storage.
                /// @return constant pointer to the internal buffer storage
                const T *data(this const Vector<T> &self)
                {
                        return self.buf.get();
                }

                /// @brief Gets a copy of the internal buffer.
                /// @return copy of the internal buffer
                Memory::Buffer<T> toBuffer(this const Vector<T> &self)
                {
                        Memory::Buffer<T> result(self.obj_count);
                        for (Typing::USize i = 0; i < self.obj_count; ++i)
                                result.construct(i, self.buf[i]);
                        return result;
                }

                /// @brief Gets the length of the vector.
                /// @return object count
                Typing::USize length(this const Vector<T> &self)
                {
                        return self.obj_count;
                }

                /// @brief Gets the capacity of the vector.
                /// @return capacity
                Typing::USize capacity(this const Vector<T> &self)
                {
                        return self.__capacity;
                }

                /// @brief Checks if the vector is empty.
                /// @return boolean
                bool isEmpty(this const Vector<T> &self)
                {
                        return self.obj_count == 0;
                }

                bool contains(this const Vector<T> &self, const T &object)
                {
                        for (auto &o : self) {
                                if (o == object)
                                        return true;
                        }

                        return false;
                }

                /// @brief Checks if both buffers are the same.
                ///
                /// This checks for the buffer only, not for the object count and
                /// capacity.
                /// @return boolean
                bool operator ==(this const Vector<T> &self, const Vector<T> &other)
                {
                        if (self.obj_count != other.obj_count)
                                return false;

                        for (Typing::USize i = 0; i < self.obj_count; ++i) {
                                if ( self.buf[i] != other.buf[i])
                                        return false;
                        }

                        return true;
                }

                /// @brief Gets an object from the vector.
                /// @param index object position
                T &operator [](this Vector<T> &self, Typing::USize index)
                {
                        if (index >= self.obj_count)
                                throw Exceptions::OutOfRange(index, self.obj_count);

                        return self.buf[index];
                }

                /// @brief Gets an object from the vector.
                /// @param index object position
                const T &operator [](this const Vector<T> &self, Typing::USize index)
                {
                        if (index >= self.obj_count)
                                throw Exceptions::OutOfRange(index, self.obj_count);

                        return self.buf[index];
                }

                /// @brief Copy assignment operator.
                /// @param other vector to copy
                /// @return self
                Vector<T> &operator =(this Vector<T> &self, const Vector<T> &other)
                {
                        if (&self != &other) {
                                self.clear();
                                self.buf = Memory::Buffer<T>(other.__capacity);
                                self.__capacity = other.__capacity;

                                for (Typing::USize i = 0; i < other.obj_count; ++i)
                                        self.buf.construct(i, other.buf[i]);
                                self.obj_count = other.obj_count;
                        }

                        return self;
                }

                /// @brief Move assignment operator.
                /// @param other vector to copy
                /// @return self
                Vector<T> &operator =(this Vector<T> &self, Vector<T> &&other)
                {
                        if (&self != &other) {
                                self.clear();
                                self.buf = Memory::move(other.buf);
                                self.__capacity = other.__capacity;
                                self.obj_count = other.obj_count;

                                other.__capacity = 0;
                                other.obj_count = 0;
                        }

                        return self;
                }
        };
} // namespace Melon::Vector