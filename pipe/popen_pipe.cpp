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

    string cmd = "ffmpeg -i /Users/hxz/test/input/miomiotv.mp4 -c:v rawvideo -s 320x240 -r 25.0 -f rawvideo - 2>/dev/null";
    FILE * rfp = popen (cmd.c_str(), "r");
    if (!rfp) {
        perror ("popen error");
        exit (1);
    }

    printf ("begin read yuv data\n");

    unsigned char buf[BUF_LEN];
    int len, total = 0;
    FILE * fp = fopen ("bak2.data", "wb");
    while ((len = fread (buf, 1, BUF_LEN, rfp)) > 0) {
        fwrite (buf, len, 1, fp); 
        total += len;
        printf ("read %d, total %d\n", len, total);
    }
    
    pclose (rfp);
    fclose (fp);
    
    return 0;  
}  
