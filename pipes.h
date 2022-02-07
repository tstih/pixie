/*
 * pipes.h
 *
 * Named pipes handling.
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2022 tomaz stih
 *
 * 02.02.2022   tstih
 *
 */
#ifndef __PIPES_H__
#define __PIPES_H__

#include <stdbool.h>

#include "args.h"

#define MAX_LINE_LEN    0xff

extern void fifo_connect(args_t *args);
extern void fifo_disconnect();
extern int fifo_readline(char *buff, int maxlen);

#endif /* __PIPES_H__ */