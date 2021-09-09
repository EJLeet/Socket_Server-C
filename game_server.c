#include "dependencies.h"
#include "LL_queue.h"

int main(int argc, char* argv[])
{// read in command line arguements as game parameters
    int serversock, clientsock, res, read_size, player_count = 0, message_id, temp;
    char client_buf[BUF_SIZE], welcome[] = "Welcome to the game", ftok_key[BUF_SIZE],
         game_started[] = "Game has already started";
    struct sockaddr_in server, client;
    struct queue* child_pid = create_queue(); // queue to hold child pids
    pid_t cpid; // child processes
    key_t key; // message queue

    if ((serversock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {// Create TCP socket
        perror("ERROR! Creating socket failed\n");
        exit(1);
    }
    printf("Socket successfully created\n");

    // Prepare the sockaddr_in struct
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(atoi(argv[1]));

    if ((res = bind(serversock, (struct sockaddr *) &server, sizeof(server))) < 0)
    {// Bind addr to socket
        perror("ERROR! Bind failed\n");
        exit(1);
    }
    printf("Bind was successfully completed\n");

    if ((res = listen(serversock, BACKLOG)) != 0)
    {// Listen
        perror("ERROR! Listen failed\n");
        exit(1);
    }
    printf("Waiting for incoming connections...\n");

    while(1)
    {// accept multiple connections
        // Accept connection from client
        int clientlen = sizeof(client);
    
        
        if ((clientsock = accept(serversock, (struct sockaddr *) &client, &clientlen)) < 0)
        {// if cant create client exit
            perror("ERROR! Accept failed");
            exit(1);
        }

        else if (player_count++ >= atoi(argv[3])) 
        {// if max players tell client game is full
            send(clientsock, game_started, sizeof(game_started), 0);
            close(clientsock);
        }


        else
        {// client accepted
            printf("Connection accepted\n");
            send(clientsock, welcome, sizeof(welcome), 0); // welcome each client to game
            cpid = fork(); //create child process
        }
        
        if (cpid < 0)
        {// spawn failed
            perror("ERROR! Fork failed");
            exit(1);
        }

        else if (cpid == 0)
        {// child process
            while(1)
            {   
                close(serversock); 
                memset(client_buf, '\0', BUF_SIZE);
                if ((read_size = recv(clientsock, client_buf, BUF_SIZE, 0)) < 0)
                {// Receive request
                    perror("ERROR! Recv failed");
                    exit(1);
                }
   
            }
        }

        else 
        {// parent process
            enqueue(child_pid, cpid); // add each child pid to queue
            // printf("queue Front : %d \n", child_pid->front->item);
            // printf("queue Rear : %d\n", child_pid->rear->item);

            snprintf(ftok_key, BUF_SIZE, "%d", cpid); // convert cpid to string and save to char
            send(clientsock, ftok_key, sizeof(ftok_key), 0); // send child their pid
            memset(ftok_key, '\0', BUF_SIZE); // clear ftok_key char for next child

            if (player_count >=2)
            {// if 2 people have joined start the game
                for(;;)
                {
                    key = ftok("message", 65); // generate unique key
                    message_id = msgget(key, 0666 | IPC_CREAT); // create a message queue return identifier
                    message_queue.message_type = child_pid->front->item; // assign message type to whoever is at front of queue
                    printf("Write Data : ");
                    fgets(message_queue.message_text,BUF_SIZE,stdin); 
                    msgsnd(message_id, &message_queue, sizeof(message_queue), 0); // msgsnd to send message
                    printf("Data send is : %s \n", message_queue.message_text); // display the message

                    // rotate through queue in parent sending child message based on their pid key
                    temp = child_pid->front->item; // get child pid before dequeue
                    dequeue(child_pid);
                    enqueue(child_pid, temp);
                }
                
            }
            else
            {// game ends if < 2 people

            }
        }
    }
    close(clientsock);
    close(serversock);

    return 0;
}

