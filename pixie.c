#include "panic.h"
#include "args.h"
#include "gui.h"

/* TODO: Collect from cmd line. */


int main(int argc, char *argv[])
{
    gui_init(parse_args(argc, argv));    
    gui_run();
    gui_exit();

    return SUCCESS;
}