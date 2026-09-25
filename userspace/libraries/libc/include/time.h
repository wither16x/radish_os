#pragma once

#include <stdint.h>
#include "__utils/decls.h"

#define CLOCKS_PER_SEC          1000

typedef int64_t         clock_t;
typedef int64_t         time_t;
typedef uint64_t        suseconds_t;

struct timeval {
        time_t tv_sec;
        suseconds_t tv_usec;
};

struct timezone {
        int tz_minuteswest;
        int tz_dsttime;
};

struct timespec {
        // TODO
};

BEGIN_DECLS

clock_t clock(void);
time_t time(time_t *tloc);
int clock_gettime(int clk_id, struct timespec *tp);
int gettimeofday(struct timeval *__restrict tv, struct timezone *__restrict tz);

END_DECLS