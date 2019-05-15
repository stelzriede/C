// Jacob Stelzriede - 1436063
// CMPE 101 - UC Santa Cruz
// Fall 2018
// Programming Assignment 2: Abstract Data Types in C
//
// Header File for List.c
//
//
//
//


typedef struct ListObj* List;
typedef struct NodeObj* Node;

// Constructors Deconstructors
List newList(void);
void freeList(List* pL);
void freeNode(Node* pN);

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ Access Functions ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
//
int length(List L);

//
int index(List L);

//
int front(List L);

//
int back(List L);

//
int get(List L);

//
int equals(List A, List B);

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ Manipulation Functions ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
//
void clear(List L);

//
void moveFront(List L);

//
void moveBack(List L);

//
void movePrev(List L);

//
void moveNext(List L);

//
void prepend(List L, int data);

//
void append(List L, int data);

//
void insertBefore(List L, int data);

//
void insertAfter(List L, int data);

//
void deleteFront(List L);

//
void deleteBack(List L);

//
void delete(List L);

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ Other Functions ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
//
void printList(FILE* out, List L);

//
List copyList(List L);