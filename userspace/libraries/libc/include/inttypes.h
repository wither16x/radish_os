#pragma once

#include <stdint.h>
#include "__utils/decls.h"

#define PRIdPTR "ld"
#define PRIxPTR "lx"
#define PRIuPTR "lu"
#define PRIoPTR "lo"
#define PRId64  "lld"
#define PRIx64  "llx"
#define PRIu64  "llu"
#define PRIo64  "llo"
#define PRId32  "d"
#define PRIx32  "x"
#define PRIu32  "u"
#define PRIo32  "o"
#define PRId16  "hd"
#define PRIx16  "hx"
#define PRIu16  "hu"
#define PRId8   "hhd"
#define PRIx8   "hhx"
#define PRIu8   "hhu"
#define PRIdMAX "lld"
#define PRIxMAX "llx"
#define PRIuMAX "llu"

typedef struct imaxdiv {
        intmax_t quot;
        intmax_t rem;
} imaxdiv_t;

BEGIN_DECLS

intmax_t imaxabs(intmax_t j);
imaxdiv_t imaxdiv(intmax_t number, intmax_t demon);

END_DECLS