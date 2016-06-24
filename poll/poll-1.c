#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <poll.h>

int main()
{
    char buf[1024];
    struct pollfd clifds[1];
    int nready = 0;

    clifds[0].fd     = STDIN_FILENO;
    clifds[0].events = POLLIN;

    for(;;)
    {
        nready = poll (clifds, 1, 2000);
        if (nready == -1) {
            printf ("poll erroro, errno: %d\n", errno);
            exit (-1);
        }

        if (nready == 0) {
            printf ("No data available\n");
        }
        else if (clifds[0].revents & POLLIN) {
            read (clifds[0].fd, buf, 1024);
            printf ("Read buf is: %s\n", buf);
        }
    }
    exit(0);
}
