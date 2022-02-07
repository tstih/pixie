/*
 * exec.h
 *
 * Pixie executive reads commands from the pipeline,
 * parses them, and executes them on the gui.
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2022 tomaz stih
 *
 * 02.02.2022   tstih
 *
 */
#ifndef __EXEC_H__
#define __EXEC_H__

#include "args.h"

extern bool exec_commands();
extern void exec_error(char const *msg, int line, int column);

#endif /* __EXEC_H__ */