// Jacob Stelzriede

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "LinkedList.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

/**************************************************************************************************/
ListItem *LinkedListNew(char *data)
{
    struct ListItem *newListPointer = (struct ListItem*) malloc(sizeof (struct ListItem));
    if (newListPointer == NULL) {
        return NULL;
    }
    newListPointer->data = data; //setting newListPointer elements
    newListPointer->previousItem = NULL;
    newListPointer->nextItem = NULL;
    return newListPointer;
};

/**************************************************************************************************/
ListItem *LinkedListCreateAfter(ListItem *item, char *data)
{
    if (item == NULL) {
        return NULL;
    }
    ListItem *newItem = LinkedListNew(data);
    ListItem* followingItem = item->nextItem;
    newItem->nextItem = followingItem;
    newItem->previousItem = item;
    item->nextItem = newItem;
    if (followingItem != NULL) {
        followingItem->previousItem = newItem;
    }
    return newItem;

};

/**************************************************************************************************/
int LinkedListPrint(ListItem *list)
{
    if (list == NULL) {
        return STANDARD_ERROR; //check for empty list
    }
    ListItem *tempItem = LinkedListGetFirst(list);
    while (tempItem != NULL) { //cycle through the list
        printf("%s, ", tempItem->data); //print current data item
        tempItem = tempItem->nextItem; //move to next items
    }
    printf("\n");
    return SUCCESS;
};

/**************************************************************************************************/
int LinkedListSwapData(ListItem *firstItem, ListItem *secondItem)
{
    if (firstItem->data == NULL || secondItem->data == NULL) {
        return STANDARD_ERROR;
    }
    char *temp = firstItem->data; // save data1
    firstItem->data = secondItem->data; // move data2 to data1
    secondItem->data = temp; // move data1 to data2
    return SUCCESS;
}

/**************************************************************************************************/
int LinkedListSize(ListItem *list)
{
    int size = 0;
    if (list == NULL) {
        return size; //check for empty list
    }
    ListItem *current = list;
    LinkedListGetFirst(current);
    while (current != NULL) { //cycle through the list
        ++size; //increment size
        current = current->nextItem; //move to next items
    }
    return size;
}

/**************************************************************************************************/
ListItem *LinkedListGetFirst(ListItem *list)
{
    if (list == NULL) {
        return STANDARD_ERROR;
    }
    while (list->previousItem != NULL) {
        list = list->previousItem;
    }
    return list;
}

/**************************************************************************************************/
char *LinkedListRemove(ListItem *item)
{
    if (item == NULL) {
        return item->data;
    }
    char *rtnPointer = item->data; //I think this line is killing me.
    ListItem *previous = item->previousItem;
    ListItem *next = item->nextItem;

    if(previous!=NULL){
        previous->nextItem = item->nextItem;
    }
    if(next!=NULL){
        next->previousItem = item->previousItem;
    }

    free(item);
    return rtnPointer;
}

/**************************************************************************************************/

