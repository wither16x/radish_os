#include "ctype.h"
#include "__utils/decls.h"

BEGIN_DECLS

int isspace(int ch)
{
        return ch == ' ' || ch == '\f' || ch == '\n' || ch == '\r' || ch == '\t' || ch == '\v';
}

int isdigit(int ch)
{
        return ch >= '0' && ch <= '9';
}

int isalpha(int ch)
{
        return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
}

int isalnum(int ch)
{
        return isalpha(ch) || isdigit(ch);
}

int isupper(int ch)
{
        return ch >= 'A' && ch <= 'Z';
}

int islower(int ch)
{
        return ch >= 'a' && ch <= 'z';
}

int isxdigit(int ch)
{
        return isdigit(ch) || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F');
}

int iscntrl(int ch)
{
        return (ch >= 0 && ch < 0x20) || ch == 0x7f;
}

int isprint(int ch)
{
        return ch >= 0x20 && ch < 0x7f;
}

int isgraph(int ch)
{
        return isprint(ch) && ch != ' ';
}

int ispunct(int ch)
{
        return isprint(ch) && !isalnum(ch) && ch != ' ';
}

int isblank(int ch)
{
        return ch == ' ' || ch == '\t';
}

int tolower(int ch)
{
        return isupper(ch) ? ch + ('a' - 'A') : ch;
}

int toupper(int ch)
{
        return islower(ch) ? ch - ('a' - 'A') : ch;
}

END_DECLS