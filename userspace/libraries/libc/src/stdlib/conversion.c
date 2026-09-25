#include "__utils/decls.h"
#include "stdlib.h"
#include "ctype.h"
#include "math.h"
#include "limits.h"
#include "errno.h"
#include <limits.h>

BEGIN_DECLS

/* ------------------------------------------------------------------------------------------------- */
double strtod(const char *str, char **str_end)
{
        const char *p = str;
        while (isspace((unsigned char)*p))
                p++;

        int sign = 1;
        if (*p == '+' || *p == '-') {
                if (*p == '-')
                        sign = -1;
                p++;
        }

        double mantissa = 0.0;
        int any_digits = 0;

        while (isdigit((unsigned char)*p)) {
                mantissa = mantissa * 10.0 + (*p - '0');
                p++;
                any_digits = 1;
        }

        if (*p == '.') {
                p++;
                double frac = 0.1;
                while (isdigit((unsigned char)*p)) {
                        mantissa += (*p - '0') * frac;
                        frac *= 0.1;
                        p++;
                        any_digits = 1;
                }
        }

        if (!any_digits) {
                if (str_end)
                        *str_end = (char *)str;
                return 0.0;
        }

        int exponent = 0;
        if (*p == 'e' || *p == 'E') {
                const char *exp_start = p;
                p++;

                int exp_sign = 1;
                if (*p == '+' || *p == '-') {
                        if (*p == '-')
                                exp_sign = -1;
                        p++;
                }
                if (isdigit((unsigned char)*p)) {
                        int exp_val = 0;
                        while (isdigit((unsigned char)*p)) {
                                exp_val = exp_val * 10 + (*p - '0');
                                p++;
                        }
                        exponent = exp_sign * exp_val;
                } else {
                        p = exp_start;
                }
        }

        double result = sign * mantissa;
        if (exponent != 0)
                result *= pow(10.0, exponent);

        if (str_end)
                *str_end = (char *)p;

        return result;
}
/* ------------------------------------------------------------------------------------------------- */

long double strtold(const char *str, char **str_end)
{
        const char *p = str;
        while (isspace((unsigned char)*p))
                p++;

        int sign = 1;
        if (*p == '+' || *p == '-') {
                if (*p == '-')
                        sign = -1;
                p++;
        }

        long double mantissa = 0.0;
        int any_digits = 0;

        while (isdigit((unsigned char)*p)) {
                mantissa = mantissa * 10.0 + (*p - '0');
                p++;
                any_digits = 1;
        }

        if (*p == '.') {
                p++;
                long double frac = 0.1;
                while (isdigit((unsigned char)*p)) {
                        mantissa += (*p - '0') * frac;
                        frac *= 0.1;
                        p++;
                        any_digits = 1;
                }
        }

        if (!any_digits) {
                if (str_end)
                        *str_end = (char *)str;
                return 0.0;
        }

        int exponent = 0;
        if (*p == 'e' || *p == 'E') {
                const char *exp_start = p;
                p++;

                int exp_sign = 1;
                if (*p == '+' || *p == '-') {
                        if (*p == '-')
                                exp_sign = -1;
                        p++;
                }
                if (isdigit((unsigned char)*p)) {
                        int exp_val = 0;
                        while (isdigit((unsigned char)*p)) {
                                exp_val = exp_val * 10 + (*p - '0');
                                p++;
                        }
                        exponent = exp_sign * exp_val;
                } else {
                        p = exp_start;
                }
        }

        long double result = sign * mantissa;
        if (exponent != 0)
                result *= pow(10.0, exponent);

        if (str_end)
                *str_end = (char *)p;

        return result;
}

