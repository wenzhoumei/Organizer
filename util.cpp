#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdarg.h>

void die(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    fputc('\n', stderr);

    exit(1);
}
