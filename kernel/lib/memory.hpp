#pragma once

#include <lib/typing.hpp>

namespace kernel::lib {

extern "C" {

void *memcpy(void *__restrict dest, const void *__restrict src, usize n);
void *memset(void *s, int c, usize n);
void *memmove(void *dest, const void *src, usize n);
int memcmp(const void *s1, const void *s2, usize n);

}

// for copy() and move()
//
// remove_reference
// -----------------------------------------------------------
template<typename T>
struct remove_reference {
        using type = T;
};

template<typename T>
struct remove_reference<T&> {
        using type = T;
};

template<typename T>
struct remove_reference<T&&> {
        using type = T; 
};

template<typename T>
using remove_reference_t = typename remove_reference<T>::type;
// -----------------------------------------------------------

template<typename T>
T *copy(const T *first, const T *last, T *dest)
{
        while (first != last)
                *dest++ = *first++;

        return dest;
}

template<typename T>
constexpr remove_reference_t<T>&& move(T&& value) noexcept
{
        return static_cast<remove_reference_t<T>&&>(value);
}

// strings (char pointers)

int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, usize length);
usize strlen(const char *s);
void strcpy(const char *src, char *dest);
bool strstartswith(const char *s1, const char *s2);

} /* namespace kernel::lib */