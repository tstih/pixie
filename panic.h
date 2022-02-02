/*
 * panic.h
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
#ifndef __PANIC_H__
#define __PANIC_H__

#define SUCCESS             0
#define CANT_INIT_GUI       1
#define INVALID_ARGS        2
#define CANT_CREATE_PIPE    3
#define CANT_OPEN_PIPE      4

extern void panic(int errcode, const char *fornat, ...);

#endif /* __PANIC_H__ */