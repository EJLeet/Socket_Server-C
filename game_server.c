#include "dependencies.h"
#include "LL_queue.h"

int main(int argc, char* argv[])
{// read in command line arguements as game parameters
    int serversock, clientsock, res, player_count = 0;
    char client_buf[BUF_SIZE];
    struct sockaddr_in server, client;
    struct queue* game_order = create_queue(); // queue to hold child pids

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

    while(player_count++ < atoi(argv[3]))
    {// accept clients until player count is reached
        // Accept connection from client
        int clientlen = sizeof(client);
        char queue_id[BUF_SIZE];
        if((clientsock = accept(serversock, (struct sockaddr *) &client, &clientlen)) < 0)
        {
            perror("Accept failed");
            exit(1);
        }
        else printf("Connection accepted\n");

        printf("client sock id is %d\n", clientsock);

        enqueue(game_order, clientsock); // add clients to queue with clientsock as their id
        send(clientsock, welcome, sizeof(welcome), 0); // send welcome message to clients
        snprintf(queue_id, BUF_SIZE, "%d", clientsock); // convert clientsock to string to pass to client
        send(clientsock, queue_id, sizeof(queue_id), 0); // send queue id to client 

        printf("queue id sent to client is %d\n", clientsock);
    }
    printf("outside loop\n");
    printf("queue front item = %d\n", game_order->front->item);
    while (1)
    {
        send(game_order->front->item, welcome, sizeof(welcome), 0);
    }
    // while (1)
    // {// loop to play the game
    //     printf("front of queue %d", game_order->front->item);
    //     send(game_order->front->item, "TEXT", strlen("TEXT"), 0);
    //     send(game_order->front->item, "GO", strlen("GO"), 0);
        
    // }

    return 0;
}

