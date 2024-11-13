#include <stdio.h>
#include <stdlib.h>
#define FALSE 0
#define TRUE 1

struct sNode {
    int data;
    struct sNode *next;
};

typedef struct sNode tNode;
typedef tNode *Node;

void insert(Node *start, int data);
void bubbleSort(Node start);
void swap(Node a, Node b);
void print(Node start);

int main() {
    int initArr[] = {100, 50, 15, 1, 100}; /* Initial value in list, min 5 */
    Node start = NULL;
    for(int i = 0; i < 5; i++) {
        insert(&start, initArr[i]);
    }
    
    print(start);
    printf("Input: ");
    int input;
    scanf("%d", &input);
    insert(&start, input);
    bubbleSort(start);
    print(start);
    return 0;
}

void insert(Node *start, int data) {
    Node temp = malloc(sizeof(tNode));
    temp->data = data;
    temp->next = *start;
    *start = temp;
}

void print(Node start) {
    Node temp = start;
    while(temp != NULL) {
        if(temp->next != NULL) {
            printf("%d, ", temp->data);
        } else {
            printf("%d", temp->data);
        }
        temp = temp->next;
    }
    printf("\n");
}

/* In this case we use Bubble Sort for the sorting algorithm */
void bubbleSort(Node start) {
    int swapped = 0;
    Node temp;
    Node temp1 = NULL;
    
    if(start == NULL) return;
    
    do {
        swapped = 0;
        temp = start;
        
        while(temp->next != temp1) {
            if(temp->data > temp->next->data) {
                swap(temp, temp->next);
                swapped = 1;
            }
            temp = temp->next;
        }
        temp1 = temp;
    } while(swapped);
}

void swap(Node a, Node b) {
    int temp = a->data;
    a->data = b->data;
    b->data = temp;
}
