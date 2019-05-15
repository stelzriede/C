// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "Stack.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries

/*************************************************************************************************/
void StackInit(struct Stack *stack)
{
    stack->currentItemIndex = -1;
    stack->initialized = TRUE;
}

/*************************************************************************************************/
int StackPush(struct Stack *stack, float value)
{
    if ((StackIsFull(stack) != TRUE) && (stack->initialized == TRUE)) {
        ++(stack->currentItemIndex);
        stack->stackItems[stack->currentItemIndex] = value;
        return SUCCESS;
    } else
        return STANDARD_ERROR;
}

/*************************************************************************************************/
int StackPop(struct Stack *stack, float *value)
{
    if ((StackIsEmpty(stack) != TRUE) && (stack->initialized == TRUE)) {
        *value = stack->stackItems[stack->currentItemIndex];
        --(stack->currentItemIndex);
        return SUCCESS;
    } else
        return STANDARD_ERROR;
}

/*************************************************************************************************/
int StackIsEmpty(const struct Stack *stack)
{
    if ((stack->currentItemIndex == -1) && (stack->initialized == TRUE))
        return TRUE; //TRUE
    else
        return FALSE; //FALSE
}

/*************************************************************************************************/
int StackIsFull(const struct Stack *stack)
{
    if ((stack->currentItemIndex == STACK_SIZE) && (stack->initialized == TRUE))
        return TRUE; //TRUE
    else
        return FALSE; //FALSE
}

/*************************************************************************************************/
int StackGetSize(const struct Stack *stack)
{
    int temp = 0;
    if (stack->initialized == FALSE)
        return SIZE_ERROR;
    else if ((stack->initialized == TRUE) && (stack->currentItemIndex == -1))
        return 0;
    else
        temp = (stack->currentItemIndex) + 1;
    return temp;
}

/*************************************************************************************************/