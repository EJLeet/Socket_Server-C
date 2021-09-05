#include <stdio.h>
#include <stdlib.h>
//#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BACKLOG 10

int main(int argc, char* argv[])
{// read in command line arguements as game parameters
    int /*port_number = atoi(argv[1]),*/ game_args = atoi(argv[2]), serversock, clientsock, res;
    char *game_type = argv[3];
    struct sockaddr_in server, client;

    // Create TCP socket
    serversock = socket(AF_INET, SOCK_STREAM, 0);
    if(serversock == -1)
    {
        printf("Creating socket failed\n");
        exit(1);
    }
    printf("Socket successfully created\n");

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(atoi(argv[1]));

    // Bind addr to socket
    res = bind(serversock, (struct sockaddr *) &server, sizeof(server));
    if(res < 0)
    {
        printf("Bind failed\n");
        exit(1);
    }
    printf("Bind was successfully completed\n");

    // Listen
    res = listen(serversock, BACKLOG);
    if(res != 0)
    {
        printf("Listen failed\n");
        exit(1);
    }
    printf("Waiting for incoming connections...\n");

    // Accept connection from client
    int clientlen = sizeof(client);
    clientsock = accept(serversock, (struct sockaddr *) &client, &clientlen);
    if(clientsock < 0)
    {
        perror("Accept failed");
        exit(1);
    }
    else printf("Connection accepted\n");

    return 0;
}