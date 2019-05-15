//Jacob Stelzriede - CE13
// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "Adc.h"
#include "Ascii.h"
#include "Buttons.h"
#include "Oled.h"
#include "OledDriver.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Leds.h"  //created by me.

// **** Set any macros or preprocessor directives here ****
// Set a macro for resetting the timer, makes the code a little clearer.
#define TIMER_2HZ_RESET() (TMR1 = 0)

// words for turning on led's
#define led1 (0x01)
#define led8 (0x80)

// Adc conversions and a macro to make use very clear
#define Adc_to_Temperature ((AdcRead() >> 2) + 300)
#define Adc_to_Time ((AdcRead() >> 2) +1)

//press checks
#define LongPressCheck (Hold_Time > LONG_PRESS)
#define ShortPress (Hold_Time < LONG_PRESS)

//oven formatting
#define OvenTopOff "|\x02\x02\x02\x02\x02|"
#define OvenTopOn "|\x01\x01\x01\x01\x01|"
#define OvenSpace "|     |"
#define OvenMiddle "|-----|"
#define OvenBottomOff "|\x04\x04\x04\x04\x04|"
#define OvenBottomOn "|\x03\x03\x03\x03\x03|"


// **** Declare any datatypes here ****
//to store interrupt data

typedef struct TimerResult {
    uint8_t event;
    uint8_t value;
} TimerResult;

//to store oven and state information
//I used enum's for some oven data

typedef struct ovenData {
    int timeLeft;
    int cookTime;
    int cookTemp;
} ovenData;

// **** Define any module-level, global, or external variables here ****
static ovenData ovenInfo;

//use to set oven mode display.
static char *OvenTop;
static char *OvenBottom;
static char display[50];
static int SaveTimer1 = 0;
static int SaveTimer2 = 0;
static int Hold_Time = 0;
const int LONG_PRESS = 5;

//interrupt/timer structure names
static TimerResult Timer2Hz; //2Hz
static TimerResult TimerRunning; //5Hz
static TimerResult TimerButtons; //100Hz

static enum {
    BAKE = 1,
    TOAST = 2,
    BROIL = 3
} BakeMode;

static enum {
    TIME = 1,
    TEMP = 2,
    COOKING = 3
} SelectionMode;

//function prototypes
void GenerateOled(void);
void InitialOvenSetting(void);

//*******************************************Main***************************************************

int main()
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. We configure it using a 1:256 prescalar, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR1 to F_PB / 256 / 2 yields a 0.5s timer.
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_256, BOARD_GetPBClock() / 256 / 2);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T1);
    INTSetVectorPriority(INT_TIMER_1_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T1, INT_ENABLED);

    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_16, BOARD_GetPBClock() / 16 / 100);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T2);
    INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T2, INT_ENABLED);

    // Configure Timer 3 using PBCLK as input. We configure it using a 1:256 prescalar, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR3 to F_PB / 256 / 5 yields a .2s timer.
    OpenTimer3(T3_ON | T3_SOURCE_INT | T3_PS_1_256, BOARD_GetPBClock() / 256 / 5);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T3);
    INTSetVectorPriority(INT_TIMER_3_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_3_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T3, INT_ENABLED);

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/

    OledInit();
    ButtonsInit();
    LEDS_INIT();
    AdcInit();
    InitialOvenSetting();
    GenerateOled();

    //states for FSM

    enum {
        START = 1,
        RESET = 2,
        PENDING_SELECTOR_CHANGE = 3,
        COUNTDOWN = 4,
        PENDING_RESET = 5
    } state = START;

    //begin state/switch statements.
    while (TRUE) {
        switch (state) {
            //START State  ******************************************************************
        case START:
            GenerateOled();
            // pot control
            if (AdcChanged() == TRUE) {
                if (SelectionMode == TIME) {
                    ovenInfo.cookTime = Adc_to_Time;
                    state = START;
                }
                if (SelectionMode == TEMP) {
                    ovenInfo.cookTemp = Adc_to_Temperature;
                    state = START;
                }
            } //end of AdcChange check
            if (TimerButtons.value == BUTTON_EVENT_3DOWN) { //save timer clear event DOWNpress
                SaveTimer1 = TimerRunning.value;
                TimerButtons.value = BUTTON_EVENT_NONE;
            }
            if (TimerButtons.value == BUTTON_EVENT_3UP) { //save timer got to pending select
                Hold_Time = TimerRunning.value - SaveTimer1;
                //TimerButtons.value = BUTTON_EVENT_NONE;
                state = PENDING_SELECTOR_CHANGE;
            }
            if (TimerButtons.value == BUTTON_EVENT_4DOWN) { //countdown state, reset 2hz event
                SaveTimer2 = TimerRunning.value;
                TimerButtons.value = BUTTON_EVENT_NONE;
                state = COUNTDOWN;
            }
            if (TimerButtons.value == BUTTON_EVENT_4UP) { //countdown state, reset 2hz event
                Timer2Hz.event = FALSE;
                Hold_Time = TimerRunning.value - SaveTimer2;
                TimerButtons.value = BUTTON_EVENT_NONE;
                state = COUNTDOWN;
            }
            break;

            // Pending Selector Change *******************************************************
        case PENDING_SELECTOR_CHANGE:
            if (BUTTON_EVENT_3UP && ShortPress) {
                BakeMode += 1;
                TimerButtons.value = BUTTON_EVENT_NONE;
                state = START;
            }
            if (LongPressCheck) {
                if (SelectionMode == 1) {
                    SelectionMode++;
                } else if (SelectionMode == 2) {
                    SelectionMode--;
                }
                state = START;
            }//end of long press check
            if (BakeMode > 3) {
                BakeMode = BAKE;
                state = START;
            }
            TimerButtons.value = BUTTON_EVENT_NONE;
            state = START;
            break;
            //Countdown*****************************************************************
        case COUNTDOWN:
            ovenInfo.timeLeft = ovenInfo.cookTime;
            
            OvenTop = OvenTopOn;
            OvenBottom = OvenBottomOn;
            LEDS_SET(0xffff);
            SelectionMode = COOKING;
            if (TimerButtons.value == BUTTON_EVENT_4DOWN) { //countdown state, reset 2hz event
                SaveTimer2 = TimerRunning.value;
                TimerButtons.value = BUTTON_EVENT_NONE;
            }
            if (TimerButtons.value == BUTTON_EVENT_4UP) { //countdown state, reset 2hz event
                Hold_Time = TimerRunning.value - SaveTimer2;
                TimerButtons.value = BUTTON_EVENT_NONE;
            }
            if(LongPressCheck){
                ovenInfo.cookTime = 0;
                state = RESET;
            }
            if(Timer2Hz.event == TRUE){
                ovenInfo.timeLeft--;
                Timer2Hz.event = FALSE;
            }
            GenerateOled();
            state = COUNTDOWN;

            if(ovenInfo.timeLeft == 0){
                state = RESET;
            }
            break;
            //
        case PENDING_RESET:
            break;

            //RESET State
        case RESET:
            OledClear(OLED_COLOR_BLACK);
            InitialOvenSetting();
            TimerButtons.value = BUTTON_EVENT_NONE;
            Hold_Time = 0;
            state = START;
            break;

        }//end of switch(state)
    }//end of while(TRUE)

    while (1);
} //end of int main()

