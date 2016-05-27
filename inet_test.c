#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
//#include <arpa/inet.h>

int main(int argc,char *argv[])
{
    struct sockaddr_in cli_addr;
    char *some_addr;

    inet_aton("127.0.0.1", &cli_addr.sin_addr);

    some_addr = inet_ntoa(cli_addr.sin_addr); 
    printf("%s\n", some_addr); // prints "127.0.0.1"

    return 0;
}
