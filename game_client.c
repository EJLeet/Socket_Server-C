#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

#define BUF_SIZE 1024

void checkHostEntry(struct hostent * hostentry);

int main(int argc, char* argv[])
{// read in command line arguements as game parameters
    char *game_type = argv[1], client_buf[BUF_SIZE], server_reply[BUF_SIZE], *ip_buffer;
    int clientsock, res;
    struct sockaddr_in server;
    struct hostent *host;
    
    // take computer as arg, convert to ip, pass ip below
    host = gethostbyname(argv[2]);
    checkHostEntry(host);
    ip_buffer = inet_ntoa(*((struct in_addr*)host->h_addr_list[0]));
  
    // assign server address and port
    server.sin_addr.s_addr = inet_addr(ip_buffer);
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[3]));

    // create socket
    clientsock = socket(AF_INET, SOCK_STREAM, 0);
    if (clientsock < 0) perror("Could not create socket");
    printf("Socket created\n");

    // connect to remote server
    res = connect(clientsock, (struct sockaddr *) &server, sizeof(server));
    if (res == -1)
    {
        printf("Connection failed\n");
        exit(1);
    }

    return 0;
}

void checkHostEntry(struct hostent * hostentry)
{
    if (hostentry == NULL)
    {
        perror("gethostbyname");
        exit(1);
    }
}