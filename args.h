/*
 * args.h
 *
 * Command line arguments parsing.
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2022 tomaz stih
 *
 * 01.02.2022   tstih
 *
 */
#ifndef __ARGS_H__
#define __ARGS_H__

typedef struct args_s {
    int width;
    int height;
    char *pname;
    char *title;
} args_t;

extern args_t *parse_args(int argc, char *argv[]);

#endif /* __ARGS_H__ */