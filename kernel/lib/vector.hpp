#pragma once

#include <lib/alloc.hpp>
#include <lib/typing.hpp>

namespace kernel::lib {

template<typename T>
class Vector {
private:
        T *data;

        usize cap;
        usize length;

public:
        // Constructors
        // --------------------------------------------------------

        Vector()
        {
                this->cap = 1;
                this->length = 0;
                this->data = new T[this->cap];
        }

        Vector(const Vector<T> &other)
        {
                this->length = other.length;
                this->cap = other.cap;

                this->data = new T[this->cap];
                for (usize i = 0; i < this->length; i++)
                        this->data[i] = other.data[i];
        }

        Vector(Vector<T> &&other) noexcept
        {
                this->data = other.data;
                this->length = other.length;
                this->cap = other.cap;

                other.data = nullptr;
                other.length = 0;
                other.cap = 0;
        }
        // --------------------------------------------------------

        // Destructors
        // --------------------------------------------------------
        ~Vector()
        {
                delete[] this->data;
        }
        // --------------------------------------------------------

        // Both methods below for range-based for
        // --------------------------------------------------------
        const T *begin(this const Vector<T> &self)
        {
                return self.data;
        }

        const T *end(this const Vector<T> &self)
        {
                return self.data + self.length;
        }
        // --------------------------------------------------------

        usize size(this const Vector<T> &self)
        {
                return self.length;
        }

        usize capacity(this const Vector<T> &self)
        {
                return self.cap;
        }

        void set(this Vector<T> &self, usize idx, const T& n)
        {
                if (idx >= self.length)
                        return;         // do not extend the vector

                self.data[idx] = n;
        }

        void push_back(this Vector<T> &self, const T& n)
        {
                if (self.length == self.cap) {
                        if (self.cap == 0)
                                self.cap = 1;
                        self.cap *= 2;
                        T *new_data = new T[self.cap];
                        for (usize i = 0; i < self.length; ++i)
                                new_data[i] = self.data[i];
                        delete[] self.data;
                        self.data = new_data;
                }

                self.data[self.length] = n;
                self.length++;
        }

        void pop_back(this Vector<T> &self)
        {
                if (self.length > 0)
                        self.length--;
        }

        void erase(this Vector<T> &self, usize index)
        {
                if (index >= self.length)
                        return;

                for (usize i = index; i < self.length - 1; i++)
                        self.data[i] = self.data[i + 1];

                self.length--;
        }

        void resize(this Vector<T> &self, usize new_size)
        {
                if (new_size > self.cap) {
                        if (self.cap == 0)
                                self.cap = new_size;

                        while (self.cap < new_size)
                                self.cap *= 2;

                        T *new_data = new T[self.cap];
                        for (usize i = 0; i < self.length; ++i)
                                new_data[i] = self.data[i];

                        delete[] self.data;
                        self.data = new_data;
                }

                self.length = new_size;
        }

        const T *get_data(this const Vector<T> &self)
        {
                return self.data;
        }

        T *get_data(this Vector<T> &self)
        {
                return self.data;
        }

        // Operators
        // --------------------------------------------------------
        Vector &operator =(const Vector<T> &other)
        {
                if (this == &other)
                        return *this;

                delete[] this->data;

                this->length = other.length;
                this->cap = other.cap;

                this->data = new T[this->cap];
                for (usize i = 0; i < this->length; i++)
                        this->data[i] = other.data[i];

                return *this;
        }

        Vector &operator =(Vector<T> &&other) noexcept
        {
                if (this == &other)
                        return *this;

                delete[] this->data;

                this->data = other.data;
                this->length = other.length;
                this->cap = other.cap;

                other.data = nullptr;
                other.length = 0;
                other.cap = 0;                

                return *this;
        }

        T &operator [](this Vector<T> &self, usize idx)
        {
                return self.data[idx];
        }

        const T &operator [](this const Vector<T> &self, usize idx)
        {
                return self.data[idx];
        }

        // --------------------------------------------------------
};

} /* namespace kernel::lib */