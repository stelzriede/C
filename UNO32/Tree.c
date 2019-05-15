// Jacob Stelzriede

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "Tree.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>


Node *TreeCreate(int level, const char *data){

    Node *newNode = malloc(sizeof(Node));
   
    if(!newNode){
        printf("malloc fail\n");
        return NULL;
    }
    if(level >1){// if not on level 1.
        printf("traversing tree\n");
        newNode->leftChild = TreeCreate((level - 1), data+1);
        newNode->rightChild = TreeCreate((level - 1), data + (int)pow(2, level -1));
    }else{
        printf("assigning children\n");
        newNode->leftChild = NULL;
        newNode->rightChild = NULL;
    }
    newNode->data=*data;
    printf("returning tree\n");
    return newNode;
}//end of TreeCreate()

