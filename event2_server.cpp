#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <errno.h>  

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
  
#include <event2/event.h>  
#include <event2/bufferevent.h>  

#define SERVER_ADDR    "127.0.0.1"  
#define LISTEN_PORT    8787 
#define LISTEN_BACKLOG 2  
#define MAX_LINE       256
  
void do_accept_cb(evutil_socket_t listener, short event, void *arg);  
void read_cb(struct bufferevent *bev, void *arg);  
void error_cb(struct bufferevent *bev, short event, void *arg);  
void write_cb(struct bufferevent *bev, void *arg);  

int main () 
{
    evutil_socket_t listener;
    if ((listener = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf (stderr, "socket error\n");
        return -1;
    }
    
    evutil_make_listen_socket_reuseable (listener);
    
    struct sockaddr_in sin;
    bzero (&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr=inet_addr(SERVER_ADDR); 
    sin.sin_port = htons (LISTEN_PORT);

    if (bind (listener, (struct sockaddr*) &sin, sizeof (sin)) < 0) {
        fprintf (stderr, "bind error\n");
        return -1;
    }

    if (listen (listener, LISTEN_BACKLOG) < 0) {
        fprintf (stderr, "listen error\n");
        return -1;
    }

    printf ("Listening...\n");

    evutil_make_socket_nonblocking (listener);

    struct event_base * base = event_base_new ();
    if (!base) {
        fprintf (stderr, "new base error\n");
        return -1;
    }

    struct event * listen_event;
    listen_event = event_new (base, listener, EV_READ | EV_PERSIST, do_accept_cb, (void*) base);
    event_add (listen_event, NULL);
    event_base_dispatch (base);

    printf ("The end\n");

    return 0;
}

void do_accept_cb (evutil_socket_t listener, short event, void * arg)
{
    struct event_base * base = (struct event_base *) arg;
    evutil_socket_t fd;
    struct sockaddr_in cli_addr;
    socklen_t slen = sizeof (cli_addr);
    
    fd = accept (listener, (struct sockaddr *)&cli_addr, &slen);
    if (fd < 0) {
        fprintf (stderr, "accept error\n");
        return;
    }

    printf ("accept: fd = %u\n", fd);

    struct bufferevent * bev = bufferevent_socket_new (base, fd, BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb (bev, read_cb, NULL, error_cb, arg);
    bufferevent_enable (bev, EV_READ | EV_WRITE | EV_PERSIST);
}

void read_cb(struct bufferevent *bev, void *arg)
{
    char line[MAX_LINE + 1];
    int n;
    evutil_socket_t fd = bufferevent_getfd (bev);

    while ((n = bufferevent_read (bev, line, MAX_LINE)) > 0) {
        line[n] = '\0';
        printf ("fd=%d, read line:%s\n", fd, line);
        bufferevent_write (bev, line, n);
    }
} 

void error_cb(struct bufferevent *bev, short event, void *arg)
{
    evutil_socket_t fd = bufferevent_getfd (bev);
    printf ("fd=%d", fd);

    if (event & BEV_EVENT_TIMEOUT) {
        fprintf (stderr, "Time out\n");
    }
    else if (event & BEV_EVENT_EOF) {
        fprintf (stderr, "read eor, closed\n");
    }
    else if (event & BEV_EVENT_ERROR) {
        fprintf (stderr, "some other error\n");
    }

    bufferevent_free (bev);
} 

void write_cb(struct bufferevent *bev, void *arg)
{

} 
