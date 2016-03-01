#include <stdio.h>;
#include <unistd.h>;
#include <stdlib.h>;
#include <fcntl.h>
#include <sys/types.h>;
#include <sys/stat.h>;
#include <string.h>;
#include <strings.h>;


#define TESTSTR "Hello dup2\n"
#define SIZEOFTESTSTR 11
int main () 
{
    int fd3;
    int s_fd;
    int n_fd;
    int r_fd;

    fd3 = open("testdup2.dat",O_WRONLY | O_CREAT| O_TRUNC, 0666);
    printf("after open, fds:%d\n", fd3);
    if (fd3 < 0) {
        printf("open error\n");
        exit(-1);
    }

    /* 复制标准输出描述符 */
    s_fd = dup(STDOUT_FILENO);

    printf("after dup, s_fd:%d\n", s_fd);
    if (s_fd < 0) {
        printf("err in dup\n");
        exit(-1);
    }


    /* 重定向标准输出到文件 */
    n_fd = dup2(fd3,STDOUT_FILENO);

    printf("after dup2, n_fd:%d\n", n_fd);

    if (n_fd < 0) {
        printf("err in dup2\n");
        exit(-1);
    }

    write(STDOUT_FILENO,TESTSTR,SIZEOFTESTSTR); /* 写入testdup2.dat中 */
   
     /* 重定向恢复标准输出 */
    r_fd = dup2(s_fd,n_fd);
    printf("after restore, r_fd:%d\n", r_fd);
    if ( r_fd < 0) {
        printf("err in dup2\n");
    }

    write(STDOUT_FILENO,TESTSTR,SIZEOFTESTSTR); /* 输出到屏幕上 */
   
    return 0;
}
