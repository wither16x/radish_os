#pragma once

#include <lib/typing.hpp>
#include <lib/buffer.hpp>
#include <lib/memory.hpp>
#include <lib/forward.hpp>
#include <lib/result.hpp>

namespace Kiwi::Lib
{
        template<typename T>
        class Vector
        {
                Buffer<T> buf;
                usize obj_count;
                usize __capacity;

                void extend(this Vector<T> &self)
                {
                        usize new_capacity = self.__capacity * 2;
                        Buffer<T> new_buf(new_capacity);

                        for (usize i = 0; i < self.obj_count; ++i) {
                                new_buf.construct(i, move(self.buf[i]));
                                self.buf.destroy(i);
                        }

                        self.__capacity = new_capacity;
                        self.buf = move(new_buf);
                }

        public:
                Vector()
                        : buf(1), obj_count(0), __capacity(1)
                {}

                template<usize N>
                Vector(const T (&objects)[N])
                        : buf(N), obj_count(0), __capacity(N)
                {
                        for (usize i = 0; i < N; i++)
                                this->buf.construct(i, objects[i]);
                        this->obj_count = N;
                }

                Vector(const Vector<T> &other)
                        : buf(other.__capacity), obj_count(0), __capacity(other.__capacity)
                {
                        for (usize i = 0; i < other.obj_count; ++i)
                                this->buf.construct(i, other.buf[i]);
                        this->obj_count = other.obj_count;
                }

                Vector(Vector<T> &&other) noexcept
                        : buf(move(other.buf)), obj_count(other.obj_count), __capacity(other.__capacity)
                {
                        other.__capacity = 0;
                        other.obj_count = 0;
                }

                ~Vector()
                {
                        this->clear();
                }

                const T *begin(this const Vector<T> &self)
                {
                        return self.buf.begin();
                }

                const T *end(this const Vector<T> &self)
                {
                        return self.buf.begin() + self.obj_count;
                }

                T *begin(this Vector<T> &self)
                {
                        return self.buf.begin();
                }

                T *end(this Vector<T> &self)
                {
                        return self.buf.begin() + self.obj_count;
                }

                void pushBack(this Vector<T> &self, const T &object)
                {
                        if (self.obj_count >= self.__capacity)
                                self.extend();

                        self.buf.construct(self.obj_count, object);
                        ++self.obj_count;
                }

                template<typename... ARGS>
                void emplaceBack(this Vector<T> &self, ARGS &&...args)
                {
                        if (self.obj_count >= self.__capacity)
                                self.extend();

                        self.buf.construct(self.obj_count, forward<ARGS>(args)...);
                        ++self.obj_count;
                }

                Result<T, bool> popBack(this Vector<T> &self)
                {
                        if (self.obj_count > 0) {
                                --self.obj_count;
                                T obj = move(self.buf[self.obj_count]);
                                self.buf.destroy(self.obj_count);
                                return obj;
                        } else {
                                return Error(false);
                        }
                }

                bool erase(this Vector<T> &self, usize index)
                {
                        if (index >= self.obj_count)
                                return false;

                        for (usize i = index; i < self.obj_count - 1; i++)
                                self.buf[i] = move(self.buf[i + 1]);

                        --self.obj_count;
                        self.buf.destroy(self.obj_count);
                        return true;
                }

                void clear(this Vector<T> &self)
                {
                        for (usize i = 0; i < self.obj_count; i++)
                                self.buf.destroy(i);
                        self.obj_count = 0;
                }

                const T *data(this const Vector<T> &self)
                {
                        return self.buf.get();
                }

                Buffer<T> toBuffer(this const Vector<T> &self)
                {
                        Buffer<T> result(self.obj_count);
                        for (usize i = 0; i < self.obj_count; ++i)
                                result.construct(i, self.buf[i]);
                        return result;
                }

                usize length(this const Vector<T> &self)
                {
                        return self.obj_count;
                }

                usize capacity(this const Vector<T> &self)
                {
                        return self.__capacity;
                }

                bool isEmpty(this const Vector<T> &self)
                {
                        return self.obj_count == 0;
                }

                void resize(this Vector<T> &self, usize new_size)
                {
                        self.buf.resize(new_size);
                        if (self.buf.size() < self.__capacity)
                                self.obj_count = self.__capacity;
                        self.__capacity = self.buf.size();
                }

                bool operator ==(this const Vector<T> &self, const Vector<T> &other)
                {
                        if (self.obj_count != other.obj_count)
                                return false;

                        for (usize i = 0; i < self.obj_count; ++i) {
                                if ( self.buf[i] != other.buf[i])
                                        return false;
                        }

                        return true;
                }

                T &operator [](this Vector<T> &self, usize index)
                {
                        if (index >= self.obj_count)
                                panic("index out of range");

                        return self.buf[index];
                }

                const T &operator [](this const Vector<T> &self, usize index)
                {
                        if (index >= self.obj_count)
                                panic("index out of range");

                        return self.buf[index];
                }

                Vector<T> &operator =(this Vector<T> &self, const Vector<T> &other)
                {
                        if (&self != &other) {
                                self.clear();
                                self.buf = Buffer<T>(other.__capacity);
                                self.__capacity = other.__capacity;

                                for (usize i = 0; i < other.obj_count; ++i)
                                        self.buf.construct(i, other.buf[i]);
                                self.obj_count = other.obj_count;
                        }

                        return self;
                }

                Vector<T> &operator =(this Vector<T> &self, Vector<T> &&other)
                {
                        if (&self != &other) {
                                self.clear();
                                self.buf = move(other.buf);
                                self.__capacity = other.__capacity;
                                self.obj_count = other.obj_count;

                                other.__capacity = 0;
                                other.obj_count = 0;
                        }

                        return self;
                }
        };
} // namespace Kiwi::Lib