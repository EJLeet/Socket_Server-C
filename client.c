#include <stdio.h>
#include <stdlib.h>
//#include <sys/types.h>
//#include <sys/socket.h>
#include <netinet/in.h>
#define BUF_SIZE 1024

int main(int argc, char* argv[])
{// read in command line arguements as game parameters
    char *game_type = argv[1], /**server_name = argv[2],*/ client_buf[BUF_SIZE], server_reply[BUF_SIZE];
    int /*port_number = atoi(argv[3]),*/ clientsock, res;
    struct sockaddr_in server;

    // take computer as arg, convert to ip, pass ip below

    // assign server address and port
    server.sin_addr.s_addr = inet_addr(argv[2]);
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