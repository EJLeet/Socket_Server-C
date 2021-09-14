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

int server_setup(int portnum, int backlog)
{
    struct sockaddr_in server; 
    int sock, res;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {// Create TCP socket
        perror("ERROR! Creating socket failed\n");
        exit(1);
    }
    printf("Socket successfully created\n");
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons (portnum);
    server.sin_family = AF_INET ;      
    if ((res = bind(sock, (struct sockaddr *) &server, sizeof(server))) < 0)
    {// Bind addr to socket
        perror("ERROR! Bind failed\n");
        exit(1);
    }
    printf("Bind was successfully completed\n");
    if ((res = listen(sock, BACKLOG)) != 0)
    {// Listen
        perror("ERROR! Listen failed\n");
        exit(1);
    }
    printf("Waiting for incoming connections...\n");
    return sock;
}

int accept_clients(int serversock)
{
    int client_id;
    struct sockaddr_in client;
    char welcome[] = "Welcome to the game";
    // Accept connection from client
    int clientlen = sizeof(client);
    char queue_id[BUF_SIZE];
    if((client_id = accept(serversock, (struct sockaddr *) &client, &clientlen)) < 0)
    {
        perror("Accept failed");
        exit(1);
    }
    else printf("Connection accepted\n");
    send(client_id, welcome, sizeof(welcome), 0); // send welcome message to clients
    return client_id;
}

void get_host_ip(struct hostent * hostentry)
{// use hostname to get ip address
    if (hostentry == NULL)
    {
        perror("ERROR! Invalid hostname");
        exit(1);
    }
}

int client_setup(char *host_name, int portnum) 
{
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