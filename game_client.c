#include "dependencies.h"


int main(int argc, char* argv[])
{// read in command line arguements as game parameters
    char *game_type = argv[1], client_buf[BUF_SIZE], server_reply[BUF_SIZE], *ip_buffer;
    int clientsock, res;
    struct sockaddr_in server;
    struct hostent *host;
    key_t key;
    
    // take computer as arg, convert to ip, pass ip to sockadd_in struct
    host = gethostbyname(argv[2]);
    get_host_ip(host);
    ip_buffer = inet_ntoa(*((struct in_addr*)host->h_addr_list[0]));
  
    // assign server address and port
    server.sin_addr.s_addr = inet_addr(ip_buffer);
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[3]));

    // create socket
    if ((clientsock = socket(AF_INET, SOCK_STREAM, 0)) < 0) perror("ERROR! Could not create socket");

    if ((res = connect(clientsock, (struct sockaddr *) &server, sizeof(server))) == -1)
    {// connect to remote server
        printf("Connection failed\n");
        exit(1);
    }

    recv(clientsock, server_reply, BUF_SIZE, 0); // receive welcome message
    printf("%s\n", server_reply); // print welcome message
    memset(server_reply, '\0', sizeof(server_reply)); // clear server reply buffer for next message

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
