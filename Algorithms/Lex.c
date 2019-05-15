// Jacob Stelzriede - 1436063
// CMPE 101 - UC Santa Cruz
// Fall 2018
// Programming Assignment 2: Abstract Data Types in C
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

#define MAX_LEN 160

int main(int argc, char *argv[])
{
	// Files:
	FILE *fileIn;
	FILE *fileOut;

	int count = 0;
	int placer = 0;
	char line[MAX_LEN];
	char** lineArray;
	//char* lineArray[MAX_LEN];

	fileIn = fopen(argv[1], "r");
	fileOut = fopen(argv[2], "w");

	// Error Check:
	if(argc != 3)
	{
		printf("Error: Invalid argument length.");
		exit(1);
	}
	if(fileIn == NULL)
	{
		printf("Error: Argument file(s) return NULL.");
		exit(1);
	}

	//
	while(fgets(line,MAX_LEN,fileIn) != NULL)
	{
		count++;
	}
	rewind(fileIn);

	lineArray = malloc((count) *sizeof(char*));

	for(int i = 0; i <= count; i++)
	{
		lineArray[i] = malloc((MAX_LEN)*sizeof(char));
	}

	while(fgets(line, sizeof(line), fileIn) != NULL)
	{
		strcpy(lineArray[placer], line);
		placer++;
	}

	// InsertionSort
	List sortList = newList();
	append(sortList, 0);
	for(int i = 1; i <= count; i++)
	{
		if(strcmp(lineArray[i], lineArray[back(sortList)]) > 0 )
		{
			append(sortList,i);
			continue;
		}
		else if(strcmp(lineArray[i], lineArray[front(sortList)]) < 0)
		{
			prepend(sortList,i);
			continue;
		}
		moveFront(sortList);
		while(index(sortList) != -1)
		{
			if(strcmp(lineArray[i], lineArray[get(sortList)]) < 0)
			{
				insertBefore(sortList,i);
				break;
			}
			else
			{
				moveNext(sortList);
			}
		}
	} // end of insertionsort

	moveFront(sortList);
	//printList(fileOut, sortList);
	while(index(sortList) >= 0)
	{
		fprintf(fileOut, "%s", lineArray[get(sortList)]);
		moveNext(sortList);
	}

	fclose(fileIn);
	fclose(fileOut);
	freeList(&sortList);
	//free(lineArray);
	for(int i = 0; i <= count; i++)
	{
		free(lineArray[i]);
	}

	return 0;
}