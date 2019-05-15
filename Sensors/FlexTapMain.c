/*
 * File:   FlexTapMain.c
 * Author: Jacob
 *
 * Created on January 26, 2018, 9:12 PM
 */

// Standard Libraries
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// CMPE 167 Libraries
#include "xc.h"
#include <BOARD.h>
#include <AD.h>
#include <ToneGeneration.h>

// Function Definitions
int linearInput(unsigned int pin);
int rangeFit(int x, int in_min, int in_max, int out_min, int out_max);

// Defines and Variables
int i;
#define LOWTONE 660
#define HIGHTONE 196
#define FLATFLEX 180
#define BENTFLEX 110
#define FILTERAVERAGE 10
#define STALL for(i=1; i<500000; i++);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~ Main ~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
int main(void)
{
    BOARD_Init();
    AD_Init();
    ToneGeneration_Init();

    //Maps to pin A0
    AD_AddPins(AD_A0);
    //Maps to pin A2
    AD_AddPins(AD_A4);

    int flexInput = 0;
    int tapInput = 0;
    int tapFilterAverage = 0;

    ToneGeneration_ToneOff();
    while(1){
        int tapFilter[FILTERAVERAGE] = {0};
        for(i = 0; i<FILTERAVERAGE;i++){
            tapFilter[i] = AD_ReadADPin(AD_A4);
            tapFilterAverage += tapFilter[i];
        }
        tapFilterAverage /= FILTERAVERAGE;
        if(tapFilterAverage < 0){
            tapFilterAverage = 0;
        }
        if(tapFilterAverage > 512){
            tapFilterAverage = 512;
        }
        printf("value: %d \n\r", tapFilterAverage);
        if(tapFilterAverage>70){
            tapFilterAverage = 0;
            flexInput = linearInput(AD_A0);
            ToneGeneration_SetFrequency(flexInput);
            ToneGeneration_ToneOn();
            STALL;
            ToneGeneration_ToneOff();
        }
        //ToneGeneration_SetFrequency(flexInput);
    } // end of forever loop

} // end of main()

int linearInput(unsigned int pin){

    int valueFilter[FILTERAVERAGE];
    int valueFilterAverage = 0;
    for(i = 0; i<FILTERAVERAGE;i++){
        valueFilter[i] = AD_ReadADPin(pin);
        valueFilter[i] = rangeFit(valueFilter[i], BENTFLEX, FLATFLEX, HIGHTONE, LOWTONE);
        valueFilterAverage += valueFilter[i];
    }
    return valueFilterAverage/FILTERAVERAGE;    

} // end of linearInput()

// Techniques of Geoscientific Experiementation Documentation
// tge.readthedocs.io/en/latest/index.html  Follow this link to lab exercises, then Math
// The idea is to test the range you get from your sensor and set those values as the high and
// low threshold for the flex sensor.  Then because we are making tones, set the range of output
// to that of tones you with to cycle through.
int rangeFit(int inputVariable, int in_min, int in_max, int out_min, int out_max){
    return (inputVariable - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


/* Part 2
        ToneGeneration_ToneOn();
    while(1){
        flexInput = linearInput(AD_A0);
        ToneGeneration_SetFrequency(flexInput);
        STALL;
    } // end of forever loop

 */
