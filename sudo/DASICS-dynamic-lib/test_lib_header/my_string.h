#ifndef INCLUDE_STRING_H_
#define INCLUDE_STRING_H_
#include <stdint.h>
#include <stdio.h>

int my_memcpy(char *dest, const char *src, unsigned int len);
void my_memset(void *dest, char val, char len);
void my_bzero(void *dest, unsigned int len);
int my_strcmp(const char *str1, const char *str2);
int my_strncmp(const char *str1, const char *str2, int n);
char *my_strcpy(char *dest, const char *src);
char *my_strncpy(char *dest, const char *src, int n);
char *my_strcat(char *dest, const char *src);
int my_strlen(const char *src);
int my_getpid();
int my_sleep(uint64_t time);
uint64_t my_malloc(uint64_t size);
int my_free(uint64_t addr);
void *my_realloc(void * addr, uint64_t size);
int my_read();
uint64_t lib_call_rand();
uint64_t cross_lib_test();

#endif