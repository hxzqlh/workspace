/**
 *    简单展示原理，没有错误处理
 *    简单展示原理，能用即可
 *    这是所谓的“第一个系统”
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int fork_and_exec(char *cmd, int pin, int pout)
{
    int pid = fork();
    if (pid == 0) {
        if (pin != -1) {
            dup2 (pin, 0);
            close(pin);
        }
        if (pout != -1) {
            dup2 (pout, 1);
            close(pout);
        }
        //exec太麻烦，索性用system了
        //但是必须知道system的原理(fork+exec...)
        //if (execlp(cmd, NULL) == -1) {
        system(cmd); //若是exec且执行成功，就不需要exit了
        exit(0);
    } else if(pid > 0) {
        if (pin != -1)
            close(pin);
        if (pout != -1)
            close(pout);
    } else {
        //TODO
    }
    return pid;
}

int execute_cmd(char *cmd, int in)
{
    char *p = cmd;
    char *start_cmd = cmd;
    int pipefd[2];
    while (*p) {
        switch (*p) {
            case '|':
                *p++ = 0;
                //创建一个管道
                pipe(pipefd);
                //下面的语句执行后，程序分叉，第一叉在当前捕获的命令，第二
                //叉在后面继续解析命令
                //将写管道传给当前捕获的命令用于重定向其stdout
                if (fork_and_exec(start_cmd, in, pipefd[1]) > 0) {
                    //将读管道传给将要被捕获的命令用于重定向其stdin
                    goto call_forward_pipe_chain;
                }
                break;
            default:
                p++;
        }
    }
    fork_and_exec(start_cmd, in, -1);
    fflush(stdout);
    return 0;
call_forward_pipe_chain:
    execute_cmd(p, pipefd[0]);
    fflush(stdout);
    return 0;
}

int main(int argc, char **argv)
{
    while (1) {
        char cmd[1024]={0};
        int len;
        printf("cmd>>");
        fflush(stdout);
        gets(cmd);
        len = strlen(cmd);
        if (!strcmp(cmd, "q")) {
            fflush(stdout);
            printf("done\n");
            exit(0);
        } else {
            execute_cmd(cmd, -1);
        }
    }
    return 0;
}
