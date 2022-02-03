#include <stdio.h>

#include "panic.h"
#include "args.h"
#include "pipes.h"
#include "gui.h"
#include "exec.h"

int main(int argc, char *argv[])
{
    /* First parse args. */
    args_t *args=parse_args(argc, argv);

    /* Now establish a named pipe connection. */
    printf("pixie is listening to %s...\n", args->pname);
    fifo_connect(args);
    printf("ready?\n");

    /* And enter gui... */
    gui_init(args);    
    gui_run(exec_commands);
    gui_exit();

    fifo_disconnect();

    return SUCCESS;
}