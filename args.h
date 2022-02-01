typedef struct args_s {
    int width;
    int height;
    char *pname;
    char *title;
} args_t;

extern args_t *parse_args(int argc, char *argv[]);