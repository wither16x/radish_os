#pragma once

#include <lib/typing.hpp>

namespace kernel::lib {

extern "C" {

/// Copy some bytes from a buffer to another. 
void *memcpy(void *__restrict dest, const void *__restrict src, usize n);
/// Set some bytes in a buffer to a given value.
void *memset(void *s, int c, usize n);
/// Copy some bytes from a buffer to another. This function also
/// avoids overlaps.
void *memmove(void *dest, const void *src, usize n);
/// Compare some bytes from two buffers together.
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

// NOTE: all the functions below assume that the strings
// we give to them are null-terminated

/// Compare two strings.
int strcmp(const char *s1, const char *s2);
/// Compare `length` characters from two strings.
int strncmp(const char *s1, const char *s2, usize length);
/// Return the length of a string.
usize strlen(const char *s);
/// Copy a string to another.
void strcpy(const char *src, char *dest);
/// Check if a string starts with another string.
bool strstartswith(const char *s1, const char *s2);

} /* namespace kernel::lib */