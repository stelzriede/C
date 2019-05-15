// Jacob Stelzriede

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "Morse.h"
#include "Buttons.h"
#include "Tree.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

//
static int timer100hz = 0;
static uint8_t buttonEvent;
static Node *characterTree;
static Node *headNode;

//
const int MORSE_EVENT_LENGTH_UP_INTER_WORD = 200;

//
static enum {
        WAITING = 1,
        DOT = 2,
        DASH = 3,
        INTER_LETTER = 4
    } state = WAITING;
    
//
//**************************************************************************************************
//**************************************************************************************************
int MorseInit(void){
    //ButtonsInit();
    char asciiValues[]={'\0', 'E', 'I', 'S', 'H', '5', '4', 'V', '\0', '3', 'U', 'F', '\0', '\0', '\0','\0',
                        '2', 'A', 'R', 'L', '\0', '\0', '\0', '\0', '\0', 'W', 'P', '\0', '\0', 'J', '\0', '1',
                        'T', 'N', 'D', 'B', '6', '\0', 'X', '\0', '\0', 'K', 'C', '\0', '\0', 'Y', '\0', '\0',
                        'M', 'G', 'Z', '7','\0', 'Q', '\0', '\0', 'O', '\0', '\0', '8', '\0', '9', '0'};
    characterTree = TreeCreate(6, asciiValues);
    if(characterTree != NULL){
        headNode = characterTree;  //save the head address
        return SUCCESS;
    }// end of if
    else{
        return STANDARD_ERROR;
    }//end of else
}

//**************************************************************************************************
//**************************************************************************************************
char MorseDecode(MorseChar in){
    switch(in){
//***************************************************************MORSE_CHAR_DOT
    case MORSE_CHAR_DOT:
        if(characterTree->leftChild == NULL){
            return STANDARD_ERROR; //too many characters
        }
        else{
            characterTree = characterTree->leftChild;
            return SUCCESS;
        }
        break;
//***************************************************************MORSE_CHAR_DASH
    case MORSE_CHAR_DASH:
        if(characterTree->rightChild == NULL){
            return STANDARD_ERROR; //too many characters
        }
        else{
            characterTree = characterTree->rightChild;
            return SUCCESS;
        }
        break;
//***************************************************************MORSE_CHAR_END_OF_CHAR
    case MORSE_CHAR_END_OF_CHAR:
        return characterTree->data;
        characterTree = headNode;
        break;
//***************************************************************MORSE_CHAR_DECODE_RESET
    case MORSE_CHAR_DECODE_RESET:
        characterTree = headNode;
        return SUCCESS;
        break;
//***************************************************************default
    default:
        return STANDARD_ERROR;
    }
}//end of MorseDecode()

//**************************************************************************************************
//**************************************************************************************************
MorseEvent MorseCheckEvents(void){
    buttonEvent = ButtonsCheckEvents();
    timer100hz ++;

        switch (state) {
//*********************************************************************WAITING Case**********
        case WAITING:
            if(buttonEvent == BUTTON_EVENT_4DOWN){
                timer100hz = 0;
                state = DOT;
                buttonEvent = BUTTON_EVENT_NONE;
            }
            break;
//***********************************************************************DOT Case************
        case DOT:
            if(timer100hz >= MORSE_EVENT_LENGTH_DOWN_DASH){
                state = DASH;
            }
            if(buttonEvent == BUTTON_EVENT_4UP){
                timer100hz = 0;
                state = INTER_LETTER;
                printf("dot event\n");
                buttonEvent = BUTTON_EVENT_NONE;
                return MORSE_EVENT_DOT;
            }
            break;
//***********************************************************************DASH Case************
        case DASH:
            if(buttonEvent == BUTTON_EVENT_4UP){
                timer100hz = 0;
                state = INTER_LETTER;
                printf("dash event\n");
                buttonEvent = BUTTON_EVENT_NONE;
                return MORSE_EVENT_DASH;
            }
            break;
//**************************************************************INTER_LETTER Case*************
        case INTER_LETTER:
            if(timer100hz >= MORSE_EVENT_LENGTH_UP_INTER_WORD){
                state = WAITING;
                timer100hz = 0;
                printf("inter word event\n");
                buttonEvent = BUTTON_EVENT_NONE;
                return MORSE_EVENT_INTER_WORD;
            }
            if(buttonEvent == BUTTON_EVENT_4DOWN &&
                    timer100hz < MORSE_EVENT_LENGTH_UP_INTER_WORD){
                timer100hz = 0;
                state = DOT;
                printf("inter letter event\n");
                buttonEvent = BUTTON_EVENT_NONE;
                return MORSE_EVENT_INTER_LETTER;
            }
            break;
        default:
            printf("none event\n");
            return MORSE_EVENT_NONE;
        }//end of switch statement
}//end of MorseEvent()
//**************************************************************************************************
//**************************************************************************************************