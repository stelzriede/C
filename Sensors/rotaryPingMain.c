/*
 * File:   rotaryPingMain.c
 * Author: Jacob
 *
 * Created on February 5, 2018, 10:34 PM
 */

// Standard Libraries
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <pwm.h>

// CMPE 167 Libraries
#include "xc.h"
#include <BOARD.h>
#include <AD.h>
#include <ToneGeneration.h>
#include <timers.h>

// Function Definitions
int linearInput(unsigned int value);

// Defines
#define FILTERAVERAGE 5
#define PRINTHOLD 1000

#define LOWTONE 500
#define HIGHTONE 200
#define FLATFLEX 5
#define BENTFLEX 200

#define TRIGGERTIME 12  // uS
#define WAITTIME 1 // datasheet, 60 mS
#define STALL for(i=1; i<500000; i++);
#define BLUE PWM_PORTY12
#define GREEN PWM_PORTY10
#define RED PWM_PORTY04
// 5 6 9

// Variables & Typedef's
// Rotary Encoder state machine states
typedef enum{
    IDLE = 0x00,    // IDLE A=1  B=1
    CW1,            // CW1 A=0 B=1
    CW2,            // CW2 A=0  B=0
    CW3,            // CW3 A=1 B=0
    CCW1,           // CCW1 A=1 B=0
    CCW2,           // CCW2 A=0 B=0
    CCW3            // CCW3 A=0 B=1
}STATES;

typedef enum{
    TIMERS,
    TRIGGER,
    WAIT,
    ECHO,
    DISTANCE
}TIME_STATES;

