/* COP 3502C Assignment 5
This program is written by: Abigail Royce */
/*I want to be considered for the bonus and I agree that any of my submissions
after July 23rd will not be considered for grading regardless of my score on the most recent
submission before July 23rd */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 26  // max length for name

typedef struct Node {
    int points;  // stores points
    struct Node *left;  // pointer to left child node
    struct Node *right;  // pointer to right child node
    char name[NAME_LEN];  // name of the node
} Node;

Node* *nodeList = NULL;  // pointer to array of node pointers
int nodeIndex = 0;  // index to keep track of node position
int nodeCapacity = 0;  // current capacity of nodeList

void height_balance(Node* root);
void freeTree(Node* root);
Node* createNode(char* name, int points);
Node* delete(Node* root, char* name, int* have);
Node* search(Node* root, char* name, int* depth);
int height(Node* root);
Node* findMax(Node* node);
Node* insert(Node* root, char* name, int points, int* depth);


int main() {
    Node* root = NULL;  // initialize the root of the tree
    int n;  // variable to store the number of commands

    
    if (scanf("%d", &n) != 1) { // read number of commands
        fprintf(stderr, "Error reading the number of commands\n");
        exit(EXIT_FAILURE);
    }

    // Allocate initial memory for node list
    nodeCapacity = 1000;  // Set an initial capacity
    nodeList = malloc(nodeCapacity * sizeof(Node*));
    if (nodeList == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for command input
    char *command = malloc(20 * sizeof(char));
    if (command == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Process each command
    for (int i = 0; i < n; i++) {
        int depth;  // variable to store depth
        char name[NAME_LEN];
        int points;  // variable to store points
        int have;  // variable to store the have indicator

        
        if (scanf("%s", command) != 1) { //read from the command in input
          fprintf(stderr, "Error reading command\n");
          free(command);
          free(nodeList);
          exit(EXIT_FAILURE);
        }

        if (strcmp(command, "add") == 0) {  // check if the command is addition
            if (scanf("%s %d", name, &points) != 2) {
                fprintf(stderr, "Error reading name and points\n");
                free(command);
                free(nodeList);  
                exit(EXIT_FAILURE);
            }
            depth = 0;  // reset depth
            root = insert(root, name, points, &depth);  // insert into the tree
            Node* node = search(root, name, &depth);  // search for the node and update depth
            
        if (node) {
            printf("%s %d %d\n", name, node->points, depth);  // print node details
        } else {
            printf("%s not found\n", name);  // node not found
        }

        } else if (strcmp(command, "sub") == 0) {  // check if the command is subtraction
            if (scanf("%s %d", name, &points) != 2) {
                fprintf(stderr, "Error reading name and points\n");
                free(command);
                free(nodeList);  
                exit(EXIT_FAILURE);
            }
            depth = 0;  // reset depth
            Node* node = search(root, name, &depth);  // search for the node
            if (node) {
                node->points -= points;  // subtract points from node
                if (node->points < 0) node->points = 0;  // ensure points are non-negative
                printf("%s %d %d\n", name, node->points, depth);  // print updated node details
            } else {
                printf("%s not found\n", name);  // node not found
            }
        } else if (strcmp(command, "del") == 0) {  // check if the command is deletion
            if (scanf("%s", name) != 1) {
                fprintf(stderr, "Error reading name\n");
                free(command);
                free(nodeList);  
                exit(EXIT_FAILURE);
            }
            have = 0;  // reset have indicator
            root = delete(root, name, &have);  // delete from the tree
            if (have) {
                printf("%s deleted\n", name);  
            } else {
                printf("%s not found\n", name);  
            }
        } else if (strcmp(command, "search") == 0) {  // check if the command is "search"
            if (scanf("%s", name) != 1) {
                fprintf(stderr, "Error reading name\n");
                free(command);
                free(nodeList);  
                exit(EXIT_FAILURE);
            }
            depth = 0;  // reset depth
            Node* node = search(root, name, &depth);  // search for the node
            if (node) {
                printf("%s %d %d\n", name, node->points, depth);  // print node details
            } else {
                printf("%s not found\n", name);  // node not found
            }
        } else if (strcmp(command, "height_balance") == 0) {  
            height_balance(root); 
        } else {
            fprintf(stderr, "Unknown command: %s\n", command); 
        }
    }

    free(command);  // free memory allocated for command
    free(nodeList);  // free memory allocated for nodeList
    freeTree(root);  // free memory allocated for the entire tree
    return 0;
}


Node* insert(Node* root, char* name, int points, int* depth) { // inserts a node into the binary search tree
    if (root == NULL) { // if current node is NULL, create a new node
        *depth = 0;
        return createNode(name, points);  // create and return new node
    }

    int cmp = strcmp(name, root->name); // compare name with current node's name
    if (cmp < 0) { // if name is less, go to left subtree
        root->left = insert(root->left, name, points, depth); // insert into left
    } else if (cmp > 0) { // if name is greater, go to right subtree
        root->right = insert(root->right, name, points, depth); 
    } else { 
        root->points += points; // add points to existing node
        *depth = 0;
    }

    if (*depth != 0) { // if depth is not zero, increment depth
        *depth += 1;
    }
    return root;
}


Node* createNode(char* name, int points) { // creates new nodes with the given name and points
    if (nodeIndex >= nodeCapacity) { // check if nodeIndex has exceeded
        nodeCapacity *= 2; 
        nodeList = realloc(nodeList, nodeCapacity * sizeof(Node*)); // reallocate memory
        if (nodeList == NULL) { 
            fprintf(stderr, "Memory reallocation failed\n");
            exit(EXIT_FAILURE);
        }
    }

    Node* newNode = (Node*)malloc(sizeof(Node)); // allocate memory for a new node
    if (newNode == NULL) { // check if memory allocation failed
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    strncpy(newNode->name, name, NAME_LEN - 1); // copy the name to the new node
    newNode->name[NAME_LEN - 1] = '\0'; // ensure name is null-terminated
    newNode->points = points; 
    newNode->left = NULL; // initialize left child to NULL
    newNode->right = NULL; // initialize right child to NULL
    nodeList[nodeIndex++] = newNode; // store the new node in the nodeList
    return newNode; 
}

Node* findMax(Node* node) { // finds the node with the maximum value
    while (node && node->right) { // keep going right if it is a right child
        node = node->right;
    }
    return node; // return the node with maximum value
}

Node* delete(Node* root, char* name, int* have) { // function to delete noode
    if (root == NULL) { // if tree is empty
        *have = 0; 
        return NULL; 
    }

    int cmp = strcmp(name, root->name); 
    if (cmp < 0) { // if name is less than root's name
        root->left = delete(root->left, name, have); // delete in left subtree
    } else if (cmp > 0) { // if name is greater than root's name
        root->right = delete(root->right, name, have); 
    } else { 
        *have = 1; // set have to 1 
        if (root->left == NULL) { 
            Node* temp = root->right; // set temp to right child
            free(root); // free current node
            return temp; 
        } else if (root->right == NULL) { // if right child is NULL
            Node* temp = root->left; 
            free(root); // free current node
            return temp; 
        }

        Node* temp = findMax(root->left); // find max node in left subtree
        strncpy(root->name, temp->name, NAME_LEN - 1); 
        root->name[NAME_LEN - 1] = '\0'; // null-terminate the string
        root->points = temp->points; 
        root->left = delete(root->left, temp->name, have); // delete the max node from left subtree
    }
    return root; // return updated root
}


Node* search(Node* root, char* name, int* depth) { // function to search for a node with a name
    if (root == NULL) { // check if the current node is NULL
        *depth = -1; 
        return NULL;
    }

    int cmp = strcmp(name, root->name); // compare the given name with the current node's name
    if (cmp < 0) {
        *depth += 1; 
        return search(root->left, name, depth); // search in the left subtree
    } else if (cmp > 0) {
        *depth += 1; 
        return search(root->right, name, depth); // search in the right subtree
    } else { 
        return root; // return the current node if name matches
    }
}

int height(Node* root) { // function to calculate the height of nodes in binary tree
    if (root == NULL) { // check if the current node is NULL
        return -1; 
    }

    int leftHeight = height(root->left); // calculate the height of the left subtree
    int rightHeight = height(root->right); // calculate the height of the right subtree

    return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1; 
}

void height_balance(Node* root) { // function to print the balance 
    if (root == NULL) { // check if the current node is NULL
        printf("left height = -1 right height = -1 balanced\n");
        return;
    }

    int leftHeight = height(root->left); // calculate the height of the left subtree
    int rightHeight = height(root->right); // calculate the height of the right subtree

    if (leftHeight == rightHeight) { // check if both heights are the same
        printf("left height = %d right height = %d balanced\n", leftHeight, rightHeight); 
    } else {
        printf("left height = %d right height = %d not balanced\n", leftHeight, rightHeight);
    }
}

void freeTree(Node* root) { //frees memory of tree
    if (root == NULL) return; 
    freeTree(root->left); // free the left subtree
    freeTree(root->right); // free the right subtree
    free(root); // free the current node
}

