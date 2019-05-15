// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Leds.h"

// **** Set macros and preprocessor directives ****

#define led1 (0x01)
#define led8 (0x80)

//#define led2 (0x02)
//#define led3 (0x04)
//#define led4 (0x08)
//#define led5 (0x10)
//#define led6 (0x20)
//#define led7 (0x40)

//#define DELAY 10000000
//#define SWITCH_SUM (SW1 + SW2 + SW3 + SW4 + 1)

// **** Declare any datatypes here ****

typedef struct TimerResult {
    uint8_t event;
    uint8_t value;
} TimerResult;

// **** Define global, module-level, or external variables here ****
static TimerResult eventData;
static int LEFT = 1;
static int RIGHT = 0;



// **** Declare function prototypes ****

int main(void)
{
    BOARD_Init();
    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_8, 0xFFFF);

    // Set up the timer interrupt with a priority of 4.
    INTClearFlag(INT_T1);
    INTSetVectorPriority(INT_TIMER_1_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T1, INT_ENABLED);

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    //attempt4
    LEDS_INIT();
    LEDS_SET(led8);
    int direction = RIGHT;

    while (TRUE) {
        if (eventData.event == TRUE) {
            if (direction == RIGHT) {
                if ((LEDS_GET()) != led1) {
                    LEDS_SET(LEDS_GET() >> 1);
                }
                else {
                    LEDS_SET(led1);
                    direction = LEFT;
                }
                eventData.event = FALSE;
            } //end of while not led1

             if (direction == LEFT) {
                if (LEDS_GET() != led8) {
                    LEDS_SET(LEDS_GET() << 1);
                } //shifting left
                else {
                    LEDS_SET(led8);
                    direction = RIGHT;
                }//end of change direction else
                eventData.event = FALSE;
            } //end of while not led1
        }; //end of while event true (attempt4)
    }//main while true loop

        /***************************************************************************************************
         * Your code goes in between this comment and the preceding one with asterisks
         **************************************************************************************************/

        while (1);
    }

    /**
     * This is the interrupt for the Timer1 peripheral. During each call it increments a counter (the
     * value member of a module-level TimerResult struct). This counter is then checked against the
     * binary values of the four switches on the I/O Shield (where SW1 has a value of 1, SW2 has a value
     * of 2, etc.). If the current counter is greater than this switch value, then the event member of a
     * module-level TimerResult struct is set to true and the value member is cleared.
     */
    void __ISR(_TIMER_1_VECTOR, IPL4AUTO) Timer1Handler(void)
    {
        // Clear the interrupt flag.
        INTClearFlag(INT_T1);
        eventData.value += 1;
        if (eventData.value > SWITCH_STATES()) { //if next value > switchstates, event and reset
            eventData.event = TRUE;
            eventData.value = 0;
        }

    }