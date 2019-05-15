// Jacob Stelzriede - 1436063
// CMPE 101 - UC Santa Cruz
// Fall 2018
// Programming Assignment 4: Graph Theory in C
//
// 
//
//
//
//
#include <stdio.h>
#include <stdlib.h>
#include "List.h"
#include <string.h>


int main(int argc, char *argv[])
{

	
	FILE *fileIn;
	FILE *fileOut;

	fileIn = fopen(argv[1], "r");
	fileOut = fopen(argv[2], "w");
	
	// open files for reading and writing 

	if( fileIn==NULL ){
	  printf("Unable to open file %s for reading\n", argv[1]);
	  exit(1);
	}
	if( fileOut==NULL ){
	  printf("Unable to open file %s for writing\n", argv[2]);
	  exit(1);
	}

	List A = newList();
	List B = newList();
	List C = NULL;
	int i;

	for(i=1; i<=20; i++){
	  append(A,i);
	  prepend(B,i);
	}
	
	printList(stdout,A); 
	printf("\n");
	printList(stdout,B); 
	printf("\n");


	for(moveFront(A); index(A)>=0; moveNext(A)){
	  printf("%d ", get(A));
	}
	printf("\n");
	for(moveBack(B); index(B)>=0; movePrev(B)){
	  printf("%d ", get(B));
	}
	printf("\n");

	C = copyList(A);
	printf("%s\n", equals(A,B)?"true":"false");
	printf("%s\n", equals(B,C)?"true":"false");
	printf("%s\n", equals(C,A)?"true":"false");


	moveFront(A);
	for(i=0; i<5; i++) moveNext(A); // at index 5
	insertBefore(A, -1);            // at index 6
	for(i=0; i<9; i++) moveNext(A); // at index 15
	insertAfter(A, -2);
	for(i=0; i<5; i++) movePrev(A); // at index 10
	delete(A);
	printList(stdout,A);
	printf("\n");
	printf("%d\n", length(A));
	clear(A);
	printf("%d\n", length(A));

	freeList(&A);
	freeList(&B);
	freeList(&C);

	/* close files */
	fclose(fileIn);
	fclose(fileOut);

	return(0);

}
