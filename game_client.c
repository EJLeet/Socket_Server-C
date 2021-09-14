#include "dependencies.h"

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
        if (strncmp(server_reply, "GO", 2) == 0)
        {// client to make a move
            char move[BUF_SIZE] = "MOVE ";
            printf("Enter a number: ");
            fgets(client_buf, sizeof(client_buf), stdin); // read in move
            strtok(client_buf, "\n"); // remove new line char
            strcat(move, client_buf); // concat move to move
            send(clientsock, move, sizeof(move), 0); // send move to server
        }

        
    
    }

    


    return 0;
}
