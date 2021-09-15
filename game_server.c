#include "server_includes.h"
#include "LL_queue.h"


int main(int argc, char* argv[])
{// read in command line arguements as game parameters
    int serversock, clientsock, player_count = 0, score = 0, ret, error_count = 0;
    char client_buf[BUF_SIZE];
    struct queue* game_order = create_queue();// queue to hold child pids
    struct pollfd fd; // used for receive timeout

    serversock = server_setup(atoi(argv[1]), BACKLOG);

    while(player_count++ < atoi(argv[3])) 
    {
        clientsock = accept_clients(serversock);  
        enqueue(game_order, clientsock); // add clients to queue with clientsock as their id
    }

    while (1)
    {// loop to play the game
        char sum[BUF_SIZE] = "Sum is ", text[BUF_SIZE] = "TEXT "; // initiliase client messages
        char score_char[BUF_SIZE]; // setup score char to strncat to sum[]
        memset(client_buf, '\0', sizeof(client_buf)); // clear client buffer    
        clientsock = game_order->front->item; // get client from fron of queue - its their turn

        snprintf(score_char, BUF_SIZE, "%d", score); // convert score to char
        strncat(sum, score_char, sizeof(score_char)); // concat score to sum
        send(clientsock, sum, sizeof(sum), 0); // send sum is x to client
        sleep (0.5); // sleep to deal with any latency

        send(clientsock, "GO", strlen("GO"), 0); // send go to client

        fd.fd = clientsock; // setup poll to record time
        fd.events = POLLIN;
        ret = poll(&fd, 1, 30000); // 30 second for timeout

        if(ret == 0)
        {// timeout - disconnect client
            send(clientsock, "END", strlen("END"), 0);
            close(clientsock); // terminate client connection
        }

        // received message within time
        else recv(clientsock, client_buf, BUF_SIZE, 0); // wait to receive message
        
        // TODO add capital commands to dependencies

        if (strncmp(client_buf, "MOVE ", 5) == 0)
        {// calculate score and handle invalid commands
            char temp[BUF_SIZE]; // used to strtok
            strcpy(temp, client_buf);
            char *token = strtok(temp, "MOVE "); // strtok number entered after move
            
            if (strlen(client_buf) > 6 || atoi(token) <= 0) 
            {// invalid move - letter, number <= 0 or > 9 or invalid command entered
                strcat(text, "ERROR: Invalid Command");
                send(clientsock, text, sizeof(text), 0); // send error message to client
                error_count++;
                if (error_count == 5)
                {// disconnect client
                    send(clientsock, "END", strlen("END"), 0);
                    close(clientsock); // terminate client connection
                }
            }

            // TODO end game when score above 30

            else 
            {// valid move
                dequeue(game_order); // next players turn
                enqueue(game_order, clientsock); 
                score += atoi(token); // increment score
            }
            
        }
        else if (strcmp(client_buf, "QUIT") == 0)
        {// client wants to quit, send end 
            send(clientsock, "END", strlen("END"), 0);
            close(clientsock); // terminate client connection
        }
    }
    return 0;
}

