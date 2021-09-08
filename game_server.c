#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define BACKLOG 10
#define BUF_SIZE 1024

// Queue (implimented as Linked List) to handle child pids in order to see whose turn it is
struct QNode { int key; struct QNode* next; };
struct Queue { struct QNode *front, *rear; };
struct QNode* newNode(int k);
struct Queue* createQueue();
void enqueue(struct Queue* q, int k);
void dequeue(struct Queue* q);

int main(int argc, char* argv[])
{// read in command line arguements as game parameters
    int game_args = atoi(argv[3]), serversock, clientsock, res, read_size, player_count = 0;
    char *game_type = argv[2], client_buf[BUF_SIZE], welcome[] = "Welcome to the game", 
         game_started[] = "Game has already started";
    struct sockaddr_in server, client;
    pid_t cpid; // child processes
    struct Queue* child_pid = createQueue(); // queue to hold child pids

    // struct Queue* q = createQueue();
    // enQueue(q, 10);
    // enQueue(q, 20);
    // deQueue(q);
    // deQueue(q);
    // enQueue(q, 30);
    // enQueue(q, 40);
    // enQueue(q, 50);
    // deQueue(q);
    // printf("Queue Front : %d \n", q->front->key);
    // printf("Queue Rear : %d", q->rear->key);

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
    
        clientsock = accept(serversock, (struct sockaddr *) &client, &clientlen);

        if (player_count++ >= atoi(argv[3])) 
        {// if max players tell client game is full
            send(clientsock, game_started, sizeof(game_started), 0);
            close(clientsock);
        }
        else if (clientsock < 0)
        {// if cant create client exit
            perror("ERROR! Accept failed");
            exit(1);
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
            printf("Queue Front : %d \n", child_pid->front->key);
            printf("Queue Rear : %d\n", child_pid->rear->key);

            if (player_count >=2)
            {// if 2 people have joined start the game
                player_count = atoi(argv[3]); // no players can join after game started

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

void dequeue(struct Queue* q)
{// pop first item from queue
    if (q->front == NULL) return;
    struct QNode* temp = q->front; // assign head to temp
    q->front = q->front->next; // assign head - 1 to head
    if (q->front == NULL) q->rear = NULL; // If front becomes NULL, then change rear to NULL
    free(temp); // free old head
}

void enqueue(struct Queue* q, int k)
{// push k to queue
    struct QNode* temp = newNode(k); // create new node
    if (q->rear == NULL) 
    {// If queue is empty, then new node is front and rear both
        q->front = q->rear = temp;
        return;
    }
    q->rear->next = temp; // Add the new node at the end of queue and change rear
    q->rear = temp;
}

struct Queue* createQueue()
{// initialises empty queue
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}

struct QNode* newNode(int k)
{// creates a new node
    struct QNode* temp = (struct QNode*)malloc(sizeof(struct QNode));
    temp->key = k;
    temp->next = NULL;
    return temp;
}