int main(void)
{
    // Library Initializations
    BOARD_Init();
    AD_Init();
    ToneGeneration_Init();
    TIMERS_Init();
    PWM_Init();

    // set port states 0 = output 1 = input
    TRISFbits.TRISF1 = 0;
    TRISDbits.TRISD10 = 1;

    //Pins       A0  -  A1 -    A2     A3
    AD_AddPins(AD_A0 | AD_A1 | AD_A2 | AD_A3);
    // PWM Pin      pin5        pin6          pin9
    PWM_AddPins(PWM_PORTY12 | PWM_PORTY10 | PWM_PORTY04);

    // state machine name holders
    unsigned char state = IDLE;
    unsigned char timeState = TRIGGER;

    int hold = 0;
    int count = 0;
    int m = 0;

    int pingTimer = 0;
    int pingDuration = 0;
    int waitTimer = 0;
    int endWaitTimer = 0;
    int echoTimer = 0;
    int echoDuration = 0;

    int distance = 0;
    int finalDistance = 0;

    // Initial states of Pins.
    int newA = 1;
    int newB = 1;
    int pushButton = 0;
    int average = 0;
    int distanceAve = 0;

    // Forever Loop
    while(1){

        switch(timeState){
            case TIMERS:
                pingTimer = TIMERS_GetMicroSeconds();
                LATDbits.LATD10 = 0;
                LATFbits.LATF4 = 1;
                timeState = TRIGGER;
                break;
            case TRIGGER:
                pingDuration = TIMERS_GetMicroSeconds();
                if((pingDuration - pingTimer) > 12){
                    LATFbits.LATF4 = 0;
                    waitTimer = TIMERS_GetMicroSeconds();
                    timeState = WAIT;
                }
                break;
            case WAIT:
                endWaitTimer = TIMERS_GetMicroSeconds();
                if((endWaitTimer - waitTimer) > 1){
                    LATDbits.LATD10 = 1;
                    timeState = ECHO;
                }
                break;
            case ECHO:
                if(PORTDbits.RD10 == 1){
                    echoTimer = TIMERS_GetMicroSeconds();
                    timeState = DISTANCE;
                }
                break;
            case DISTANCE:
                if(PORTDbits.RD10 == 0){
                    //echoDuration = TIMERS_GetMicroSeconds();
                    echoDuration = echoTimer - waitTimer;
                    distance = (echoDuration / 58) + 1;
                    timeState = TRIGGER;
                }
                break;
            default:
                timeState = TRIGGER;
                break;

        }// end of timeStates

        newA = AD_ReadADPin(AD_A1);
        if(newA > 700){
            newA = 1;
        }else newA = 0;

        newB = AD_ReadADPin(AD_A2);
        if(newB > 700){
            newB = 1;
        }else newB = 0;

        pushButton = AD_ReadADPin(AD_A3);
        if(pushButton > 600){
            pushButton = 1;
        }else pushButton = 0;

        if(pushButton == 1){
            count = 0;
        }
        // ~~~~~~~~~~~~~~~~~ Rotary State Machine~~~~~~~~~~~~~~~~~~~ //
        switch(state){
            case IDLE:
                if(newA==0 && newB==1){
                    state = CW1;
                }
                if(newA==1 && newB==0){
                    state = CCW1;
                }
                break;
            case CW1:
                if(newA==0 && newB==0){
                    state = CW2;
                }
                break;
            case CW2:
                if(newA==1 && newB==0){
                    state = CW3;
                }
                break;
            case CW3:
                if(newA==1 && newB==1){
                    count +=1;
                    state = IDLE;
                }
                break;
            case CCW1:
                if(newA==0 && newB==0){
                    state = CCW2;
                }
                break;
            case CCW2:
                if(newA==0 && newB==1){
                    state = CCW3;
                }
                break;
            case CCW3:
                if(newA==1 && newB==1){
                    count -=1;
                    state = IDLE;
                }
                break;
            default:
                state = IDLE;
                break;
        } // ~~~~~~~~~~~~~~~~~~~~ End of Rotary State Machine ~~~~~~~~~~~~~~~~~~~~~
        // Just a counter to print things semi slowly

        hold ++;
        if(hold > PRINTHOLD){
            if(distance > 5 && distance < 200){
                finalDistance = rangeFit(distance,BENTFLEX, FLATFLEX, HIGHTONE, LOWTONE);
                ToneGeneration_SetFrequency(finalDistance);
                ToneGeneration_ToneOn();
            }
            printf("dist: %d\n\r", distance);
            printf("tone: %d\n\r", finalDistance);
           // printf("~~~~~~~~~~~~~\n\r");
            hold = 0;
        } // end of print loop

// ~~~~~~~~~~ Color Mapping for Rotary ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
        if(count >= 40 || count < -40){
            count = 0;
        }
        if(count >= 0 && count < 5){
            // 5 6 9 - PWM_PORTY12 | PWM_PORTY10 | PWM_PORTY04
            PWM_SetDutyCycle(RED, 0);
            PWM_SetDutyCycle(GREEN, 0);
            PWM_SetDutyCycle(BLUE, 1000);
        } //yellow
        if(count >= 5 && count <10){
            PWM_SetDutyCycle(RED, 500);
            PWM_SetDutyCycle(GREEN, 0);
            PWM_SetDutyCycle(BLUE, 1000);
        } //lime
        if(count >= 10 && count < 15){
            PWM_SetDutyCycle(RED, 1000);
            PWM_SetDutyCycle(GREEN, 0);
            PWM_SetDutyCycle(BLUE, 1000);
        } //green
        if(count >= 15 && count <20){
            PWM_SetDutyCycle(RED, 1000);
            PWM_SetDutyCycle(GREEN, 0);
            PWM_SetDutyCycle(BLUE, 0);
        } //turquoise
        if(count >= 20 && count <25){
            PWM_SetDutyCycle(RED, 1000);
            PWM_SetDutyCycle(GREEN, 1000);
            PWM_SetDutyCycle(BLUE, 0);
        } //blue
        if(count >= 25 && count <30){
            PWM_SetDutyCycle(RED, 500);
            PWM_SetDutyCycle(GREEN, 1000);
            PWM_SetDutyCycle(BLUE, 0);
        } //purple
        if(count >= 30 && count <35){
            PWM_SetDutyCycle(RED, 0);
            PWM_SetDutyCycle(GREEN, 1000);
            PWM_SetDutyCycle(BLUE, 1000);
        } //red
        if(count >= 35 && count <40){
            PWM_SetDutyCycle(RED, 0);
            PWM_SetDutyCycle(GREEN, 600);
            PWM_SetDutyCycle(BLUE, 1000);
        } //orange

    } // end of while loop
    return 0;
}
// ~~~~~~~~~~~~~~~~~~~~~ User Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
int rangeFit(int inputVariable, int in_min, int in_max, int out_min, int out_max){
    return (inputVariable - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// ~~~~~~~~ Notes and things that may be reused below this point ~~~~~~~~ //



//// Negative count
//        if(count < 0 && count > -5){
//            PWM_SetDutyCycle(RED, 0);
//            PWM_SetDutyCycle(GREEN, 0);
//            PWM_SetDutyCycle(BLUE, 1000);
//        } //yellow
//        if(count < 5 && count > -10){
//            PWM_SetDutyCycle(RED, 0);
//            PWM_SetDutyCycle(GREEN, 500);
//            PWM_SetDutyCycle(BLUE, 1000);
//        } //orange
//        if(count >= -10 && count > -15){
//            PWM_SetDutyCycle(RED, 0);
//            PWM_SetDutyCycle(GREEN, 1000);
//            PWM_SetDutyCycle(BLUE, 1000);
//        } //red
//        if(count >= -15 && count > -20){
//            PWM_SetDutyCycle(RED, 500);
//            PWM_SetDutyCycle(GREEN, 1000);
//            PWM_SetDutyCycle(BLUE, 0);
//        } //purple
//        if(count >= -20 && count > -25){
//            PWM_SetDutyCycle(RED, 1000);
//            PWM_SetDutyCycle(GREEN, 1000);
//            PWM_SetDutyCycle(BLUE, 0);
//        } //blue
//        if(count >= -25 && count > -30){
//            PWM_SetDutyCycle(RED, 1000);
//            PWM_SetDutyCycle(GREEN, 0);
//            PWM_SetDutyCycle(BLUE, 0);
//        } //turquoise
//        if(count >= -30 && count > -35){
//            PWM_SetDutyCycle(RED, 1000);
//            PWM_SetDutyCycle(GREEN, 0);
//            PWM_SetDutyCycle(BLUE, 1000);
//        } //green
//        if(count >= -35 && count > -40){
//            PWM_SetDutyCycle(RED, 500);
//            PWM_SetDutyCycle(GREEN, 0);
//            PWM_SetDutyCycle(BLUE, 1000);
//        } //lime


    // RD10 is Pin8
    //PORTSetPinsDigitalIn(PORTDbits.RD10);
    // RF4 is Pin4
    //PORTSetPinsDigitalIn(PORTFbits.RF4);

  //PORTDbits.RD10 == 1    means port is HIGH