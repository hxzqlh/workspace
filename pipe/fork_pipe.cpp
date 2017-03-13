#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <fcntl.h>  
#include <string>

#define BUF_LEN 65536

using namespace std;  

int main(int argc, char *argv[])  
{  
    if (argc < 2) {
        printf ("Usage: %s file\n", argv[0]);
        exit (-1);
    }

    int fd[2];  
    int ret = pipe (fd);
    if (ret < 0) {  
        perror ("pipe create error");  
        exit (1);  
    } 

    /*
    *  fd[0]                          fd[1]
    * parent  <------  pipe  <------  child
    *
    * parent read child process's ffmpeg decoded yuv data
    */
    pid_t pid = fork ();
    if (pid > 0) {         // parent
        close (fd[1]); // close write end
            
        unsigned char buf[BUF_LEN];
        int len, total = 0;
        FILE * fp = fopen ("bak.data", "wb");
        while ((len = read (fd[0], buf, BUF_LEN)) > 0) {
            fwrite (buf, len, 1, fp); 
            total += len;
            printf ("read %d yuv, total %d\n", len, total);
        }
        fclose (fp);

        if (waitpid (pid, NULL, 0) < 0) {
            perror ("waitped error");
            exit (4);
        }
    } else if (pid == 0) { // child
        close (fd[0]); // close read end

        if (fd[1] != STDOUT_FILENO) {
            if (dup2 (fd[1], STDOUT_FILENO) != STDOUT_FILENO) {
                perror ("dup2 error to stdout");
                exit (3);
            }
            close (fd[1]); // don't need this after dup2
        }
        
        string cmd = "ffmpeg -i /Users/hxz/test/input/miomiotv.mp4 -c:v rawvideo -s 320x240 -r 25.0 -f rawvideo - 2>/dev/null";
        /*
        string cmd;
        cmd += "cat ";
        cmd += argv[1];
        */
        system (cmd.c_str ());
        exit (0);
    } else {
        perror ("fork error");
        exit (2);
    }
    
    return 0;  
}  
