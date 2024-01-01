#include <my__string.h>
#include <my_string.h>
// #pragma GCC push_options
// #pragma GCC optimize("O0")
void my__memcpy(char *dest, const char *src, unsigned int len)
{
    my_memcpy(dest, src, len);
}

void my__memset(void *dest, char val, char len)
{
    my_memset(dest, val, len);
}

void my__bzero(void *dest, unsigned int len)
{
    my__memset(dest, 0, len);
}

int my__strlen(const char *src)
{
    return my_strlen(src); 
}

int my__strcmp(const char *str1, const char *str2)
{
    return my_strcmp(str1, str2);
}

int my__strncmp(const char *str1, const char *str2, int n)
{
    return my_strncmp(str1, str2, n);
}

char *my__strcpy(char *dest, const char *src)
{
    return my_strcpy(dest, src);
}

char *my__strncpy(char *dest, const char *src, int n)
{
    return my_strncpy(dest, src, n);
}

char *my__strcat(char *dest, const char *src)
{
    return my_strcat(dest, src);
}
// #pragma GCC pop_options
