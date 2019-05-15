// Jacob Stelzriede - 1436063
// CMPE 101 - UC Santa Cruz
// Fall 2018
// Programming Assignment 4: Graph Theory in C
//
// 
//
//	Used Tantalo queue and stack examples as starting point
//	and for nameing conventions.
//
#include <stdio.h>
#include <stdlib.h>
#include "List.h"

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ Data Structures ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ 
//
typedef struct NodeObj
{
	int data;
	struct NodeObj* next;
	struct NodeObj* prev;
} NodeObj;

typedef NodeObj* Node;

typedef struct ListObj
{
	int length;
	int index;
	Node front;
	Node back;
	Node cursor;
} ListObj;
//typedef ListObj* List;

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ Constructors ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

// Creates a new Node
Node newNode(int data)
{
	Node N = malloc(sizeof(NodeObj));
	N -> data = data;
	N -> next = NULL;
	N -> prev = NULL;
	return(N);
}

// Deconstructor, frees heap memory and sets pointer no NULL.
void freeNode(Node* pN)
{
	if(pN != NULL && *pN != NULL)
	{
		free(*pN);
		*pN = NULL;
	}
}

// Creates a new empty list.
List newList(void)
{
	List L;
	L = malloc(sizeof(ListObj));
	L -> front = L-> back = NULL;
	L -> length = 0;
	L -> cursor = NULL;
	L -> index = -1;
	return(L);
}

