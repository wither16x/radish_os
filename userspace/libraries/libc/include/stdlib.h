#pragma once

#include <stddef.h>
#include <stdbool.h>
#include "__utils/decls.h"

typedef struct div
{
        int quot;
        int rem;
} div_t;

typedef struct ldiv
{
        long quot;
        long rem;
} ldiv_t;

typedef struct lldiv
{
        long long quot;
        long long rem;
} lldiv_t;

BEGIN_DECLS

div_t div(int x, int y);
ldiv_t ldiv(long x, long y);
lldiv_t lldiv(long long x, long long y);

void *malloc(size_t size);
void *calloc(size_t num, size_t size);
void *realloc(void *ptr, size_t new_size);
void *aligned_alloc(size_t alignment, size_t size);

void free(void *ptr);

float strtof(const char *str, char **str_end);
double strtod(const char *str, char **str_end);
long double strtold(const char *str, char **str_end);
long strtol(const char *__restrict str, char **__restrict str_end, int base);
long long strtoll(const char *__restrict str, char **__restrict str_end, int base);
unsigned long strtoul(const char *nptr, char **endptr, int base);
unsigned long long strtoull(const char *nptr, char **endptr, int base);

int atoi(const char *str);

void abort(void);

char *getenv(const char *name);

END_DECLS