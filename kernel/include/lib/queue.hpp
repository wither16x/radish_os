#pragma once

#include <lib/memory.hpp>
#include <lib/typing.hpp>

namespace kernel::lib {

template<typename T, usize SIZE>
class Queue {
private:
        T data[SIZE];
        usize tail;
        usize head;

public:
        Queue()
                : tail(0), head(0)
        {}

        void enqueue(this Queue<T, SIZE> &self, const T &element)
        {
                usize next = (self.head + 1) % SIZE;
                if (next == self.tail)
                        return;

                self.data[self.head] = element;
                self.head = next;
        }

        void dequeue(this Queue<T, SIZE> &self, T *buf)
        {
                if (self.head == self.tail)
                        return;

                memcpy(buf, &self.data[self.tail], sizeof(*buf));
                self.tail = (self.tail + 1) % SIZE;
        }

        const T &get(this const Queue<T, SIZE> &self, usize i)
        {
                return self.data[i];
        }

        const T *begin(this const Queue<T, SIZE> &self)
        {
                return self.data;
        }

        const T *end(this const Queue<T, SIZE> &self)
        {
                return &self.data[SIZE];
        }
};

} /* namespace kernel::lib */