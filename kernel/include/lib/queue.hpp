#pragma once

#include <lib/typing.hpp>

namespace kernel::lib {

template<typename T, usize SIZE>
class Queue {
private:
        T data[SIZE];
        usize tail;
        usize head;
        usize count;

public:
        Queue()
                : tail(0), head(0), count(0)
        {}

        void enqueue(this Queue<T, SIZE> &self, const T &element)
        {
                self.data[self.tail] = element;
                self.tail = (self.tail + 1) % SIZE;
                if (self.full())
                        self.head = (self.head + 1) % SIZE;
                else
                        self.count++;
        }

        T &dequeue(this Queue<T, SIZE> &self)
        {
                if (self.count == 0)
                        return;

                T element = self.data[self.head];
                
                self.head = (self.head + 1) % SIZE;
                self.count--;

                return element;
        }

        const T &get(this const Queue<T, SIZE> &self, usize i)
        {
                return self.data[i];
        }

        bool full(this const Queue<T, SIZE> &self)
        {
                return self.count == SIZE;
        }

        bool empty(this const Queue<T, SIZE> &self)
        {
                return self.count == 0;
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