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
#include "Graph.h"
#include <string.h>

#define MAX_LEN 160

int main(int argc, char *argv[])
{	
	FILE *fileIn, *fileOut;
	char line[MAX_LEN];
	int vertA = 0;
	int vertB = 0;

	if(argc != 3)
	{
		fprintf("Invalid argument(s) on input.");
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
	// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ Part1 ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//
	//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ //
	// Parse input for Graph.
	int n = 0;
	//rewind(fileIn);
	fgets(line, MAX_LEN, fileIn);
	sscanf(line, "%d", &n);
	Graph G = newGraph(n);
	while(fgets(line, MAX_LEN, fileIn) != NULL)
	{
		sscanf(line, "%d %d", &vertA, &vertB);
		//sscanf(fileIn, "%d", &vertB);
		if(vertA == 0 && vertB == 0)
		{
			break;
		}
		addEdge(G, vertA, vertB);
		//printf("%d ", vertA);
		//printf("%d \n", vertB);
	}
	printGraph(fileOut, G);
	fprintf(fileOut, "\n");


	// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ Part2 ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~//
	//~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ //
	// Parse input again, 

	while(fgets(line, MAX_LEN, fileIn) != NULL)
	{
		sscanf(line, "%d %d", &vertA, &vertB);
		//sscanf(fileIn, "%d", &vertB);
		if(vertA == 0 && vertB == 0)
		{
			break;
		}
		List P = newList();
		BFS(G, vertA);
		getPath(P,G,vertB);
		if(getDist(G,vertB) != INF)
		{
			fprintf(fileOut, "The distance from %d to %d is %d. \n", vertA, vertB, length(P)-1);
			fprintf(fileOut, "A shortest %d - %d path is: ", vertA, vertB);
			printList(fileOut, P);
			fprintf(fileOut, "\r\n");
		}
		else
		{
			fprintf(fileOut, "The distance from %d to %d is infinity. \n", vertA, vertB);
			fprintf(fileOut, "No %d - %d exists. \r\n", vertA, vertB);
		}
		freeList(&P);
	}

	freeGraph(&G);

	/* close files */
	fclose(fileIn);
	fclose(fileOut);

	return(0);
}
// Test code area
/*



*/
