#include "client_includes.h"

int main(int argc, char* argv[])
{// read in command line arguements as game parameters
    char client_buf[BUF_SIZE], server_reply[BUF_SIZE];
    int clientsock;

    clientsock = client_setup(argv[2], atoi(argv[3]));

    while(1)
    {// loop to play the game
        recv(clientsock, server_reply, BUF_SIZE, 0); // receive sum message
        printf("%s\n", server_reply);
        memset(server_reply, '\0', sizeof(server_reply)); // clear server reply buffer for next message
        recv(clientsock, server_reply, BUF_SIZE, 0); // receive go command
       
        if (strcmp(server_reply, "GO") == 0)
        {// client to make a move or send quit
            char move[BUF_SIZE] = "MOVE "; // initialise move message
            printf("Enter a number: ");
            fgets(client_buf, sizeof(client_buf), stdin); // read in move or quit
            strtok(client_buf, "\n"); // remove new line char

            if (strcmp(client_buf, "quit") == 0)
            {// send QUIT to server and receive END;
                send(clientsock, "QUIT", strlen("QUIT"), 0);
                recv(clientsock, server_reply, BUF_SIZE, 0); // receive END
                printf("%s\n", server_reply);
                close(clientsock); // terminate connection
                exit(1);
            }

            else
            {// it was an attempt at a move
                strcat(move, client_buf); // concat move to move
                send(clientsock, move, sizeof(move), 0); // send move to server
            }
            
        }

        
    
    }

    


    return 0;
}
