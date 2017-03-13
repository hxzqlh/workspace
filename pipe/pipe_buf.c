#include    <stdio.h>  
#include    <stdlib.h>  
#include    <unistd.h>  
#include    <sys/types.h>  
#include    <fcntl.h>  
int main(int argc, char *argv[])  
{  
    int pipe_fd[2];  
    int len=0;  
    //pit_t pid;  
    if( pipe(pipe_fd)<0 )  
    {  
        perror("pipe create error");  
        exit(1);  
    }  
    //write可能会被阻塞，用fcntl设置为非阻塞  
    fcntl(pipe_fd[1], F_SETFL, O_NONBLOCK );  
    while( (1==write(pipe_fd[1], "a", 1)) )  
    {  
        len++;    
    }  
    printf("len=%d\n", len);  
    close(pipe_fd[0]);  
    close(pipe_fd[1]);  
    return 0;  
  
}  
