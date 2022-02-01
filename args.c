/*
 * args.c
 *
 * Command line arguments parsing.
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2022 tomaz stih
 *
 * 01.02.2022   tstih
 *
 */
#include <getopt.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "panic.h"
#include "args.h"

#define DEFAULT_SCREEN_WIDTH    1024
#define DEFAULT_SCREEN_HEIGHT   512
#define DEFAULT_PIPE_NAME       "/tmp/pixie.pipe"
#define DEFAULT_TITLE           "pixie"

/* Set defaults. */
static struct args_s _args =  { 
    DEFAULT_SCREEN_WIDTH, 
    DEFAULT_SCREEN_HEIGHT, 
    DEFAULT_PIPE_NAME,
    DEFAULT_TITLE
};

args_t *parse_args(int argc, char *argv[]) {

    int ch;
    int i=0;

    /* Get width and height. */
    while (true) {
        static struct option opts[] = {
            { "width",   required_argument, 0,  'w' },
            { "height",  required_argument, 0,  'h' },
            { "title",   required_argument, 0,  't' },
            { 0, 0, 0, 0 } /* Last row. */
        };
        if ((ch = getopt_long(argc,argv,"h:t:w:",opts,&i)) == -1)
            break;
        switch(ch) {
            case 'w':
                _args.width=atoi(optarg);
                break;
            case 'h':
                _args.height=atoi(optarg);
                break;
            case 't':
                _args.title=optarg;
                break;
            default:
                panic(INVALID_ARGS, 
                    "Usage: pixie -w <width> -h <height> <named pipe>\n");
        }
    }

    /* Get named pipe file, ignore other args. */
    if (optind < argc)
        _args.pname=argv[optind];
        
    return &_args;
}