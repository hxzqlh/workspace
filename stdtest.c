#include <stdio.h>

int main()
{
    // 將輸出、輸入、標準錯誤都寫到指定檔案
    //freopen("./stdin.log", "w", stdin);
    //freopen("./stdout.log", "w", stdout);
    //freopen("./stderr.log", "w", stderr);

    fprintf(stdin, "This is stdin\n"); // 等同 scanf("...");
    fprintf(stdout, "This is stdout\n"); // 等同 printf("...");
    fprintf(stderr, "This is stderr\n"); // 會印在螢幕上, 但是不會進入 輸出導向 裡面.

    return 0;
}
