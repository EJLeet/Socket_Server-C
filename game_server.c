#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define BACKLOG 10
#define BUF_SIZE 1024

int main(int argc, char* argv[])
{// read in command line arguements as game parameters
    int game_args = atoi(argv[2]), serversock, clientsock, res, read_size;
    char *game_type = argv[3], client_buf[BUF_SIZE], server_reply[BUF_SIZE];
    struct sockaddr_in server, client;
    pid_t cpid; // child processes

    if ((serversock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {// Create TCP socket
        perror("ERROR! Creating socket failed\n");
        exit(1);
    }
    printf("Socket successfully created\n");

    // Prepare the sockaddr_in struct
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(atoi(argv[1]));

    if ((res = bind(serversock, (struct sockaddr *) &server, sizeof(server))) < 0)
    {// Bind addr to socket
        perror("ERROR! Bind failed\n");
        exit(1);
    }
    printf("Bind was successfully completed\n");

    if ((res = listen(serversock, BACKLOG)) != 0)
    {// Listen
        perror("ERROR! Listen failed\n");
        exit(1);
    }
    printf("Waiting for incoming connections...\n");

    while(1)
    {// accept multiple connections
        // Accept connection from client
        int clientlen = sizeof(client);
        if((clientsock = accept(serversock, (struct sockaddr *) &client, &clientlen)) < 0)
        {
            perror("ERROR! Accept failed\n");
            exit(1);
        }
        printf("Connection accepted\n");
        
        cpid = fork(); //create child process
        if (cpid < 0)
        {// spawn failed
            perror("ERROR! Fork failed\n");
            exit(1);
        }
        else if (cpid == 0)
        {// child process
            while(1)
            {
                close(serversock); 
                memset(client_buf, '\0', BUF_SIZE);

                if ((read_size = recv(clientsock, client_buf, BUF_SIZE, 0)) < 0)
                {// Receive request
                    perror("ERROR! Recv failed\n");
                    exit(1);
                }
   
                if (strcmp(client_buf, "quit") == 0)
                {// Send request
                    printf("Server Terminating\n");
                    close(clientsock);
                    exit(0);
                }
            }
        }
        // else{} this is the parent process
    }
    close(clientsock);
    close(serversock);

    return 0;
}