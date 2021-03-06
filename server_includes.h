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
     end[] = "END", move[] = "MOVE ", go[] = "GO", quit[] = "QUIT",
     game_overview[] = 

"\nGAME OVERVIEW AND RULES\n\n\
You are playing against other players. The aim is to reach 30 first.\n\
A minimum of two players are required to play the game.\n\
Once two players have joined the game will start.\n\
You may enter a single integer between 1 and 9 to make a valid move.\n\
You may type 'quit' at any time to leave the game.\n\
All other input will be classified as invalid.\n\
Upon 5 consecutive invalid inputs you will be disconnected.\n";

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
    char welcome[] = "Welcome to the game";
    // Accept connection from client
    int clientlen = sizeof(client);

    if ((client_id = accept(serversock, (struct sockaddr *)&client, &clientlen)) < 0)
    {
        perror("Accept failed");
        exit(1);
    }

    else printf("Connection accepted\n");

    send(client_id, welcome, sizeof(welcome), 0);             // send welcome message to clients

    sleep(0.5);                                               // deal with any latency before sending next message

    send(client_id, game_overview, sizeof(game_overview), 0); // send game overview to clients

    return client_id;
}

#endif