#ifndef INCLUDE__STRING_H_
#define INCLUDE__STRING_H_

void my__memcpy(char *dest, const char *src, unsigned int len);
void my__memset(void *dest, char val, char len);
void my__bzero(void *dest, unsigned int len);
int my__strcmp(const char *str1, const char *str2);
int my__strncmp(const char *str1, const char *str2, int n);
char *my__strcpy(char *dest, const char *src);
char *my__strncpy(char *dest, const char *src, int n);
char *my__strcat(char *dest, const char *src);
int my__strlen(const char *src);

#endif