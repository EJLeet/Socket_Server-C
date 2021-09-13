#ifndef DEPENDENCIES
#define DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BACKLOG 10
#define BUF_SIZE 1024
int test = 1;
char welcome[] = "Welcome to the game";

void get_host_ip(struct hostent * hostentry)
{
    if (hostentry == NULL)
    {
        perror("invalid hostname");
        exit(1);
    }
}

struct message_buffer 
{
    long long int message_type;
    char message_text[BUF_SIZE];
}   message_queue;

#endif