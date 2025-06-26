#include <stddef.h>
#ifndef TOOLS_H
#define TOOLS_H

// Safer string copy, ensure null terminated string.
void strcpyS(char *__restrict__ __dest, const char *__restrict__ __src, size_t __n);

#endif