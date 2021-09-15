#ifndef CLIENT_INCLUDES
#define CLIENT_INCLUDES

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define BUF_SIZE 1024

void get_host_ip(struct hostent *hostentry)
{// use hostname to get ip address
    if (hostentry == NULL)
    {
        perror("ERROR! Invalid hostname");
        exit(1);
    }
}

int client_setup(char *host_name, int portnum) 
{/*
                Handles client connection to server:
                gets hostname and converts to ip
                assign server address and port
                create socket
                connect to socket
                returns client id
                                                    */
    struct sockaddr_in server;
    struct hostent *host;
    char *ip_buffer, server_reply[BUF_SIZE];
    int res, client_id;
    // take computer as arg, convert to ip, pass ip to sockadd_in struct
    host = gethostbyname(host_name);
    get_host_ip(host);
    ip_buffer = inet_ntoa(*((struct in_addr*)host->h_addr_list[0]));
    // assign server address and port
    server.sin_addr.s_addr = inet_addr(ip_buffer);
    server.sin_family = AF_INET;
    server.sin_port = htons(portnum);
    // create socket
    if ((client_id = socket(AF_INET, SOCK_STREAM, 0)) < 0) perror("ERROR! Could not create socket");
    if ((res = connect(client_id, (struct sockaddr *) &server, sizeof(server))) == -1)
    {// connect to remote server
        printf("Connection failed\n");
        exit(1);
    }
    recv(client_id, server_reply, BUF_SIZE, 0); // receive welcome message
    printf("%s\n", server_reply); // print welcome message
    memset(server_reply, '\0', sizeof(server_reply)); // clear server reply buffer for next message
    return client_id;
}

#endif