#include "client_includes.h"

int main(int argc, char* argv[])
{// read in command line arguements as game parameters
    char client_buf[BUF_SIZE], server_reply[BUF_SIZE];
    int clientsock;

    clientsock = client_setup(argv[2], atoi(argv[3]));

    while(1)
    {// loop to play the game
        memset(server_reply, '\0', sizeof(server_reply)); // clear server reply buffer for next message
        recv(clientsock, server_reply, BUF_SIZE, 0); // receive message
        
        if (strncmp(server_reply, "TEXT ", 5) == 0)
        {// client received an error message
            char *temp = server_reply;
            while (*temp != 0 && *(temp++) != ' ') {} // temp = everything after "TEXT "
            printf("%s\n", temp);
        }

        else if (strcmp(server_reply, "END") == 0)
        {// client received end message, quit
            printf("%s\n", server_reply);
            close(clientsock); // terminate connection
            exit(1);
        }

        // else if (strcmp(server_reply, "ERROR") == 0)
        // {// protocol error
        //     close(clientsock); // terminate connection
        //     exit(1);
        // }

        else if (strncmp(server_reply, "Sum is ", 7) == 0)
        {// client received sum - therefore go
            printf("%s\n", server_reply);
            memset(server_reply, '\0', sizeof(server_reply)); // clear server reply buffer for next message
            recv(clientsock, server_reply, BUF_SIZE, 0); // receive go command
            printf("%s\n", server_reply);
            
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

        // else
        // {// GO
        //     printf("%s\n", server_reply);
        //     memset(server_reply, '\0', sizeof(server_reply)); // clear server reply buffer for next message
        //     recv(clientsock, server_reply, BUF_SIZE, 0); // receive go command
        //     printf("%s\n", server_reply);
            
        //     if (strcmp(server_reply, "GO") == 0)
        //     {// client to make a move or send quit
        //         char move[BUF_SIZE] = "MOVE "; // initialise move message

        //         printf("Enter a number: ");
        //         fgets(client_buf, sizeof(client_buf), stdin); // read in move or quit
        //         strtok(client_buf, "\n"); // remove new line char
                
        //         if (strcmp(client_buf, "quit") == 0)
        //         {// send QUIT to server and receive END;
        //             send(clientsock, "QUIT", strlen("QUIT"), 0);
        //             recv(clientsock, server_reply, BUF_SIZE, 0); // receive END
        //             printf("%s\n", server_reply);
        //             close(clientsock); // terminate connection
        //             exit(1);
        //         }

        //         else
        //         {// it was an attempt at a move
        //             strcat(move, client_buf); // concat move to move
        //             send(clientsock, move, sizeof(move), 0); // send move to server
        //         }
        //     }
        // }


        
    
    }

    


    return 0;
}
