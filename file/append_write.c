#include <stdio.h>;
#include <unistd.h>;
#include <stdlib.h>;
#include <fcntl.h>
#include <sys/types.h>;
#include <sys/stat.h>;
#include <string.h>;
#include <strings.h>;

#define TESTSTR "Hello dup2\n"

int main () 
{
    int fd3;
    int s_fd;
    int n_fd;
    int r_fd;
    int off_set;

    char buf[] = "abcdefghij";

    fd3 = open ("testdup2.dat", O_WRONLY | O_APPEND, 0666);
 
    printf ("after open, fds:%d\n", fd3);
    if (fd3 < 0) {
        printf ("open error\n");
        exit (-1);
    }

    off_set = lseek (fd3, 0, SEEK_CUR);
    printf ("cur off_set:%d\n", off_set);
    
    off_set = lseek (fd3, 10, SEEK_SET);
    printf ("lseek1:%d\n", off_set);

    if (write (fd3, buf, 10) != 10) {
        printf ("write1 error\n");
    }

    off_set = lseek (fd3, 18, SEEK_SET);
    printf ("lseek2:%d\n", off_set);

    if (write (fd3, buf, 10) != 10) {
        printf ("write2 error\n");
    }
 
    return 0;
}