float strtof(const char *str, char **str_end)
{
        const char *p = str;
        while (isspace((unsigned char)*p))
                p++;

        int sign = 1;
        if (*p == '+' || *p == '-') {
                if (*p == '-')
                        sign = -1;
                p++;
        }

        float mantissa = 0.0;
        int any_digits = 0;

        while (isdigit((unsigned char)*p)) {
                mantissa = mantissa * 10.0 + (*p - '0');
                p++;
                any_digits = 1;
        }

        if (*p == '.') {
                p++;
                float frac = 0.1;
                while (isdigit((unsigned char)*p)) {
                        mantissa += (*p - '0') * frac;
                        frac *= 0.1;
                        p++;
                        any_digits = 1;
                }
        }

        if (!any_digits) {
                if (str_end)
                        *str_end = (char *)str;
                return 0.0;
        }

        int exponent = 0;
        if (*p == 'e' || *p == 'E') {
                const char *exp_start = p;
                p++;

                int exp_sign = 1;
                if (*p == '+' || *p == '-') {
                        if (*p == '-')
                                exp_sign = -1;
                        p++;
                }
                if (isdigit((unsigned char)*p)) {
                        int exp_val = 0;
                        while (isdigit((unsigned char)*p)) {
                                exp_val = exp_val * 10 + (*p - '0');
                                p++;
                        }
                        exponent = exp_sign * exp_val;
                } else {
                        p = exp_start;
                }
        }

        float result = sign * mantissa;
        if (exponent != 0)
                result *= pow(10.0, exponent);

        if (str_end)
                *str_end = (char *)p;

        return result;
}

long strtol(const char *restrict str, char **restrict str_end, int base)
{
        const char *p = str, *endp;
        bool is_neg = 0, overflow = 0;
        unsigned long n = 0ul, cutoff;
        int cutlim;
        if (base < 0 || base == 1 || base > 36)
                return 0l;

        endp = str;
        while (isspace(*p))
                p++;
        if (*p == '+') {
                p++;
        } else if (*p == '-') {
                is_neg = 1, p++;
        }
        if (*p == '0') {
                p++;
                endp = p;
                if (base == 16 && (*p == 'X' || *p == 'x')) {
                        p++;
                } else if (base == 2 && (*p == 'B' || *p == 'b')) {
                        p++;
                } else if (base == 8 && (*p == 'O' || *p == 'o')) {
                        p++;
                } else if (base == 0) {
                        if (*p == 'X' || *p == 'x') {
                                base = 16, p++;
                        } else if (*p == 'B' || *p == 'b') {
                                base = 2, p++;
                        } else {
                                base = 8;
                                if (*p == 'O' || *p == 'o')
                                        p++;
                        }
                }
        } else if (base == 0) {
                base = 10;
        }
        cutoff = (is_neg) ? -(LONG_MIN / base) : LONG_MAX / base;
        cutlim = (is_neg) ? -(LONG_MIN % base) : LONG_MAX % base;
        while (1) {
                int c;
                if (*p >= 'A')
                        c = ((*p - 'A') & (~('a' ^ 'A'))) + 10;
                else if (*p <= '9')
                        c = *p - '0';
                else
                        break;
                if (c < 0 || c >= base)
                        break;
                endp = ++p;
                if (overflow) {
                        if (str_end)
                                continue;
                        break;
                }
                if (n > cutoff || (n == cutoff && c > cutlim)) {
                        overflow = 1;
                        continue;
                }
                n = n * base + c;
        }
        if (str_end)
                *str_end = (char *)endp;
        if (overflow) {
                errno = ERANGE;
                return ((is_neg) ? LONG_MIN : LONG_MAX);
        }
        return (long)((is_neg) ? -n : n);
}

