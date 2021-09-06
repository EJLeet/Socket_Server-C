#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 1024

void get_host_ip(struct hostent * hostentry);

int main(int argc, char* argv[])
{// read in command line arguements as game parameters
    char *game_type = argv[1], client_buf[BUF_SIZE], server_reply[BUF_SIZE], *ip_buffer;
    int clientsock, res;
    struct sockaddr_in server;
    struct hostent *host;
    
    // take computer as arg, convert to ip, pass ip to sockadd_in struct
    host = gethostbyname(argv[2]);
    get_host_ip(host);
    ip_buffer = inet_ntoa(*((struct in_addr*)host->h_addr_list[0]));
  
    // assign server address and port
    server.sin_addr.s_addr = inet_addr(ip_buffer);
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[3]));

    // create socket
    if ((clientsock = socket(AF_INET, SOCK_STREAM, 0)) < 0) perror("Could not create socket");
    printf("Socket created\n");

    if ((res = connect(clientsock, (struct sockaddr *) &server, sizeof(server))) == -1)
    {// connect to remote server
        printf("Connection failed\n");
        exit(1);
    }
    while (1)
    {
        memset(client_buf, '\0', sizeof(client_buf));
        memset(server_reply, '\0', sizeof(server_reply));
        printf(">>> ");
        // read from stdin to client_buf
        scanf("%s", client_buf);

        if ((res = send(clientsock, client_buf, BUF_SIZE, 0)) < 0)
        {// send request
            printf("Sending data to server failed");
            exit(1);
        }

        if (strcmp(client_buf, "quit") == 0)
        {// receive request
            res = recv(clientsock, server_reply, BUF_SIZE - 1, 0);
            printf("Client Terminating\n");
            close(clientsock);
            exit(0);
        }
    }
    // close connection
    close(clientsock);
    return 0;
}

void get_host_ip(struct hostent * hostentry)
{
    if (hostentry == NULL)
    {
        perror("invalid hostname");
        exit(1);
    }
}