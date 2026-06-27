#pragma once

#include <stdint.h>

struct symbol {
        uintptr_t addr;
        const char *name;
};