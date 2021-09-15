#ifndef SERVER_INCLUDES
#define SERVER_INCLUDES

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>

#define BACKLOG 10
#define BUF_SIZE 1024

int server_setup(int portnum, int backlog);
int accept_clients(int serversock);

char lose_message[] = "You Lost!", win_message[] = "You Won!",
     end[] = "END", move[] = "MOVE ", go[] = "GO", quit[] = "QUIT";

int server_setup(int portnum, int backlog)
{ /*
                Handles server setup:
                create sockes
                setup server
                bind
                listen
                returns server socket
                                                    */
    struct sockaddr_in server;
    int sock, res;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    { // Create TCP socket
        perror("ERROR! Creating socket failed\n");
        exit(1);
    }
    printf("Socket successfully created\n");

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(portnum);
    server.sin_family = AF_INET;

    if ((res = bind(sock, (struct sockaddr *)&server, sizeof(server))) < 0)
    { // Bind addr to socket
        perror("ERROR! Bind failed\n");
        exit(1);
    }
    printf("Bind was successfully completed\n");

    if ((res = listen(sock, BACKLOG)) != 0)
    { // Listen
        perror("ERROR! Listen failed\n");
        exit(1);
    }
    printf("Waiting for incoming connections...\n");

    return sock;
}

int accept_clients(int serversock)
{ /*
                Handles accepting clients:
                accept clientd
                sends them welcome message
                returns client id
                                                    */
    int client_id;
    struct sockaddr_in client;
    char welcome[] = "Welcome to the game", queue_id[BUF_SIZE];
    // Accept connection from client
    int clientlen = sizeof(client);

    if ((client_id = accept(serversock, (struct sockaddr *)&client, &clientlen)) < 0)
    {
        perror("Accept failed");
        exit(1);
    }

    else printf("Connection accepted\n");

    send(client_id, welcome, sizeof(welcome), 0); // send welcome message to clients

    return client_id;
}

#endif