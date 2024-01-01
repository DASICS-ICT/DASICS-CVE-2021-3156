#ifndef INCLUDE_MY_STDIO_H_
#define INCLUDE_MY_STDIO_H_

#include "my_stdarg.h"

int my_printf(const char *fmt, ...);
int my_vprintf(const char *fmt, va_list va);

#endif
