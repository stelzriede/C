// File:   Leds.h
// Author: Jacob Stelzriede
/*************************************************************************************************/
#ifndef LEDS_H          // header guards 1of3
#define	LEDS_H          // header guards 2of3

#include "BOARD.h"

#include <xc.h>

//LATE register controls the LED's
//TRISE is used to set the ports to input or output to enable control of LED's

//a macro used to initialize the registers used to operate the LED's
/*************************************************************************************************/
#define LEDS_INIT() do { TRISE = 0x0; LATE = 0x0; } while(0)

//a macro to return the address currently on the LATE register
/*************************************************************************************************/
#define LEDS_GET() (LATE)

//a macro to set the LATE register to control LED's
/*************************************************************************************************/
#define LEDS_SET(x) (LATE = (x))

/*************************************************************************************************/
#endif	/* LEDS_H */        //header guards 3of3