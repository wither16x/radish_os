#pragma once

#include <stddef.h>
#include "__utils/decls.h"

BEGIN_DECLS

size_t strlen(const char *s);

void *memcpy(void *__restrict dest, const void *__restrict src, size_t n);
void *memset(void *s, int c, size_t n);
void *memmove(void *dest, const void *src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
void *memchr(const void *s, int c, size_t n);

int strcmp(const char *lhs, const char *rhs);
int strncmp(const char *lhs, const char *rhs, size_t count);
char *strcpy(char *dest, const char *src);
char* strncpy(char* __restrict dest, const char* __restrict src, size_t count);

char *strerror(int errnum);

char *strcat(char *__restrict dest, const char *__restrict src);

END_DECLS