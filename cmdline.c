#include "panic.h"
#include "cmdline.h"

#define DEFAULT_SCREEN_WIDTH    1024
#define DEFAULT_SCREEN_HEIGHT   512

static args_t _args;

/* TODO:https://www.geeksforgeeks.org/getopt-function-in-c-to-parse-command-line-arguments/ */
args_t *parse_args(int argc, char *argv[]) {

    return &_args;
}