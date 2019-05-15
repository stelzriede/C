// Jake Stelzriede CE13-Lab04 Spring '16
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Stack.h"

// **** Set macros and preprocessor directives ****

// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****

// If this code is used for testing, rename main to something we can call from our testing code. We
// also skip all processor configuration and initialization code.
#ifndef LAB4_TESTING

int main()
{
    BOARD_Init();
#else

int their_main(void)
{
#endif // LAB4_TESTING

    /******************************** Your custom code goes below here *******************************/
    struct Stack s;
    char userInput[61];
    char *token;
    float holder = 0;
    float op1 = 0;
    float op2 = 0;
    float *pointer = &holder;

TOP:
    memset(&userInput[61], 0, sizeof (userInput));
    //greeting
    printf("Welcome to Jake's RPN Calculator. Please \n");
    printf("enter the RPN string you wish to calculate: \n");

    //userinput
    fgets(userInput, 61, stdin);

    //initialize and 1st strtok()
    StackInit(&s);
    token = strtok(userInput, " ");

    while (token != NULL) {
        //check each token for the 4 operands, if not pass it through and let atof() convert them
        //to non-ascii zero
        if (*token == '+' && strlen(token) == 1) {
            StackPop(&s, &op1);
            StackPop(&s, &op2);
            holder = op2 + op1;
            StackPush(&s, *pointer);
            printf("Value Pop'd: %.2f \n", (double) op1);
            printf("Value Pop'd: %.2f \n", (double) op2);
            printf("Value Push'd: %.2f \n", (double) holder);

        } else if (*token == '-' && strlen(token) == 1) {
            StackPop(&s, &op1);
            StackPop(&s, &op2);
            holder = op2 - op1;
            StackPush(&s, *pointer);
            printf("Value Pop'd: %.2f \n", (double) op1);
            printf("Value Pop'd: %.2f \n", (double) op2);
            printf("Value Push'd: %.2f \n", (double) holder);

        } else if (*token == '*' && strlen(token) == 1) {
            StackPop(&s, &op1);
            StackPop(&s, &op2);
            holder = op2 * op1;
            StackPush(&s, *pointer);
            printf("Value Pop'd: %.2f \n", (double) op1);
            printf("Value Pop'd: %.2f \n", (double) op2);
            printf("Value Push'd: %.2f \n", (double) holder);

        } else if (*token == '/' && strlen(token) == 1) {
            StackPop(&s, &op1);
            StackPop(&s, &op2);
            holder = op2 / op1;
            StackPush(&s, *pointer);
            printf("Value Pop'd: %.2f \n", (double) op1);
            printf("Value Pop'd: %.2f \n", (double) op2);
            printf("Value Push'd: %.2f \n", (double) holder);

        } else {
            holder = atof(token);
            StackPush(&s, holder);
        }
        token = strtok(NULL, " ");
    }
    printf("\n");
    if (StackGetSize(&s) != 1) {
        printf("Error: Invalid RPM Calculation: +/- 1 item on stack.\n");
    } else if (StackPop(&s, &op1) == STANDARD_ERROR) {
        printf("Error: Not enough operands before operator.\n");
    } else if (StackPush(&s, *pointer) == STANDARD_ERROR) {
        printf("Error: No more room on stack.\n");
    } else {
        float result1 = 0;
        StackPop(&s, &result1);
        printf("Your input result is: %.2f ", (double) result1);
        printf("\n");
        printf("**~~~~~~~~**~~~~~~~~**~~~~~~~~**~~~~~~~~**~~~~~~~~** \n");
    }
    goto TOP;
    /*************************************************************************************************/

    // You can never return from main() in an embedded system (one that lacks an operating system).
    // This will result in the processor restarting, which is almost certainly not what you want!

    while (1);
}
