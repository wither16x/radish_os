#pragma once

#include <lib/typing.hpp>
#include <lib/forward.hpp>
#include <lib/alloc.hpp>
#include <panic.hpp>

namespace Kiwi::Lib
{
        template<typename T, usize LENGTH>
        class Array
        {
                T data[LENGTH];

        public:
                Array()
                {
                        memset(this->data, 0, LENGTH);
                }

                Array(const T (&objects)[LENGTH])
                {
                        for (usize i = 0; i < LENGTH; i++) {
                                if (not this->construct(i, objects[i]))
                                        panic("failed to construct object at %u", i);
                        }
                }

                Array(const Array<T, LENGTH> &other)
                {
                        for (usize i = 0; i < LENGTH; ++i)
                                memcpy(&this->data[i], &other.data[i], sizeof(T));

                        for (usize i = 0; i < LENGTH; ++i)
                                this->construct(i, other.data[i]);
                }

                Array(Array<T, LENGTH> &&other) noexcept
                {
                        for (usize i = 0; i < LENGTH; ++i) {
                                memcpy(&this->data[i], &other.data[i], sizeof(T));
                                this->destroy(other.data[i]);
                        }

                        for (usize i = 0; i < LENGTH; ++i)
                                this->construct(i, other.data[i]);
                }

                ~Array()
                {
                        this->clear();
                }

                void clear(this Array<T, LENGTH> &self)
                {
                        for (usize i = 0; i < LENGTH; i++) {
                                if (not self.destroy(i))
                                        panic("failed to destroy object at %u", i);
                        }
                }

                const T *raw(this const Array<T, LENGTH> &self)
                {
                        return self.data;
                }

                T *begin(this Array<T, LENGTH> &self)
                {
                        return self.data;
                }

                T *end(this Array<T, LENGTH> &self)
                {
                        return self.data + LENGTH;
                }

                const T *begin(this const Array<T, LENGTH> &self)
                {
                        return self.data;
                }

                const T *end(this const Array<T, LENGTH> &self)
                {
                        return self.data + LENGTH;
                }

                template<typename... ARGS>
                bool construct(this Array<T, LENGTH> &self, usize index, ARGS &&...args)
                {
                        if (index >= LENGTH)
                                return false;

                        new (&self.data[index]) T(forward<ARGS>(args)...);
                        return true;
                }

                bool destroy(this Array<T, LENGTH> &self, usize index)
                {
                        if (index >= LENGTH)
                                return false;

                        self.data[index].~T();
                        return true;
                }

                Array<T, LENGTH> &operator =(this Array<T, LENGTH> &self, const Array<T, LENGTH> &other)
                {
                        if (&self != &other) {
                                for (usize i = 0; i < LENGTH; ++i)
                                        memcpy(&self.data[i], &other.data[i], sizeof(T));

                                for (usize i = 0; i < LENGTH; ++i)
                                        self.construct(i, other.data[i]); 
                        }

                        return self;
                }

                Array<T, LENGTH> &operator =(this Array<T, LENGTH> &self, Array<T, LENGTH> &&other) noexcept
                {
                        if (&self != &other) {
                                for (usize i = 0; i < LENGTH; ++i) {
                                        memcpy(&self.data[i], &other.data[i], sizeof(T));
                                        self.destroy(other.data[i]);
                                }

                                for (usize i = 0; i < LENGTH; ++i)
                                        self.construct(i, other.data[i]);
                        }

                        return self;
                }

                T &operator [](this Array<T, LENGTH> &self, usize index)
                {
                        if (index >= LENGTH)
                                panic("index out of range");

                        return self.data[index];
                }

                const T &operator [](this const Array<T, LENGTH> &self, usize index)
                {
                        if (index >= LENGTH)
                                panic("index out of range");

                        return self.data[index]; 
                }
        };
} // namespace Kiwi::Lib