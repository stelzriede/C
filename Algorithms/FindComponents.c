// Jacob Stelzriede - 1436063
// CMPE 101 - UC Santa Cruz
// Fall 2018
// Programming Assignment 5: Graph Theory in C
//
// 
//
//
//
//

#include<stdio.h>
#include<stdlib.h>
#include"List.h"
#include"Graph.h"
#include <string.h>

#define MAX_LEN 160

int main(int argc, char *argv[])
{	
	FILE *fileIn, *fileOut;
	char line[MAX_LEN];

	if(argc != 3)
	{
		printf("Invalid argument(s) on input.");
		exit(1);
	}

	// Open Read Write files. 
	fileIn = fopen(argv[1], "r");
	fileOut = fopen(argv[2], "w");
	if(fileIn==NULL)
	{
		printf("Unable to open file %s for reading\n", argv[1]);
	  	exit(1);
	}
	if(fileOut==NULL)
	{
	  	printf("Unable to open file %s for writing\n", argv[2]);
	  	exit(1);
	}

	// parse input file and create graph using addArc():
	int n, vertA, vertB;
	rewind(fileIn);
	fgets(line, MAX_LEN, fileIn);
	sscanf(line, "%d", &n);
	Graph G = newGraph(n);
	while(fgets(line, MAX_LEN, fileIn) != NULL)
	{
		sscanf(line, "%d %d", &vertA, &vertB);
		if(vertA == 0 && vertB == 0)
		{
			break;
		}
		addArc(G, vertA, vertB);
	}
	fprintf(fileOut, "Adjacency list representation of G: ");
	fprintf(fileOut, "\r\n");
	printGraph(fileOut, G);
	fprintf(fileOut, "\r\n");

	// part 2
	List S = newList(); //process que
	for(int i = 1; i<= getOrder(G); i++)
	{
		append(S,i);
	}
	//first call
	DFS(G,S);
	Graph gT = transpose(G);
	//second call
	DFS(gT,S);
	/*
   for(int i=1; i<=n; i++){
     fprintf(stdout, "%d: %2d %2d %2d\n", i, getDiscover(G, i), getFinish(G, i), getParent(G, i));
   }
   for(int i=1; i<=n; i++){
     fprintf(stdout, "%d: %2d %2d %2d\n", i, getDiscover(gT, i), getFinish(gT, i), getParent(gT, i));
   }
	*/
	//strongly connected components pg 617
	int strong = 0;
	for(int i = 1; i<= getOrder(gT); i++)
	{
		if(getParent(gT, i) == NIL)
		{
			strong++;
			//insertBefore(S,-1); //internal flag
		}
	}
	fprintf(fileOut,"G contains %d strongly connected components: ", strong);
	fprintf(fileOut,"\r\n");
	//top order (backwards)
	List tempL = newList();
	moveBack(S);

	int x = 1;
	while(index(S) != -1)
	{
		prepend(tempL, get(S));
		//moveFront(tempL);
		if(getParent(gT, get(S)) == NIL)
		{
			fprintf(fileOut, "Component %d: ", x);
			printList(fileOut, tempL);
			fprintf(fileOut,"\r\n");
			//delete(tempL);
			clear(tempL);
			x++;
		}
		movePrev(S);
	}

	freeGraph(&G);
	freeGraph(&gT);
	freeList(&S);
	freeList(&tempL);
	fclose(fileIn);
	fclose(fileOut);


	//return(0);

}