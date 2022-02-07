/*
 * pipes.c
 *
 * Named pipes handling.
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2022 tomaz stih
 *
 * 02.02.2022   tstih
 *
 */
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdbool.h>
#include <fcntl.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>

#include "panic.h"
#include "args.h"

static int _fd = 0;

void fifo_connect(args_t *args) {
    /* Create fifo. Be tolerant if it already exists... */
    if (mkfifo(args->pname,0777)==-1 && errno!=EEXIST)
        panic(CANT_CREATE_PIPE, "Unable to create fifo %s\n", args->pname);
    /* Now open it for reading only. */
    if ((_fd = open(args->pname,O_RDONLY))<=0)
        panic(CANT_OPEN_PIPE, "Unable to open fifo %s\n", args->pname);
}

void fifo_disconnect() {
    if (_fd) close(_fd);
}

/* Return value is: 
   = buflen + 1 ... buffer overflow
   > 0 ... bytes read
   = 0 ... no error, process...
   < 0 ... pipe error
*/
int fifo_readline(char *buff, int maxlen, bool append) {
    int bytesread, i = append?strlen(buff):0;
    char ch = 0;
    while(1)
    {
        /* Check for buffer overflow. */
        if (i==maxlen) { buff[maxlen-1]=0; return maxlen+1; };
        /* Read one byte. */
        bytesread = read( _fd, &ch, 1);
        if(bytesread > 0) {
            switch(ch) {
                /* Newline? */
                case '\n':
                case '\r':
                    buff[i]=0;
                    return i+1;
                /* Ordinary letter. */
                default:
                    buff[i++]=ch;
            }
        } else if (bytesread==0 && errno == EAGAIN) { /* Try again. */
            buff[i]=0;
            return 0;   
        }
        else return bytesread; /* Pipe is no more? Return negative. */
    }
}