#ifndef LINKED_LIST_QUEUE
#define LINKED_LIST_QUEUE
#include <stdlib.h>

struct node
{
    int item;
    struct node *next;
};

struct queue { struct node *front, *rear; };

struct queue *create_queue()
{ // initialises empty queue
    struct queue *q = (struct queue *)malloc(sizeof(struct queue));
    q->front = q->rear = NULL;
    return q;
}

struct node *new_node(int i)
{ // creates a new node
    struct node *temp = (struct node *)malloc(sizeof(struct node));
    temp->item = i;
    temp->next = NULL;
    return temp;
}

void dequeue(struct queue *q)
{ // pop first item from queue
    if (q->front == NULL) return;
    struct node *temp = q->front;         // assign head to temp
    q->front = q->front->next;            // assign head to second item in queue
    if (q->front == NULL) q->rear = NULL; // If front becomes NULL, then change rear to NULL
    free(temp);                           // free old head
}

void enqueue(struct queue *q, int i)
{ // push i to queue
    struct node *temp = new_node(i); // create new node
    if (q->rear == NULL)
    { // If queue is empty, then new node is front and rear both
        q->front = q->rear = temp;
        return;
    }
    q->rear->next = temp; // Add the new node at the end of queue and change rear
    q->rear = temp;
}

#endif