#include "panic.h"
#include "cmdline.h"
#include "gui.h"

/* TODO: Collect from cmd line. */


int main(int argc, char *argv[])
{
    args_t *args=parse_args(argc, argv);

    gui_init(args->width, args->height);    
    gui_run();
    gui_exit();

    return SUCCESS;
}