long long strtoll(const char *restrict str, char **restrict str_end, int base)
{
        const char *p = str, *endp;
        bool is_neg = 0, overflow = 0;
        unsigned long long n = 0ul, cutoff;
        int cutlim;
        if (base < 0 || base == 1 || base > 36)
                return 0l;

        endp = str;
        while (isspace(*p))
                p++;
        if (*p == '+') {
                p++;
        } else if (*p == '-') {
                is_neg = 1, p++;
        }
        if (*p == '0') {
                p++;
                endp = p;
                if (base == 16 && (*p == 'X' || *p == 'x')) {
                        p++;
                } else if (base == 2 && (*p == 'B' || *p == 'b')) {
                        p++;
                } else if (base == 8 && (*p == 'O' || *p == 'o')) {
                        p++;
                } else if (base == 0) {
                        if (*p == 'X' || *p == 'x') {
                                base = 16, p++;
                        } else if (*p == 'B' || *p == 'b') {
                                base = 2, p++;
                        } else {
                                base = 8;
                                if (*p == 'O' || *p == 'o')
                                        p++;
                        }
                }
        } else if (base == 0) {
                base = 10;
        }
        cutoff = (is_neg) ? -(LONG_MIN / base) : LONG_MAX / base;
        cutlim = (is_neg) ? -(LONG_MIN % base) : LONG_MAX % base;
        while (1) {
                int c;
                if (*p >= 'A')
                        c = ((*p - 'A') & (~('a' ^ 'A'))) + 10;
                else if (*p <= '9')
                        c = *p - '0';
                else
                        break;
                if (c < 0 || c >= base)
                        break;
                endp = ++p;
                if (overflow) {
                        if (str_end)
                                continue;
                        break;
                }
                if (n > cutoff || (n == cutoff && c > cutlim)) {
                        overflow = 1;
                        continue;
                }
                n = n * base + c;
        }
        if (str_end)
                *str_end = (char *)endp;
        if (overflow) {
                errno = ERANGE;
                return ((is_neg) ? LONG_MIN : LONG_MAX);
        }
        return (long long)((is_neg) ? -n : n);
}

unsigned long strtoul(const char *nptr, char **endptr, int base)
{
	const char *s = nptr;
	unsigned long acc;
	int c;
	unsigned long cutoff;
	int neg = 0, any, cutlim;

	do {
		c = *s++;
	} while (isspace(c));
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else if (c == '+')
		c = *s++;
	if ((base == 0 || base == 16) &&
	    c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;
	cutoff = (unsigned long)ULONG_MAX / (unsigned long)base;
	cutlim = (unsigned long)ULONG_MAX % (unsigned long)base;
	for (acc = 0, any = 0;; c = *s++) {
		if (isdigit(c))
			c -= '0';
		else if (isalpha(c))
			c -= isupper(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
			any = -1;
		else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0) {
		acc = ULONG_MAX;
		errno = ERANGE;
	} else if (neg)
		acc = -acc;
	if (endptr != 0)
		*endptr = (char *) (any ? s - 1 : nptr);
	return (acc);
}

unsigned long long strtoull(const char *nptr, char **endptr, int base)
{
	const char *s = nptr;
	unsigned long long acc;
	int c;
	unsigned long long cutoff;
	int neg = 0, any, cutlim;

	do {
		c = *s++;
	} while (isspace(c));
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else if (c == '+')
		c = *s++;
	if ((base == 0 || base == 16) &&
	    c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;
	cutoff = (unsigned long long)ULLONG_MAX / (unsigned long long)base;
	cutlim = (unsigned long long)ULLONG_MAX % (unsigned long long)base;
	for (acc = 0, any = 0;; c = *s++) {
		if (isdigit(c))
			c -= '0';
		else if (isalpha(c))
			c -= isupper(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
			any = -1;
		else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0) {
		acc = ULONG_MAX;
		errno = ERANGE;
	} else if (neg)
		acc = -acc;
	if (endptr != 0)
		*endptr = (char *) (any ? s - 1 : nptr);
	return (acc);
}

int atoi(const char *str)
{
        int sign = 1;
        int base = 0;
        int i = 0;

        while (str[i] == ' ')
                ++i;

        if (str[i] == '-' || str[i] == '+')
                sign = 1 - 2 * (str[i++] == '-');

        while (str[i] >= '0' && str[i] <= '9') {
                if (base > INT_MAX / 10 || (base == INT_MAX / 10 && str[i] - '0' > 7)) {
                        if (sign == 1)
                                return INT_MAX;
                        else
                                return INT_MIN;
                }

                base = 10 * base + (str[i++] - '0');
        }

        return base * sign;
}

END_DECLS