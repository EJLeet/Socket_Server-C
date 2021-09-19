#include "server_includes.h"
#include "LL_queue.h"

int main(int argc, char *argv[])
{ // read in command line arguements as game parameters
    int serversock, clientsock, player_count = 0, score = 0, ret, error_count = 0;
    char client_buf[BUF_SIZE];
    struct queue *game_order = create_queue(); // queue to hold child pids
    struct pollfd fd;                          // used for receive timeout

    serversock = server_setup(atoi(argv[1]), BACKLOG); // setup server

    while (player_count++ < atoi(argv[3]))
    { // accept clients until max players reached
        clientsock = accept_clients(serversock);
        enqueue(game_order, clientsock); // add clients to queue with clientsock as their id
    }

    for (int i = 5; i > 0; i--)
    { // send 5 second countdown message
        for (int j = 0; j < player_count - 1; j++)
        { // send to each client
            char counter[BUF_SIZE], game_starting[BUF_SIZE] = "Game will start in ";
            snprintf(counter, BUF_SIZE, "%d...", i);                   // convert counter to char
            strcat(game_starting, counter);                            // concat game starting and counter
            clientsock = game_order->front->item;
            send(clientsock, game_starting, sizeof(game_starting), 0); // send game starting
            dequeue(game_order);
            enqueue(game_order, clientsock);                 
        }
        sleep(1);
    }

    while (1)
    { // loop to play the game
        char sum[BUF_SIZE] = "Sum is ", text[BUF_SIZE] = "TEXT "; // initiliase client messages
        char score_char[BUF_SIZE];                                // setup score char to strncat to sum[]
        memset(client_buf, '\0', sizeof(client_buf));             // clear client buffer
        clientsock = game_order->front->item;                     // get client from fron of queue - its their turn

        if (score >= 30)
        { // loop through all losing clients
            strcat(text, lose_message);
            for (int i = 0; i < player_count - 2; i++)
            { // send you lost message
                send(clientsock, text, sizeof(text), 0);
                close(clientsock); // terminate client connection
                dequeue(game_order);
                clientsock = game_order->front->item;
            }
            close(serversock);
            exit(1);
        }

        if (player_count <= 2)
        { // one player left - they won
            strcat(text, win_message);
            send(clientsock, text, sizeof(text), 0); // send winner you won
            close(clientsock);                       // terminate client connection
            close(serversock);
            exit(1);
        }

        snprintf(score_char, BUF_SIZE, "%d", score);  // convert score to char
        strncat(sum, score_char, sizeof(score_char)); // concat score to sum
        send(clientsock, sum, sizeof(sum), 0);        // send sum is x to client
        sleep(0.5);                                   // sleep to deal with any latency
        send(clientsock, go, sizeof(go), 0);          // send go to client

        fd.fd = clientsock; // setup poll to record time
        fd.events = POLLIN;
        ret = poll(&fd, 1, 30000); // 30 second for timeout

        if (ret == 0)
        { // timeout - disconnect client
            send(clientsock, end, sizeof(end), 0);
            dequeue(game_order);
            player_count--;
            close(clientsock); // terminate client connection
        }

        else
        { // received message within time
            recv(clientsock, client_buf, BUF_SIZE, 0); // wait to receive message

            if (strncmp(client_buf, move, 5) == 0)
            { // calculate score and handle invalid commands
                char temp[BUF_SIZE]; // used to strtok
                strcpy(temp, client_buf);
                char *token = strtok(temp, move); // strtok number entered after move

                if (strcmp(client_buf, "MOVE  ") == 0 || strlen(client_buf) > 6 || atoi(token) <= 0)
                { // invalid move - letter, number <= 0 or > 9 or invalid command entered
                    strcat(text, "ERROR: Invalid Command");
                    send(clientsock, text, sizeof(text), 0); // send error message to client
                    error_count++;

                    if (error_count == 5)
                    { // disconnect client
                        send(clientsock, end, sizeof(end), 0);
                        dequeue(game_order);
                        close(clientsock); // terminate client connection
                        player_count--;
                    }
                }

                else
                { // valid move
                    dequeue(game_order);  // next players turn
                    enqueue(game_order, clientsock);
                    score += atoi(token); // increment score
                    error_count = 0;      // reset error count after valid move

                    if (score >= 30)
                    { // Game has been won
                        strcat(text, win_message);
                        send(clientsock, text, sizeof(text), 0); // send winner you won
                        close(clientsock);                       // terminate client connection
                    }
                }
            }

            else
            { // ERROR / protocol error / QUIT
                player_count--;
                send(clientsock, end, sizeof(end), 0);
                dequeue(game_order); // next players turn
                close(clientsock);   // terminate client connection
            }
        }
    }
    return 0;
}
