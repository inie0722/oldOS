#include <stddef.h>
#include <stdint.h>

void *memchr(const void *str, int c, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        if (((char *)str)[i] == (char)c)
        {
            return &((char *)str)[i];
        }
    }
    return NULL;
}

int memcmp(const void *str1, const void *str2, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        if (((char *)str1)[i] < ((char *)str2)[i])
        {
            return -1;
        }
        else if (((char *)str1)[i] > ((char *)str2)[i])
        {
            return 1;
        }
    }

    return 0;
}

void *memcpy(void *dest, const void *src, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        ((char *)dest)[i] = ((char *)src)[i];
    }
    return dest;
}

void *memmove(void *dest, const void *src, size_t n)
{
    for (size_t i = n; i > 0; i--)
    {
        ((char *)dest)[i - 1] = ((char *)src)[i - 1];
    }
    return dest;
}

__attribute__((optimize("O0"))) void *memset(void *str, int c, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        ((char *)str)[i] = (char)c;
    }
    return str;
}

char *strcat(char *dest, const char *src)
{
    while (*dest)
    {
        ++dest;
    }

    while (*src)
    {
        *dest++ = *src++;
    }
    return dest;
}

const char *strchr(const char *str, int c)
{
    while (*str && *str != (char)c)
    {
        ++str;
    }

    if (*str)
    {
        return str;
    }
    else
    {
        return NULL;
    }
}

int strcmp(const char *str1, const char *str2)
{
    while (*str1 && *str2 && *str1 == *str2)
    {
        ++str1;
        ++str2;
    }

    if (*str1 < *str2)
    {
        return -1;
    }
    else if (*str1 > *str2)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

char *strcpy(char *dest, const char *src)
{
    char *cp = dest;
    while ((*cp++ = *src++))
        ;

    return dest;
}