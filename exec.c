/*
 * exec.c
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
#include <stdbool.h>
#include <string.h>

#include "pipes.h"

/* Lex and yacc stuff... */
extern void exec_parse(char* str);

/* Execute single command. */
void exec_command(char *cmd) {
    /* Be tolerand towards null terminated strings. */
    if (strlen(cmd)>0)
        exec_parse(cmd);
}

/* Assumptions are that file descriptor is valid
   and gui is initialized. */
bool exec_commands() {
    static int read = 0;
    static char buffer[MAX_LINE_LEN] = "";

    /* Read line from pipe. */
    read=fifo_readline(buffer, 0xfff, read==0);
    /* And process it... */
    if (read > 0) 
        exec_command(buffer);

    /* If it is 0 then we emptied the pipe. 
       But if it is negative then it's a pipe error. */
    return (read >= 0);
}

void exec_error(char const *msg, int line, int column) {
    
}