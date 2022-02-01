/*
 * panic.c
 *
 * panic() function displays error and 
 * exits application.
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2022 tomaz stih
 *
 * 31.01.2022   tstih
 *
 */
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