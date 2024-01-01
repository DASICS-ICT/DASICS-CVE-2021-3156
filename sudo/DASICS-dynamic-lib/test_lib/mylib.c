#include <my_string.h>
#include <my__string.h>
#include <stdlib.h>
#include <assert.h>
// #pragma GCC push_options
// #pragma GCC optimize("O0")
// malloc memory on heap simplily
static inline uint64_t __BRK(uint64_t ptr)
{
    register long a7 asm("a7") = 214;
    register long a0 asm("a0") = ptr;
    asm volatile("ecall"                        \
                 : "+r"(a0)                     \
                 : "r"(a7)                      \
                 : "memory");

    return a0;
}
#pragma GCC push_options
#pragma GCC optimize("O0")
static void *(*m_be_zero)(void *, uint64_t ) = my__bzero;
uint64_t cross_lib_test()
{
    char buff[20];
    m_be_zero(buff, 20);
    return 0;
}
int my_memcpy(char *dest, const char *src, unsigned int len)
{
    for (; len != 0; len--) {
        *dest++ = *src++;
    }
    
    return 0;
}
#pragma GCC pop_options
void my_memset(void *dest, char val, char len)
{
    char *dst = (char *)dest;

    for (; len != 0; len--) {
        *dst++ = val;
    }
}
// #pragma GCC pop_options

void my_bzero(void *dest, unsigned int len)
{
    my_memset(dest, 0, len);
}

// #pragma GCC push_options
// #pragma GCC optimize("O0")
int my_strlen(const char *src)
{
    int i = 0;
    
    while (src[i] != '\0') {
        i++;
    }
    return i;
}


int my_strcmp(const char *str1, const char *str2)
{
    while (*str1 && *str2) {
        if (*str1 != *str2) {
            return (*str1) - (*str2);
        }
        ++str1;
        ++str2;
    }
    return (*str1) - (*str2);
}

int my_strncmp(const char *str1, const char *str2, int n)
{
    while (*str1 && *str2 && n-- > 0) {
        if (*str1 != *str2) {
            return (*str1) - (*str2);
        }
        ++str1;
        ++str2;
    }
    return (*str1) - (*str2);
}

char *my_strcpy(char *dest, const char *src)
{
    char *tmp = dest;

    while (*src) {
        *dest++ = *src++;
    }

    *dest = '\0';

    return tmp;
}

char *my_strncpy(char *dest, const char *src, int n)
{
    char *tmp = dest;

    while (*src && n-- > 0) {
        *dest++ = *src++;
    }

    while (n-- > 0) {
        *dest++ = '\0';
    }

    return tmp;
}

char *my_strcat(char *dest, const char *src)
{
    char *tmp = dest;

    while (*dest != '\0') {
        dest++;
    }
    while (*src) {
        *dest++ = *src++;
    }

    *dest = '\0';

    return tmp;
}

int my_sleep(uint64_t time)
{
    sleep(time);
}

int my_getpid()
{
    return getpid();
}

static void *(*m_rand)() = rand;
uint64_t my_malloc(uint64_t size)
{

    return malloc(size);
}

uint64_t lib_call_rand()
{
    return m_rand();
}

int my_free(uint64_t addr)
{
    free(addr);
}

void* my_realloc(void * addr, uint64_t size)
{
    return (void *)realloc(addr, size);
}


int my_read()
{
    FILE *test = fopen("cert.crt", "r");
    fseek(test, 0, SEEK_SET);
    char *buff = malloc(256);
    fseek(test, 0, SEEK_SET);
    fread(buff, 1, 256, test);
    for (int i = 0; i < 256; i++)
    {
        printf("%c", buff[i]);
    }
    printf("\n");
    free(buff);
    fclose(test);
}

// #pragma GCC pop_options