#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BACKLOG 10

int main(int argc, char* argv[])
{// read in command line arguements as game parameters
    int game_args = atoi(argv[2]), serversock, clientsock, res;
    char *game_type = argv[3];
    struct sockaddr_in server, client;

    // Create TCP socket
    if ((serversock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("ERROR! Creating socket failed\n");
        exit(1);
    }
    printf("Socket successfully created\n");

    // Prepare the sockaddr_in struct
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(atoi(argv[1]));

    // Bind addr to socket
    if ((res = bind(serversock, (struct sockaddr *) &server, sizeof(server))) < 0)
    {
        perror("ERROR! Bind failed\n");
        exit(1);
    }
    printf("Bind was successfully completed\n");

    // Listen
    if ((res = listen(serversock, BACKLOG)) != 0)
    {
        perror("ERROR! Listen failed\n");
        exit(1);
    }
    printf("Waiting for incoming connections...\n");

    // Accept connection from client
    int clientlen = sizeof(client);
    if ((clientsock = accept(serversock, (struct sockaddr *) &client, &clientlen)) < 0)
    {
        perror("ERROR! Accept failed\n");
        exit(1);
    }
    else printf("Connection accepted\n");

    return 0;
}