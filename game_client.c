#include "dependencies.h"
#include "LL_queue.h"

int main(int argc, char* argv[])
{// read in command line arguements as game parameters
    char *game_type = argv[1], client_buf[BUF_SIZE], server_reply[BUF_SIZE], *ip_buffer;
    int clientsock, res, unique_key;
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

    if ((res = connect(clientsock, (struct sockaddr *) &server, sizeof(server))) == -1)
    {// connect to remote server
        printf("Connection failed\n");
        exit(1);
    }
    
    res = recv(clientsock, server_reply, BUF_SIZE - 1, 0); // receive either socket created or game full message
    printf("%s\n", server_reply);

    res = recv(clientsock, server_reply, BUF_SIZE - 1, 0); // receive unique ftok_key (as pid)
    unique_key = atoi(server_reply);



    if (strcmp(server_reply, "Game has already started") == 0)
    {// if game full disconnect client and exit
        printf("Terminating\n");
        close(clientsock);
        exit(1);
    }

    while (1)
    {// client connected
        memset(client_buf, '\0', sizeof(client_buf));
        memset(server_reply, '\0', sizeof(server_reply));

    


        // read test
        key_t key;
        int msgid;
    
        // ftok to generate unique key
        key = ftok("message_queue", 65);
    
        // msgget creates a message queue
        // and returns identifier
        msgid = msgget(key, 0666 | IPC_CREAT);
    
        // msgrcv to receive message
        msgrcv(msgid, &message, sizeof(message), unique_key, 0);
    
        // display the message
        printf("Data Received is : %s \n", 
                        message.mesg_text);
    
        // to destroy the message queue
        msgctl(msgid, IPC_RMID, NULL);

        

        printf(">>> ");
        scanf("%s", client_buf); // read command
        if ((res = send(clientsock, client_buf, BUF_SIZE, 0)) < 0)
        {// send request
            printf("Sending data to server failed");
            exit(1);
        }

        

        
    }
    // close connection
    close(clientsock);
    return 0;
}
