#pragma once

#include "__utils/decls.h"

BEGIN_DECLS

int isspace(int ch);
int isdigit(int ch);
int isalpha(int ch);
int isalnum(int ch);
int isupper(int ch);
int islower(int ch);
int isxdigit(int ch);
int iscntrl(int ch);
int isprint(int ch);
int isgraph(int ch);
int ispunct(int ch);
int isblank(int ch);
int tolower(int ch);
int toupper(int ch);

END_DECLS