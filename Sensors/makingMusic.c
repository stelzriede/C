/*
 * File:   makingMusic.c
 * Author: Jacob
 *
 * Created on January 19, 2018, 9:42 PM
 */

#include "xc.h"
#include <BOARD.h>
#include <stdio.h>
#include <AD.h>
#include <ToneGeneration.h>

// Function Declarations
int linearInput(void);
int i;

// version issue, can't use delay() or sleep() ??
 
#define INPUTDIVIDER 4
#define FILTERAVERAGE 5

int main(void)
{
    // Initialize the user libraries
    BOARD_Init();
    AD_Init();
    ToneGeneration_Init();
    
    // Set up Variables
    int potInput = 0;
    i = 0;

    AD_AddPins(AD_A0);

    while(1){

        ToneGeneration_ToneOff();
        potInput = linearInput() / INPUTDIVIDER;

        while(BTN1 = 0){
            ToneGeneration_SetFrequency(TONE_196 + potInput);
            ToneGeneration_ToneOn();
        }
        while(BTN2 = 0){
            ToneGeneration_SetFrequency(TONE_293 + potInput);
            ToneGeneration_ToneOn();
        }
        while(BTN3 = 0){
            ToneGeneration_SetFrequency(TONE_440 + potInput);
            ToneGeneration_ToneOn();
        }
        while(BTN4 = 0){
            ToneGeneration_SetFrequency(TONE_659 + potInput);
            ToneGeneration_ToneOn();
        }
    } // end while(1)
} // end main()

// Quick function to clean up the input from the pot
// Cleaned this up from check off.
int linearInput(void){
    int valueFilter[FILTERAVERAGE];
    int valueFilterAverage = 0;
    for(i = 0; i<FILTERAVERAGE;i++){
        valueFilter[i] = AD_ReadADPin(AD_A0);
        valueFilterAverage += valueFilter[i];
    }
    return valueFilterAverage/FILTERAVERAGE;
} // end of linearInput()

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  Notes beyond this point.  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// for part 3 just hard code value from ToneGeneration.h values
ToneGeneration_SetFrequency(value);
ToneGeneration_ToneOn();
STALL

*/
