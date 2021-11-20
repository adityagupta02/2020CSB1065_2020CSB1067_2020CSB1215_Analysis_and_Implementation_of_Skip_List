#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
 
int MAX_LEVEL;      // Maximum Level of the Skip List 
 
typedef struct node {
    int data;
    struct node **next;
} node;
 
typedef struct skiplist {
    int level;      // current level of the skip list
    struct node *header;
} skiplist;

void create_skipList(skiplist* sl);
int rand_lvl();
int insert_element(skiplist* sl, int data);
int skiplist_search(skiplist *sl, int data);
int delete_element(skiplist *sl, int data);
void skiplist_display(skiplist* sl);

int main(void){    
    skiplist* sl;
    int max_num;
    printf("Enter the maximum number of elements in the list: ");
    scanf("%d",&max_num);
    MAX_LEVEL = log(max_num)/log(2);            // Maximum Level of Skip List = log2(Number of elements in the List)
    create_skipList(sl);
    int k, n;
    while (1)
    {
        printf("\n-----------------------\n");
        printf("\nOperations on Skip list\n");
        printf("\n-----------------------\n");
        printf("1.Insert Element\n");
        printf("2.Search Element\n");
        printf("3.Delete Element\n");
        printf("4.Display List \n");
        printf("5.Exit \n");
        printf("Enter your choice : ");
        scanf("%d",&k);
        switch(k)
        {
        case 1:
            printf("Enter the element to be inserted in the list: ");
            scanf("%d",&n);
            insert_element(sl,n);
            break;
        case 2:
            printf("Enter the element to be searched: ");
            scanf("%d",&n);
            if(skiplist_search(sl,n))
                printf("%d is present in the list\n",n);
            else
                printf("Element not present\n");
            break;
        case 3:
            printf("Enter the element to be deleted from the list: ");
            scanf("%d",&n);
            if(!skiplist_search(sl,n))
            {
                printf("Element not present\n");
                break;
            }
            delete_element(sl, n);
            printf("Element Deleted\n");
            break;
        
        case 4:
            printf("The elements in the Skip List are: \n");
            skiplist_display(sl);
            break;
        case 5:
            exit(1);
            break;
        default:
            printf("Enter a valid input.\n");
        }
    }
    return 0;
}

void create_skipList(skiplist* sl) {
    node *header = (node *) malloc(sizeof(struct node));
    sl->header = header;
    header->data = -1;
    header->next = (node **) malloc(sizeof(node*) * (MAX_LEVEL + 1));  // Allocating memeory for pointer to nodes of different levels
    for (int i = 0; i <= MAX_LEVEL; i++) {
        header->next[i] = sl->header;               // Inialising all nodes to header (can also be initialised to NULL)
    } 
    sl->level = 1;      // Updating current level to 1
}
 
int rand_lvl() {        // Function to generate random level for a node
    int level = 1;
    while (rand() < RAND_MAX / 2 && level <= MAX_LEVEL)     // Probability for incresing level is 0.5
        level++;
    return level;
}
 
int insert_element(skiplist* sl, int data) {
    node *x = sl->header;    
    node *update[MAX_LEVEL + 1];            // Creating an array to store address of previous nodes
    for (int i = sl->level; i >= 1; i--) {      // Starting from highest level of skip list 
        while (x->next[i]->data < data)
            x = x->next[i];
        update[i] = x;
    }
    x = x->next[1];
 
    if (data == x->data) {
        return 0;
    } 
    else {
        int level = rand_lvl();
        // Checking if random level generated is greater than skiplist's current level 
        // If so, initializing update value with pointer to header for further use
        // and updating sl->level to level.
        if (level > sl->level) {        
            for (int i = sl->level + 1; i <= level; i++) {
                update[i] = sl->header;
            }
            sl->level = level;
        }
 
        x = (node *) malloc(sizeof(node));      // Allocating memory to new node
        x->data = data;                         // Updating data of new node
        x->next = (node **) malloc(sizeof(node*)*(level + 1));
        for (int i = 1; i <= level; i++) {
            x->next[i] = update[i]->next[i];
            update[i]->next[i] = x;
        }
    }
    return 0;
}

// Function to search elelment in SkipList 
int skiplist_search(skiplist *sl, int data) {
    node *x = sl->header;

    /*  Starting from current maximum level of skip list and moving the pointer forward while data 
        is greater than data of node next to the current node(named x). Otherwise inserting x in update and 
        moving one level down and continuing search
    */
    for (int i = sl->level; i >= 1; i--) {
        while (x->next[i]->data < data)
            x = x->next[i];
    }
    if (x->next[1]->data == data) {
        return 1;
    } 
    else {
        return 0;
    }
    return 0;
}

// Function to delete elements from SkipList
int delete_element(skiplist *sl, int data) {
    node *update [MAX_LEVEL + 1];
    node *x = sl->header;
    /*  Starting from current maximum level of skip list and moving the pointer forward while data 
        is greater than data of node next to the current node(named x). Otherwise inserting x in update and 
        moving one level down and continuing search
    */
    for (int i = sl->level; i >= 1; i--) {
        while (x->next[i]->data < data)
            x = x->next[i];
        update[i] = x;
    } 
    x = x->next[1];
    if (x->data == data) {
        for (int i = 1; i <= sl->level; i++) {
            // If at level i, next node is not target node, no need to move ahead
            if (update[i]->next[i] != x)
                break;
            update[i]->next[i] = x->next[i];        
        }      
        if (x) {
            free(x->next);
            free(x);
        } 
        // Removing levels with no elements
        while (sl->level > 1 && sl->header->next[sl->level]== sl->header)
            sl->level--;
        return 0;
    }
    return 1;
}

// Function to display elements of SkipList
void skiplist_display(skiplist* sl) {
   
    for(int i=1; i<=MAX_LEVEL; i++){    // Starting from level 1 and printing elements at each level 
         node *x = sl->header;
        printf("LEVEL %d: ",i);
        while (x && x->next[i] != sl->header) {
            printf("%d->", x->next[i]->data);
            x = x->next[i];
        }
        printf("NULL\n");
    }
}