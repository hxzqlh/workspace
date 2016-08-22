#include <stdio.h>
#include <unistd.h>
#include <ev.h>

void io_action (struct ev_loop * loop, ev_io * e_io, int e)
{
    int rst;
    char buf[1024] = {'\0'};
    puts("in io cb\n");
    read (STDIN_FILENO,buf,sizeof(buf));
    buf[1023] = '\0';
    printf("Read in a string:%s \n",buf);

    ev_io_stop (loop, e_io);
}

void timer_action (struct ev_loop *loop,ev_timer *timer_w,int e)
{
    puts("in timer cb \n");

    printf ("timer alive?%d\n", ev_is_active (timer_w));

    //ev_timer_stop (loop,timer_w);
}

void signal_action (struct ev_loop *loop,ev_signal *signal_w,int e)
{
    puts("in signal cb \n");

    ev_signal_stop(loop,signal_w);
    ev_break (loop,EVBREAK_ALL);
}

int main () 
{
    ev_io io_w;
    ev_timer timer_w;
    ev_signal signal_w;
    
    struct ev_loop * loop = ev_default_loop (0);

    //ev_io_init (&io_w, io_action, STDIN_FILENO, EV_READ);    
    //ev_io_start (loop, &io_w);

    ev_timer_init (&timer_w, timer_action, 3, 0);
    ev_timer_start (loop, &timer_w);

    ev_signal_init (&signal_w, signal_action, SIGINT);
    ev_signal_start (loop, &signal_w);

    ev_run (loop, 0);

    return 0;
}


