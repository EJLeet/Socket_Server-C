#include "dependencies.h"
#include "LL_queue.h"

int main(int argc, char* argv[])
{// read in command line arguements as game parameters
    int serversock, clientsock, res, player_count = 0, score = 0;
    char client_buf[BUF_SIZE], welcome[] = "Welcome to the game";
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

        enqueue(game_order, clientsock); // add clients to queue with clientsock as their id
        send(clientsock, welcome, sizeof(welcome), 0); // send welcome message to clients

    }

    while (1)
    {// loop to play the game
        memset(client_buf, '\0', sizeof(client_buf)); // clear client buffer    
        clientsock = game_order->front->item; // get client from fron of queue - its their turn
        char sum[BUF_SIZE] = "Sum is "; // sum to send to client
        char score_char[BUF_SIZE]; // setup score char to strncat to sum[]
        snprintf(score_char, BUF_SIZE, "%d", score); // convert score to char
        strncat(sum, score_char, sizeof(score_char)); // concat score to sum
        send(clientsock, sum, sizeof(sum), 0); // send sum is x to client
        sleep (0.5); // sleep to deal with any latency
        send(clientsock, "GO", strlen("GO"), 0); // send go to client
        recv(clientsock, client_buf, BUF_SIZE, 0); // wait to receive message
        
        if (strncmp(client_buf, "MOVE ", 5) == 0)
        {// calculate score
            char temp[BUF_SIZE]; // used to strtok
            strcpy(temp, client_buf);
            char *token = strtok(temp, "MOVE "); // strtok number entered after move
            if (strlen(client_buf) > 6 || atoi(token) <= 0) 
            {// invalid move - letter, number <= 0 or invalid command entered
                printf("error\n");
            }
            else score += atoi(token); // increment score
        }
        
        dequeue(game_order); // dequeue
        enqueue(game_order, clientsock); // enqueue
    }
    return 0;
}