// Deconstructor, Frees heap memory and sets pointer to NULL.
void freeList(List* pL)
{
	if(pL != NULL && *pL != NULL)
	{
		free(*pL);
		*pL = NULL;
	}
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ Access Functions ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
// Returns the length of the list passed.
int length(List L)
{
	return(L -> length);
}

// Returns the current index of the list passed.
int index(List L)
{
	return(L -> index);
}

// Checks for non-empty list and returns the front value.
int front(List L)
{
	if(L == NULL)
	{
		printf("List.front.Error: Invalid length.");
		exit(1);
	}
	return(L -> front -> data);
}

//
int back(List L)
{
	if(L == NULL)
	{
		printf("List.back.Error: Invalid length.");
		exit(1);
	}
	return(L -> back -> data);
}

//
int get(List L)
{
	if(L == NULL)
	{
		printf("List.get.Error: Invalid length.");
		exit(1);
	}
	return(L -> cursor -> data);
}

//
int equals(List A, List B)
{
	if(A == NULL || B == NULL)
	{
		printf("List.equals.Error: Comparing a NULL list.");
		exit(1);
	}
	if(length(A) != length(B))
	{
		return 0;
	}
	// move to front on each list.
	Node tempA = A -> front;
	Node tempB = B -> front;

	while(tempA != NULL)
	{
		if(tempA -> data != tempB -> data)
		{
			tempA = tempB = NULL;
			return(0);
		}else
		{
			tempA = tempA -> next;
			tempB = tempB -> next;
		}
	} // end of while()
	return(1);
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ Manipulation Functions ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
//	:checks for empty list, if NULL exit, if empty, do nothing:

//
void clear(List L)
{
	if(L == NULL)
	{
		printf("List.clear.Error: NULL List.");
		exit(1);
	}
	Node tempA = L -> front;
	// memory leak, free tempB
	while(tempA != NULL)
	{
		Node tempB = tempA;
		tempA = tempA -> next;
		free(tempB);
	}
	// clear list L
	L -> length = 0;
	L -> index = -1;
	L -> front = NULL;
	L -> back = NULL;
	L -> cursor = NULL;
}

// move cursor to front of list
void moveFront(List L)
{
	if(L == NULL)
	{
		printf("List.moveFront.Error: NULL List.");
		exit(1);
	}
	if(length(L) > 0)
	{
		L -> cursor = L -> front;
		L -> index = 0;
	}

}

// move cursor to back of list
void moveBack(List L)
{
	if(L == NULL)
	{
		printf("List.moveBack.Error: NULL List.");
		exit(1);
	}
	if(length(L) > 0)
	{
		L -> cursor = L -> back;
		L -> index = L -> length - 1;
	}
}

// move cursor one step to the front if cursor != NULL
void movePrev(List L)
{
	if(L == NULL)
	{
		printf("List.movePrev.Error: NULL List.");
		exit(1);
	}
	if(L -> cursor == L -> front)
	{
		L -> cursor = NULL;
		L -> index = -1;
	}else{
		L -> cursor = L -> cursor -> prev;
		L -> index = L -> index - 1;
	}
}

// move cursor one step to the back if cursor != NULL
void moveNext(List L)
{
	if(L == NULL)
	{
		printf("List.moveNext.Error: NULL List.");
		exit(1);
	}
	if(L -> cursor == L -> back)
	{
		L -> cursor = NULL;
		L -> index = -1;
	}else{
		L -> cursor = L -> cursor -> next;
		L -> index = L -> index + 1;
	}
	/*
			if(L -> index == L -> length - 1)
		{
			L -> cursor = NULL;
			L -> index = -1;
		}
		L -> cursor = L -> cursor -> next;
		L -> index = L -> index + 1;
	*/
}

//
void prepend(List L, int data)
{
	if(L == NULL)
	{
		printf("List.prepend.Error: NULL List.");
		exit(1);
	}
	Node tempA = newNode(data);
	if(L -> front == NULL)
	{
		L -> front = tempA;
		L -> back = tempA;
		L -> cursor = L -> front;
	}else{
		L -> front -> prev = tempA;
		tempA -> next = L -> front;
		L -> front = tempA;
		L -> index++;
	} L -> length++;
	/*
	tempA -> next = L -> front;
	L -> front -> prev = tempA;
	L -> front = L -> front -> prev;
	*/
}

//
void append(List L, int data)
{
	if(L == NULL)
	{
		printf("List.append.Error: NULL List.");
		exit(1);
	}
	Node tempA = newNode(data);
	if(L -> back == NULL)
	{
		L -> front = tempA;
		L -> back = tempA;
		L -> cursor = L -> back;
	}else{
		L -> back -> next = tempA;
		tempA -> prev = L -> back;
		L -> back = tempA;
		tempA -> next = NULL;
	} L -> length++;

	/*
	tempA -> prev = L -> back;
	L -> back -> next = tempA;
	L -> back = L -> back -> next;
	*/
}

//
void insertBefore(List L, int data)
{
	if(L == NULL)
	{
		printf("List.insertBefore.Error: NULL List.");
		exit(1);
	}
	if(length(L) > 0)
	{
		if(index(L) == 0)
		{
			prepend(L, data);
			return;
		}
		Node tempA = newNode(data);
		tempA -> prev = L -> cursor -> prev;	
		tempA -> next = L -> cursor;
		L -> cursor -> prev -> next = tempA;
		L -> cursor -> prev = tempA;
		L -> index++;
		L -> length++;
		freeNode(&tempA);
	}
}
/*
		tempA -> next = L -> cursor;
		tempA -> prev = L -> cursor -> prev;
		L -> cursor -> prev -> next = tempA;
		L -> cursor -> prev = tempA;
*/


//
void insertAfter(List L, int data)
{
	if(L == NULL)
	{
		printf("List.insertAfter.Error: NULL List.");
		exit(1);
	}
	if(length(L) > 0)
	{
		if(index(L) == length(L) - 1)
		{
			append(L, data);
			return;
		}
		Node tempA = newNode(data);
		L -> cursor -> next -> prev = tempA;
		tempA -> next = L -> cursor -> next;
		tempA -> prev = L -> cursor;
		L -> cursor -> next = tempA;
		L -> length++;
		freeNode(&tempA);

		/*
		tempA -> next = L -> cursor -> next;
		tempA -> prev = L -> cursor;
		L -> cursor -> next -> prev = tempA;
		L -> cursor -> next = tempA;
		*/
	}
}

// delete front field
void deleteFront(List L)
{
	if(L == NULL)
	{
		printf("List.deleteFront.Error: NULL List.");
		exit(1);
	}
	// empty list:
	if(length(L) <= 0)
	{
		printf("List.deleteFront.Error: Empty List.");
		exit(1);
	}
	// only 1 field:
	if(L -> length == 1)
	{
		//Node tempA = L -> front;
		L -> cursor = NULL;
		L -> front = L -> back = NULL;
		L -> index = -1;
	}else
	{
		Node tempA = L -> front;
		L -> front = L -> front -> next;
		L -> front -> prev = NULL;
		if(L -> cursor != NULL)
		{

			L -> index--;
		}
		freeNode(&tempA);
	}
	L -> length--;
	/*
	Node tempA = L -> front;
	L -> front = L -> front -> next;
	L -> front -> prev = NULL;
	L -> length = L -> length - 1;
	freeNode(&tempA);
	*/
}

// delete back field
void deleteBack(List L)
{
	if(L == NULL)
	{
		printf("List.deleteBack.Error: NULL List.");
		exit(1);
	}
	// empty list:
	if(length(L) == 0)
	{
		printf("List.deleteBack.Error: Empty List.");
		exit(1);
	}
	// only 1 field:
	//if(L -> cursor == L -> back)
	if(L -> length == 1)
	{
		Node tempA = L -> back;
		L -> cursor = NULL;
		L -> front = L -> back = NULL;
		//L -> length--;
		//L -> index--;
		L -> index = -1;
		freeNode(&tempA);
	}
	else
	{
		Node tempA = L -> back;
		L -> back = L -> back -> prev;
		L -> back -> next = NULL;
		if(L -> index == L -> length -1)
		{
			L -> index = -1;
			//L -> index--;
		}
		freeNode(&tempA);
	}
	L -> length--;
}

//
void delete(List L)
{
	if(L == NULL)
	{
		printf("List.delete.Error: NULL List.");
		exit(1);
	}
	// empty list:
	if(length(L) == 0)
	{
		printf("List.delete.Error: Empty List.");
		exit(1);
	}
	if(L -> cursor == L -> back)
	{
		deleteBack(L);
	}
	else if(L -> cursor == L -> front)
	{
		deleteFront(L);
	}
	else
	{
		Node tempA = L -> cursor;
		L -> cursor -> prev -> next = L -> cursor -> next;
		L -> cursor -> next -> prev = L -> cursor -> prev;
		freeNode(&tempA);
		//L -> length(L) - 1;
		L -> length--;
	}
	//L -> cursor = NULL;
	L -> index = -1;
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ Other Functions ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
//
void printList(FILE* out, List L)
{
	if(L == NULL)
	{
		printf("List.printList.Error: NULL List.");
		exit(1);
	}
	// empty list:
	if(length(L) == 0)
	{
		printf("List.printList.Error: Empty List.");
		exit(1);
	}
	Node tempA = L -> front;
	while(tempA != NULL)
	{
		fprintf(out, "%d ", tempA -> data);
		tempA = tempA -> next;
	}
	freeNode(&tempA);
}

//
List copyList(List L)
{
	/*
	if(L == NULL)
	{
		printf("List.copyList.Error: NULL List.");
		exit(1);
	}
	if(length(L) == 0)
	{
		printf("List.copyList.Error: Empty List.");
		exit(1);
	}
	*/
	// create a new list, if the list has items copy it
	// if not, return a new empty list
	List tempList = newList();
	//Node tempNode = L -> front;
	if(length(L) > 0)
	{
		Node tempNode = L -> front;
		while(tempNode != NULL)
		{
			append(tempList, tempNode -> data);
			tempNode = tempNode -> next;
		}
		freeNode(&tempNode);
	}
	return tempList;
}