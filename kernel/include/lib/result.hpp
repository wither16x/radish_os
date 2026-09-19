#pragma once

#include <lib/memory.hpp>
#include <lib/alloc.hpp>
#include <panic_simple.hpp>

namespace Kiwi::Lib
{
        template<typename E>
        class Error
        {
        public:
                E error;
        };

        template<typename T, typename E>
        class Result
        {
                union
                {
                        T _value;
                        E _error;
                };

                bool has_value;

        public:
                Result(T v)
                        : _value(move(v)), has_value(true)
                {}

                Result(Error<E> u)
                        : _error(move(u.error)), has_value(false)
                {}

                Result(const Result<T, E> &other)
                        : has_value(other.has_value)
                {
                        if (this->has_value)
                                new (&this->_value) T(other._value);
                        else
                                new (&this->_error) E(other._error);
                }

                Result(Result<T, E> &&other) noexcept
                        : has_value(other.has_value)
                {
                        if (this->has_value)
                                new (&this->_value) T(move(other._value));
                        else
                                new (&this->_error) E(move(other._error));
                }

                ~Result()
                {
                        if (this->has_value)
                                _value.~T();
                        else
                                _error.~E();
                }

                const T &value(this const Result<T, E> &self)
                {
                        if (not self.has_value)
                                panic_simple("Result::value() on error");
                        return self._value;
                }

                const E &error(this const Result<T, E> &self)
                {
                        if (self.has_value)
                                panic_simple("Result::error() on success");

                        return self._error;
                }

                Result& operator=(this Result<T, E> &self, const Result& other)
                {
                        if (&self == &other)
                                return self;

                        self.~Result();

                        self.has_value = other.has_value;
                        if (self.has_value)
                                new (&self._value) T(other._value);
                        else
                                new (&self._error) E(other._error);

                        return self;
                }

                Result& operator=(this Result<T, E> &self, Result&& other) noexcept
                {
                        if (&self == &other)
                                return self;

                        self.~Result();

                        self.has_value = other.has_value;
                        if (self.has_value)
                                new (&self._value) T(move(other._value));
                        else
                                new (&self._error) E(move(other._error));

                        return self;
                }
        };

        template<typename T, typename E>
        class Result<T&, E>
        {
                T *_value_ptr;
                E _error;

                bool has_value;

        public:
                Result(T &v)
                        : _value_ptr(&v), has_value(true)
                {}

                Result(Error<E> u)
                        : _error(move(u.error)), has_value(false)
                {}

                Result(const Result<T&, E> &other)
                        : has_value(other.has_value)
                {
                        if (this->has_value)
                                this->_value_ptr = other._value_ptr;
                        else
                                new (&this->_error) E(other._error);
                }

                Result(Result<T&, E> &&other) noexcept
                        : has_value(other.has_value)
                {
                        if (this->has_value)
                                this->_value_ptr = other._value_ptr;
                        else
                                new (&this->_error) E(move(other._error));
                }

                ~Result()
                {
                        if (not this->has_value)
                                _error.~E();
                }

                T &value(this const Result<T&, E> &self)
                {
                        if (not self.has_value)
                                panic_simple("Result::value() on error");

                        return *self._value_ptr;
                }

                const E &error(this const Result<T&, E> &self)
                {
                        if (self.has_value)
                                panic_simple("Result::error() on success");

                        return self._error;
                }

                Result& operator=(this Result<T&, E> &self, const Result& other)
                {
                        if (&self == &other)
                                return self;

                        self.~Result();

                        self.has_value = other.has_value;
                        if (self.has_value)
                                self._value_ptr = other._value_ptr;
                        else
                                new (&self._error) E(other._error);

                        return self;
                }

                Result& operator=(this Result<T&, E> &self, Result&& other) noexcept
                {
                        if (&self == &other)
                                return self;

                        self.~Result();

                        self.has_value = other.has_value;
                        if (self.has_value)
                                self._value_ptr = other._value_ptr;
                        else
                                new (&self._error) E(move(other._error));

                        return self;
                }
        };
} // namespace Kiwi::Lib