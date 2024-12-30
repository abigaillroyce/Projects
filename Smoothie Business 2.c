/* COP 3502C Assignment 2
This program is written by: Abigail Royce */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_LINES 12

// Define structs for Customer, Node, and Queue
typedef struct Customer {
    char name[16];
    int smoothies;
    int line;
    int time; // Changed from unsigned int to int for consistency
} Customer;

typedef struct Node {
    Customer *customer;
    struct Node *next;
} Node;

typedef struct Queue {
    Node *front;
    Node *back;
} Queue;

// Function prototypes
void initQueue(Queue *queue); // initializes a queue
void enqueue(Queue *queue, Customer *customer); // adds a customer to a queue to the end 
Customer *dequeue(Queue *queue); // gives customer from front of the queue
Customer *peek(Queue *queue); // knows the next customer 
int isEmpty(Queue *queue); // checks if queue is empty
Customer *createCustomer(int time, int line, char *name, int smoothies); // creates a new customer 
int findNextCustomer(Queue queues[], int currentTime); // finds the next customer based on the time they have been present

int main() {
    int c;
    scanf("%d", &c); // reads number of test cases from input

    while (c > 0) {
        int n;
        scanf("%d", &n); // takes input for customers
        c--; // decrease the value of c after checking the value of it

        Queue queues[MAX_LINES];
        for (int i = 0; i < MAX_LINES; i++) {
            initQueue(&queues[i]); // loops and initializes value for each queue 
        }

        // Read customer data and enqueue them
        for (int i = 0; i < n; i++) {
            int time, lineNo, smoothiesNo;
            char name[16];
            scanf("%d %d %s %d", &time, &lineNo, name, &smoothiesNo);
            Customer *customer = createCustomer(time, lineNo, name, smoothiesNo); // creates a new customer 
            enqueue(&queues[lineNo - 1], customer); // adds customer to a queue
        }

        int currentTime = 0;

        // Process customers until all queues are empty
        while (1) {
            int nextInLine = findNextCustomer(queues, currentTime); // find customer 
            if (nextInLine == -1) { // if there is no customer then exit 
                break;
            }

            Customer *customer = dequeue(&queues[nextInLine]); // give the next customer that is in the queue
            if (customer->time > currentTime) {
                currentTime = customer->time;
            }
            int checkoutTime = currentTime + 30 + (5 * customer->smoothies); // calculate time needed for checkout
            printf("At time %d, %s left the counter from line %d.\n", checkoutTime, customer->name, customer->line);
            currentTime = checkoutTime; // current time is set checkout time 
            free(customer);
        }
    }

    return 0;
}

// Function to initialize a queue
void initQueue(Queue *queue) {
    queue->front = NULL; // sets front to NULL
    queue->back = NULL; // sets back to NULL
}

// Function to add a customer to the end of the queue
void enqueue(Queue *queue, Customer *customer) {
    Node *newNode = (Node *)malloc(sizeof(Node)); // creates and allocates memory for a new node 
    newNode->customer = customer; // gives a value the customer 
    newNode->next = NULL; // sets next to the newnode
    if (queue->back != NULL) {
        queue->back->next = newNode; // link node to the back if it's not empty 
    }
    queue->back = newNode; // updates the back to the new node 
    if (queue->front == NULL) {
        queue->front = newNode; // front node is the new node if queue is empty 
    }
}

// Function to remove and return the customer from the front of the queue
Customer *dequeue(Queue *queue) {
    if (queue->front == NULL) {
        return NULL;
    }
    
    Node *oldFront = queue->front; // stores the current front node
    Customer *customer = oldFront->customer;
    queue->front = oldFront->next; // update front pointer to next
    if (queue->front == NULL) {
        queue->back = NULL; // update to NULL if queue becomes empty 
    }
    free(oldFront); // frees memory for the old front node 
    return customer;
}

// Function to return the next customer without removing them from the queue
Customer *peek(Queue *queue) {
    if (queue->front == NULL) {
        return NULL;
    }
    return queue->front->customer;
}

// Function to check if the queue is empty
int isEmpty(Queue *queue) {
    return queue->front == NULL; // returns true if queue is empty
}

// Function to find the index of the next customer based on their arrival time
int findNextCustomer(Queue queues[], int currentTime) {
    int minSmoothies = 101;
    int selectedLine = -1;
    int selectedTime = 99999999;

    for (int i = 0; i < MAX_LINES; i++) {
        if (!isEmpty(&queues[i])) {
            Customer *customer = peek(&queues[i]); // peek at the next customer 
            if (customer->time <= currentTime) { // if customer has come before or at present time
                if (customer->smoothies < minSmoothies || (customer->smoothies == minSmoothies && customer->time < selectedTime)) { // if smoothies are lesser than present or have arrived early
                    minSmoothies = customer->smoothies; // update the minimum number of smoothies 
                    selectedLine = i; // update the line
                    selectedTime = customer->time; // update the time
                }
            }
        }
    }

    if (selectedLine == -1) {
        for (int i = 0; i < MAX_LINES; i++) {
            if (!isEmpty(&queues[i])) {
                Customer *customer = peek(&queues[i]); // peek at the customer at the front 
                if (customer->time < selectedTime) { // find the customer that arrived first 
                    selectedTime = customer->time; // update time 
                    selectedLine = i;
                }
            }
        }
    }

    return selectedLine; // return the index
}

// Function to create a new customer
Customer *createCustomer(int time, int line, char *name, int smoothies) {
    Customer *newCustomer = (Customer *)malloc(sizeof(Customer)); // allocates memory for a new customer
    strcpy(newCustomer->name, name); // sets customer name
    newCustomer->line = line; // sets customer's line number 
    newCustomer->time = time; // sets customer's reaching time
    newCustomer->smoothies = smoothies; // sets the number of smoothies 
    return newCustomer;
}
