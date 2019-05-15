// heap size: 2048
// Jacob Stelzriede
// **** Include libraries here ****
// Standard C libraries
#include <stdio.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "Ascii.h"
//#include "Buttons.h"
#include "Morse.h"
#include "Oled.h"
#include "OledDriver.h"
#include "Tree.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Leds.h"  //created by me.

// **** Set any macros or preprocessor directives here ****

static uint8_t morseEvent;
static char display[20];
static Node *characterTree;
static Node *headNode;

// Specify a bit mask for setting/clearing the pin corresponding to BTN4. Should only be used when
// unit testing the Morse event checker.
#define BUTTON4_STATE_FLAG (1 << 7)

// **** Declare any data types here ****

typedef struct TimerResult {
    uint8_t event;
    uint8_t value;
} TimerResult;

// **** Define any module-level, global, or external variables here ****
static TimerResult TimerButtons;
static char lineOne[] = "                    ";
static char lineTwo[] = "                    ";
static char lineThree[] = "                    ";
static char lineFour[] = "                    ";
static char blankLine[] = "                    ";
static char *line1 = lineOne;
static char *line2 = lineTwo;
static char *line3 = lineThree;
static char *line4 = lineFour;
static char *blank = blankLine;
static char c;


// **** Declare any function prototypes here ****
static void topLineOled(void);
static void appendTopLine(char in);
static void appendBottomLine(char in);
void GenerateOled(void);
void makeHappyFace(void);

int main()
{
    BOARD_Init();

    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_16, BOARD_GetPBClock() / 16 / 100);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T2);
    INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T2, INT_ENABLED);

    /******************************************************************************
     * Your code goes in this comment and the following one with asterisks.
     *****************************************************************************/
    OledInit();
    MorseInit();
    int i = 0;
    int j = 0;

    while (TRUE) {
        switch (morseEvent) {
            //**********************************************************DotEvent
        case MORSE_EVENT_DOT:
            lineOne[i] = 0x2E;
            i = i + 1;
            GenerateOled();
            MorseDecode(MORSE_CHAR_DOT);
            morseEvent = MORSE_EVENT_NONE;
            break;
            //**********************************************************DashEvent
        case MORSE_EVENT_DASH:
            lineOne[i] = 0x2D;
            i = i + 1;
            GenerateOled();
            MorseDecode(MORSE_CHAR_DASH);
            morseEvent = MORSE_EVENT_NONE;
            break;
            //**********************************************************InterLetter
        case MORSE_EVENT_INTER_LETTER:

            MorseDecode(MORSE_CHAR_DECODE_RESET);
            morseEvent = MORSE_EVENT_NONE;
            topLineOled();
            GenerateOled();
            break;
            //**********************************************************InterWord
        case MORSE_EVENT_INTER_WORD:
            c = MorseDecode(MORSE_CHAR_END_OF_CHAR);
            lineThree[j] = c;
            j=j+1;
            GenerateOled();
            morseEvent = MORSE_EVENT_NONE;
            i = 0;
            break;
            //**********************************************************
        case MORSE_EVENT_NONE:

            break;
        }//end of switch()
    }//end of while(TRUE)
    while (1);
}

//*************************************************************************************
//

void topLineOled(void)
{
    //memset(&lineOne[20], ' ', sizeof(lineOne));
    //line1 = blankline;
    //clear top line and update it
}
//*************************************************************************************
//

void appendTopLine(char in)
{
    //if in the middle of a word, add morse character here
}
//*************************************************************************************
//

void appendBottomLine(char in)
{
    //decoded morse symbols result
}
//*************************************************************************************
//

void GenerateOled(void)
{
    OledClear(OLED_COLOR_BLACK);

    sprintf(display, "%s\n\n%s", line1, line3);
    OledDrawString(display);
    OledUpdate();
}
//*************************************************************************************
//for testing, and sillyness. Just learning Oled setting/functions.

void makeHappyFace(void)
{
    OledClear(OLED_COLOR_BLACK);
    OledDrawChar(54, 0, 0x5E); //lefteye
    OledDrawChar(78, 0, 0x5E); //righteye
    OledDrawChar(66, 8, 0x2B); //nose
    OledDrawChar(54, 16, 0x5C); //mouth
    OledDrawChar(60, 16, 0x5F);
    OledDrawChar(66, 16, 0x5F); //middle
    OledDrawChar(72, 16, 0x5F);
    OledDrawChar(78, 16, 0x2F);
    OledDrawChar(66, 23, 0x55); //middle
    OledUpdate();
}//end of test display

/******************************************************************************
 * Your code goes in between this comment and the preceding one with asterisks.
 *****************************************************************************/

void __ISR(_TIMER_2_VECTOR, IPL4AUTO) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

    //******** Put your code here *************//
    morseEvent = MorseCheckEvents();
}
