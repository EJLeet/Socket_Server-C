#ifndef DEPENDENCIES
#define DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
//#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
//#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
//#include <errno.h>

#define BACKLOG 10
#define BUF_SIZE 1024

void get_host_ip(struct hostent * hostentry)
{
    if (hostentry == NULL)
    {
        perror("invalid hostname");
        exit(1);
    }
}

#endif