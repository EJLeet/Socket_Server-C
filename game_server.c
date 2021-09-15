#include "server_includes.h"
#include "LL_queue.h"


int main(int argc, char* argv[])
{// read in command line arguements as game parameters
    int serversock, clientsock, player_count = 0, score = 0;
    char client_buf[BUF_SIZE];
    struct queue* game_order = create_queue(); // queue to hold child pids

    serversock = server_setup(atoi(argv[1]), BACKLOG);

    while(player_count++ < atoi(argv[3])) 
    {
        clientsock = accept_clients(serversock);  
        enqueue(game_order, clientsock); // add clients to queue with clientsock as their id
    }

    while (1)
    {// loop to play the game
        char sum[BUF_SIZE] = "Sum is "; // sum to send to client
        char score_char[BUF_SIZE]; // setup score char to strncat to sum[]
        memset(client_buf, '\0', sizeof(client_buf)); // clear client buffer    
        clientsock = game_order->front->item; // get client from fron of queue - its their turn

        snprintf(score_char, BUF_SIZE, "%d", score); // convert score to char
        strncat(sum, score_char, sizeof(score_char)); // concat score to sum
        send(clientsock, sum, sizeof(sum), 0); // send sum is x to client
        sleep (0.5); // sleep to deal with any latency

        send(clientsock, "GO", strlen("GO"), 0); // send go to client
        recv(clientsock, client_buf, BUF_SIZE, 0); // wait to receive message
        
        // TODO add capital commands to dependencies

        if (strncmp(client_buf, "MOVE ", 5) == 0)
        {// calculate score and handle invalid commands
            char temp[BUF_SIZE]; // used to strtok
            strcpy(temp, client_buf);
            char *token = strtok(temp, "MOVE "); // strtok number entered after move
            
            if (strlen(client_buf) > 6 || atoi(token) <= 0) 
            {// invalid move - letter, number <= 0 or > 9 or invalid command entered
                printf("error\n");
            }
            else score += atoi(token); // increment score
            
            // TODO end game when score above 30
        }
        else if (strcmp(client_buf, "QUIT") == 0)
        {// client wants to quit, send end 
            send(clientsock, "END", strlen("END"), 0);
            close(clientsock); // terminate client connection
        }
        
        dequeue(game_order); // dequeue
        enqueue(game_order, clientsock); // enqueue
    }
    return 0;
}