//This function is just to make updating the Oled quicker during code. no input/outputs.
//It does use static variable display which is the character array for the display
//it also uses static pointers to access data

void GenerateOled(void)
{
    //***********************************************************************Bake Mode***
    if (BakeMode == BAKE) {
        OledClear(OLED_COLOR_BLACK);
        if (SelectionMode == TIME) {
            sprintf(display, "%s  Mode: Bake\n%s >Time: %d"":""%02d\n%s  Temp: %d\xF8 F\n%s",
                    OvenTop, OvenSpace, (Adc_to_Time)/60, (Adc_to_Time)%60,
                    OvenMiddle, ovenInfo.cookTemp, OvenBottom);
        }
        if (SelectionMode == TEMP) {
            sprintf(display, "%s  Mode: Bake\n%s  Time: %d"":""%02d\n%s >Temp: %d\xF8 F\n%s",
                    OvenTop, OvenSpace, (ovenInfo.cookTime)/60, (ovenInfo.cookTime)%60,
                    OvenMiddle, Adc_to_Temperature, OvenBottom);
        }
        if (SelectionMode == COOKING) {
            sprintf(display, "%s  Mode: Bake\n%s  Time: %d"":""%02d\n%s  Temp: %d\xF8 F\n%s",
                    OvenTop, OvenSpace, (ovenInfo.timeLeft)/60, (ovenInfo.timeLeft)%60,
                    OvenMiddle, ovenInfo.cookTemp, OvenBottom);
        }
        OledDrawString(display);
        OledUpdate();
        //******************************************************************Toast Mode***
    } else if (BakeMode == TOAST) {
        OledClear(OLED_COLOR_BLACK);
        sprintf(display, "%s  Mode: Toast\n%s  Time: %d"":""%02d\n%s\n%s",
                OvenTop, OvenSpace, (Adc_to_Time)/60, (Adc_to_Time)%60,
                OvenMiddle, OvenBottom);
        OledDrawString(display);
        OledUpdate();
        //******************************************************************Broil Mode***
    } else if (BakeMode == BROIL) {
        OledClear(OLED_COLOR_BLACK);
        ovenInfo.cookTemp = 500;
        sprintf(display, "%s  Mode: Broil\n%s  Time: %d"":""%02d\n%s  Temp: %d\xF8 F\n%s",
                OvenTop, OvenSpace, (Adc_to_Time)/60, (Adc_to_Time)%60,
                OvenMiddle, ovenInfo.cookTemp, OvenBottom);
        OledDrawString(display);
        OledUpdate();
    }
} //end of GenerateOled()
//***********************************************************************************
// some initial values to reset and start with
//other useful reset and initial values I also put here.
void InitialOvenSetting(void)
{
    BakeMode = 1;
    ovenInfo.cookTemp = 350;
    ovenInfo.cookTime = 1;
    ovenInfo.timeLeft = 0;
    LEDS_SET(0x0);
    OvenTop = OvenTopOff;
    OvenBottom = OvenBottomOff;
    SelectionMode = TIME;
    TimerButtons.value = BUTTON_EVENT_NONE;
    Hold_Time = 0;
}

/***************************************************************************************************
 * Your code goes in between this comment and the preceding one with asterisks
 **************************************************************************************************/
void __ISR(_TIMER_1_VECTOR, ipl4auto) TimerInterrupt2Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 4;
    Timer2Hz.event = TRUE;
}

void __ISR(_TIMER_3_VECTOR, ipl4auto) TimerInterrupt5Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 12;
    TimerRunning.value++;
}

void __ISR(_TIMER_2_VECTOR, ipl4auto) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;
    uint8_t check = ButtonsCheckEvents();
    if (check > 0) {
        TimerButtons.value = check;
        TimerButtons.event = TRUE;
    }
}