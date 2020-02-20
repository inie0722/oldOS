#include <stddef.h>
#include <stdint.h>

extern void *memchr(const void *str, int c, size_t n);

extern int memcmp(const void *str1, const void *str2, size_t n);

extern void *memcpy(void *dest, const void *src, size_t n);

extern void *memmove(void *dest, const void *src, size_t n);

extern void *memset(void *str, int c, size_t n);

extern char *strcat(char *dest, const char *src);

extern char *strchr(const char *str, int c);

extern int strcmp(const char *str1, const char *str2);

extern char *strcpy(char *dest, const char *src);