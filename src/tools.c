#include <string.h>
void strcpyS(char *__restrict__ __dest, const char *__restrict__ __src, size_t __n) {
    strncpy(__dest,__src,__n); // Regular strncpy (I love memory safe code)
    __dest[__n-1] = '\0'; // Ensure null terminated
}