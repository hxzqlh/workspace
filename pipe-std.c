#include <stdio.h>

int main()
{
    FILE * fi = stdin;
    FILE * fo = stdout;
    char buf[1024];
    int r_cnt, w_cnt;

    while ((r_cnt = fread (buf, 1, 1024, fi)) > 0) {
        w_cnt = fwrite (buf, 1, r_cnt, fo);
        if (w_cnt != r_cnt) {
            fprintf (stderr, "some err: %d, %d\n", r_cnt, w_cnt);
            return -1;
        }
    }

    return 0;
}
