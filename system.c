/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC33E__)
    	#include <p33Exxxx.h>
    #elif defined(__dsPIC33F__)
    	#include <p33Fxxxx.h>
    #endif
#endif


#include <stdint.h>          /* For uint16_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */

#include "system.h"          /* variables/params used by system.c             */

/******************************************************************************/
/* System Level Functions                                                     */
/*                                                                            */
/* Custom oscillator configuration funtions, reset source evaluation          */
/* functions, and other non-peripheral microcontroller initialization         */
/* functions get placed in system.c.                                          */
/*                                                                            */
/******************************************************************************/

/* Refer to the device Family Reference Manual Oscillator section for
information about available oscillator configurations.  Typically
this would involve configuring the oscillator tuning register or clock
switching useing the compiler's __builtin_write_OSCCON functions.
Refer to the C Compiler for PIC24 MCUs and dsPIC DSCs User Guide in the
compiler installation directory /doc folder for documentation on the
__builtin functions.*/

void ConfigureOscillator(void)
{
    OSCCONbits.NOSC = 3;    //select FRC (internal clock)
    OSCCONbits.CF = 0;      //clear clock fail detect bit
    OSCCONbits.LPOSCEN = 0; //disable secondary clock
    OSCCONbits.OSWEN = 1;   //request oscillator switch in order to change to FRC

    CLKDIVbits.ROI = 0;     //interrupts have no effect on dozen bit
    CLKDIVbits.DOZE = 0;    //FCY/1
    CLKDIVbits.DOZEN = 0;   //processor clock forced to 1:1
    CLKDIVbits.FRCDIV = 0;  //FRC divide by 1

    OSCTUNbits.TUN = 0;     //center frequency 7,37MHz

    OSCCONbits.CLKLOCK = 1; //lock pll and clock configuration
}

