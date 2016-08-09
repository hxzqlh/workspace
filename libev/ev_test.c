#include <stdio.h>
#include <unistd.h>
#include <ev.h>

void io_action (struct ev_loop * loop, ev_io * ev, int e)
{
    int rst;
    char buf[1024] = {'\0'};
    puts("in io cb\n");
    read(STDIN_FILENO,buf,sizeof(buf));
    buf[1023] = '\0';
    printf("Read in a string:%s \n",buf);
}

int main () 
{
    ev_io io_w;
    
    struct ev_loop * loop = ev_default_loop (0);
    ev_io_init (&io_w, &io_action, STDIN_FILENO, EV_READ);    
    ev_io_start (loop, &io_w);

    ev_run (loop, 0);

    return 0;
}


