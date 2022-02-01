#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

void panic(int errcode, const char *format, ...) {

    /* Pass to stderr. */   
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);

    /* And exit. */
    exit(errcode);
}