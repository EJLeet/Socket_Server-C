#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define BACKLOG 10
#define BUF_SIZE 1024

// queue (implimented as Linked List) to handle child pids in order to see whose turn it is
struct node { int item; struct node* next; };
struct queue { struct node *front, *rear; };
struct node* new_node(int i);
struct queue* create_queue();
void enqueue(struct queue* q, int i);
void dequeue(struct queue* q);

int main(int argc, char* argv[])
{// read in command line arguements as game parameters
    int game_args = atoi(argv[3]), serversock, clientsock, res, read_size, player_count = 0;
    char *game_type = argv[2], client_buf[BUF_SIZE], welcome[] = "Welcome to the game", 
          game_started[] = "Game has already started";
    struct sockaddr_in server, client;
    pid_t cpid; // child processes
    struct queue* child_pid = create_queue(); // queue to hold child pids

    // struct queue* q = create_queue();
    // enqueue(q, 10);
    // enqueue(q, 20);
    // dequeue(q);
    // dequeue(q);
    // enqueue(q, 30);
    // enqueue(q, 40);
    // enqueue(q, 50);
    // dequeue(q);
    // printf("queue Front : %d \n", q->front->item);
    // printf("queue Rear : %d", q->rear->item);

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
            printf("queue Front : %d \n", child_pid->front->item);
            printf("queue Rear : %d\n", child_pid->rear->item);

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

void dequeue(struct queue* q)
{// pop first item from queue
    if (q->front == NULL) return;
    struct node* temp = q->front; // assign head to temp
    q->front = q->front->next; // assign head - 1 to head
    if (q->front == NULL) q->rear = NULL; // If front becomes NULL, then change rear to NULL
    free(temp); // free old head
}

void enqueue(struct queue* q, int i)
{// push i to queue
    struct node* temp = new_node(i); // create new node
    if (q->rear == NULL) 
    {// If queue is empty, then new node is front and rear both
        q->front = q->rear = temp;
        return;
    }
    q->rear->next = temp; // Add the new node at the end of queue and change rear
    q->rear = temp;
}

struct queue* create_queue()
{// initialises empty queue
    struct queue* q = (struct queue*)malloc(sizeof(struct queue));
    q->front = q->rear = NULL;
    return q;
}

struct node* new_node(int i)
{// creates a new node
    struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp->item = i;
    temp->next = NULL;
    return temp;
}