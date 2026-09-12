#pragma once

#include <lib/typing.hpp>
#include <lib/forward.hpp>
#include <panic.hpp>

namespace Kiwi::Lib
{
        template<typename T, usize LENGTH>
        class Array
        {
                T data[LENGTH];

                template<typename... ARGS>
                bool construct(this Array<T, LENGTH> &self, usize index, ARGS &&...args)
                {
                        if (index >= LENGTH)
                                return false;

                        self.data[index](forward(args)...);
                        return true;
                }

                bool destroy(this Array<T, LENGTH> &self, usize index)
                {
                        if (index >= LENGTH)
                                return false;

                        self.data[index].~T();
                        return true;
                }

        public:
                Array() = default;

                Array(const T (&objects)[LENGTH])
                {
                        for (usize i = 0; i < LENGTH; i++) {
                                if (not this->construct(i, objects[i]))
                                        panic("failed to construct object at %u", i);
                        }
                }

                Array(const Array<T, LENGTH> &other)
                        : data(other.data)
                {
                        for (usize i = 0; i < LENGTH; ++i)
                                this->construct(i, other.data[i]);
                }

                Array(Array<T, LENGTH> &&other) noexcept
                        : data(move(other.data))
                {}

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

                T *begin(this const Array<T, LENGTH> &self)
                {
                        return self.data;
                }

                T *end(this const Array<T, LENGTH> &self)
                {
                        return self.data + LENGTH;
                }

                Array<T, LENGTH> &operator =(this Array<T, LENGTH> &self, const Array<T, LENGTH> &other)
                {
                        if (&self != &other) {
                                self.data = other.data;
                                for (usize i = 0; i < LENGTH; ++i)
                                        self.construct(i, other.data[i]); 
                        }

                        return self;
                }

                Array<T, LENGTH> &operator =(this Array<T, LENGTH> &self, Array<T, LENGTH> &&other) noexcept
                {
                        if (&self != &other)
                                self.data = move(other.data);

                        return self;    
                }
        };
} // namespace Kiwi::Lib