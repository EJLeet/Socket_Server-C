#ifndef CLIENT_INCLUDES
#define CLIENT_INCLUDES

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUF_SIZE 1024

void get_host_ip(struct hostent *hostentry);
int client_setup(char *host_name, int portnum);
void display_message(int clientsock, char server_reply[BUF_SIZE]);

char text[] = "TEXT ", go[] = "GO", quit[] = "QUIT";

void get_host_ip(struct hostent *hostentry)
{ // use hostname to get ip address
    if (hostentry == NULL)
    {
        perror("ERROR! Invalid hostname");
        exit(1);
    }
}

int client_setup(char *host_name, int portnum)
{ /*
                Handles client connection to server:
                gets hostname and converts to ip
                assign server address and port
                create socket
                connect to socket
                returns client id
                                                    */
    struct sockaddr_in server;
    struct hostent *host;
    char *ip_buffer, server_reply[BUF_SIZE];
    int res, client_id;

    // take computer as arg, convert to ip, pass ip to sockadd_in struct
    host = gethostbyname(host_name);
    get_host_ip(host);
    ip_buffer = inet_ntoa(*((struct in_addr *)host->h_addr_list[0]));

    // assign server address and port
    server.sin_addr.s_addr = inet_addr(ip_buffer);
    server.sin_family = AF_INET;
    server.sin_port = htons(portnum);

    // create socket
    if ((client_id = socket(AF_INET, SOCK_STREAM, 0)) < 0) perror("ERROR! Could not create socket");

    if ((res = connect(client_id, (struct sockaddr *)&server, sizeof(server))) == -1)
    { // connect to remote server
        perror("ERROR! Connection failed");
        exit(1);
    }

    display_message(client_id, server_reply); // receive welcome message

    memset(server_reply, '\0', sizeof(server_reply)); // clear server reply buffer for next message               

    display_message(client_id, server_reply); // receive game overview

    memset(server_reply, '\0', sizeof(server_reply)); // clear server reply buffer for next message               

    return client_id;
}

void display_message(int clientsock, char server_reply[BUF_SIZE])
{ // recieve and display message sent by client
    recv(clientsock, server_reply, BUF_SIZE, 0);
    printf("%s\n", server_reply);
}

#endif