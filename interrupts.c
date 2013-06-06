/******************************************************************************/

#include "definitions.h"

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

#include <stdint.h>        /* Includes uint16_t definition   */
#include <stdbool.h>       /* Includes true/false definition */
#include "definitions.h"

extern unsigned int dist2, dist4, dist6, dist8;
Sflags flags;
extern array distancias;
extern unsigned int grado;

/******************************************************************************/
/* Interrupt Vector Options                                                   */
/******************************************************************************/
/*                                                                            */
/* Refer to the C30 (MPLAB C Compiler for PIC24F MCUs and dsPIC33F DSCs) User */
/* Guide for an up to date list of the available interrupt options.           */
/* Alternately these names can be pulled from the device linker scripts.      */
/*                                                                            */
/* dsPIC33F Primary Interrupt Vector Names:                                   */
/*                                                                            */
/* _INT0Interrupt      _C1Interrupt                                           */
/* _IC1Interrupt       _DMA3Interrupt                                         */
/* _OC1Interrupt       _IC3Interrupt                                          */
/* _T1Interrupt        _IC4Interrupt                                          */
/* _DMA0Interrupt      _IC5Interrupt                                          */
/* _IC2Interrupt       _IC6Interrupt                                          */
/* _OC2Interrupt       _OC5Interrupt                                          */
/* _T2Interrupt        _OC6Interrupt                                          */
/* _T3Interrupt        _OC7Interrupt                                          */
/* _SPI1ErrInterrupt   _OC8Interrupt                                          */
/* _SPI1Interrupt      _DMA4Interrupt                                         */
/* _U1RXInterrupt      _T6Interrupt                                           */
/* _U1TXInterrupt      _T7Interrupt                                           */
/* _ADC1Interrupt      _SI2C2Interrupt                                        */
/* _DMA1Interrupt      _MI2C2Interrupt                                        */
/* _SI2C1Interrupt     _T8Interrupt                                           */
/* _MI2C1Interrupt     _T9Interrupt                                           */
/* _CNInterrupt        _INT3Interrupt                                         */
/* _INT1Interrupt      _INT4Interrupt                                         */
/* _ADC2Interrupt      _C2RxRdyInterrupt                                      */
/* _DMA2Interrupt      _C2Interrupt                                           */
/* _OC3Interrupt       _DCIErrInterrupt                                       */
/* _OC4Interrupt       _DCIInterrupt                                          */
/* _T4Interrupt        _DMA5Interrupt                                         */
/* _T5Interrupt        _U1ErrInterrupt                                        */
/* _INT2Interrupt      _U2ErrInterrupt                                        */
/* _U2RXInterrupt      _DMA6Interrupt                                         */
/* _U2TXInterrupt      _DMA7Interrupt                                         */
/* _SPI2ErrInterrupt   _C1TxReqInterrupt                                      */
/* _SPI2Interrupt      _C2TxReqInterrupt                                      */
/* _C1RxRdyInterrupt                                                          */
/*                                                                            */
/* dsPIC33E Primary Interrupt Vector Names:                                   */
/*                                                                            */
/* _INT0Interrupt     _IC4Interrupt      _U4TXInterrupt                       */
/* _IC1Interrupt      _IC5Interrupt      _SPI3ErrInterrupt                    */
/* _OC1Interrupt      _IC6Interrupt      _SPI3Interrupt                       */
/* _T1Interrupt       _OC5Interrupt      _OC9Interrupt                        */
/* _DMA0Interrupt     _OC6Interrupt      _IC9Interrupt                        */
/* _IC2Interrupt      _OC7Interrupt      _PWM1Interrupt                       */
/* _OC2Interrupt      _OC8Interrupt      _PWM2Interrupt                       */
/* _T2Interrupt       _PMPInterrupt      _PWM3Interrupt                       */
/* _T3Interrupt       _DMA4Interrupt     _PWM4Interrupt                       */
/* _SPI1ErrInterrupt  _T6Interrupt       _PWM5Interrupt                       */
/* _SPI1Interrupt     _T7Interrupt       _PWM6Interrupt                       */
/* _U1RXInterrupt     _SI2C2Interrupt    _PWM7Interrupt                       */
/* _U1TXInterrupt     _MI2C2Interrupt    _DMA8Interrupt                       */
/* _AD1Interrupt      _T8Interrupt       _DMA9Interrupt                       */
/* _DMA1Interrupt     _T9Interrupt       _DMA10Interrupt                      */
/* _NVMInterrupt      _INT3Interrupt     _DMA11Interrupt                      */
/* _SI2C1Interrupt    _INT4Interrupt     _SPI4ErrInterrupt                    */
/* _MI2C1Interrupt    _C2RxRdyInterrupt  _SPI4Interrupt                       */
/* _CM1Interrupt      _C2Interrupt       _OC10Interrupt                       */
/* _CNInterrupt       _QEI1Interrupt     _IC10Interrupt                       */
/* _INT1Interrupt     _DCIEInterrupt     _OC11Interrupt                       */
/* _AD2Interrupt      _DCIInterrupt      _IC11Interrupt                       */
/* _IC7Interrupt      _DMA5Interrupt     _OC12Interrupt                       */
/* _IC8Interrupt      _RTCCInterrupt     _IC12Interrupt                       */
/* _DMA2Interrupt     _U1ErrInterrupt    _DMA12Interrupt                      */
/* _OC3Interrupt      _U2ErrInterrupt    _DMA13Interrupt                      */
/* _OC4Interrupt      _CRCInterrupt      _DMA14Interrupt                      */
/* _T4Interrupt       _DMA6Interrupt     _OC13Interrupt                       */
/* _T5Interrupt       _DMA7Interrupt     _IC13Interrupt                       */
/* _INT2Interrupt     _C1TxReqInterrupt  _OC14Interrupt                       */
/* _U2RXInterrupt     _C2TxReqInterrupt  _IC14Interrupt                       */
/* _U2TXInterrupt     _QEI2Interrupt     _OC15Interrupt                       */
/* _SPI2ErrInterrupt  _U3ErrInterrupt    _IC15Interrupt                       */
/* _SPI2Interrupt     _U3RXInterrupt     _OC16Interrupt                       */
/* _C1RxRdyInterrupt  _U3TXInterrupt     _IC16Interrupt                       */
/* _C1Interrupt       _USB1Interrupt     _ICDInterrupt                        */
/* _DMA3Interrupt     _U4ErrInterrupt    _PWMSpEventMatchInterrupt            */
/* _IC3Interrupt      _U4RXInterrupt     _PWMSecSpEventMatchInterrupt         */
/*                                                                            */
/* For alternate interrupt vector naming, simply add 'Alt' between the prim.  */
/* interrupt vector name '_' and the first character of the primary interrupt */
/* vector name.  There is no Alternate Vector or 'AIVT' for the 33E family.   */
/*                                                                            */
/* For example, the vector name _ADC2Interrupt becomes _AltADC2Interrupt in   */
/* the alternate vector table.                                                */
/*                                                                            */
/* Example Syntax:                                                            */
/*                                                                            */
/* void __attribute__((interrupt,auto_psv)) <Vector Name>(void)               */
/* {                                                                          */
/*     <Clear Interrupt Flag>                                                 */
/* }                                                                          */
/*                                                                            */
/* For more comprehensive interrupt examples refer to the C30 (MPLAB C        */
/* Compiler for PIC24 MCUs and dsPIC DSCs) User Guide in the                  */
/* <C30 compiler instal directory>/doc directory for the latest compiler      */
/* release.  For XC16, refer to the MPLAB XC16 C Compiler User's Guide in the */
/* <XC16 compiler instal directory>/doc folder.                               */
/*                                                                            */
/******************************************************************************/
/* Interrupt Routines                                                         */

/******************************************************************************/
//enum modoTimer1 {
//    MOTOR, TRIGGER, ESPERA
//};
//enum modoTimer1 modo = MOTOR;

#define MOTOR   0
#define TRIGGER 1
#define ESPERA  2

unsigned char modoTimer1 = MOTOR;

void ponMotor() {
    T1CONbits.TON = 0;
    modoTimer1 = MOTOR;
    PR1 = TIME1_TRIGGER_MOTOR;
    TMR1 = 0;
    T1CONbits.TON = 1;
}


/* Timer 1 interrupt service routine */
void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void) {
    T1CONbits.TON = 0;
    IFS0bits.T1IF = 0;
    
    // Open-drain, not draining (1 at trigger sensor)
    TRISAbits.TRISA5 = 0;

    switch (modoTimer1) {
        case MOTOR:
            modoTimer1 = TRIGGER;
            PR1 = TIME1_TRIGGER_SENSOR;
            T1CONbits.TCKPS = 1;
    
	    //Ponemos un 1 en el puerto trigger del sensor para activarlo
	    TRISAbits.TRISA5 = 0;

            PORTBbits.RB0 = 1;
            PORTFbits.RF4 = 0;
            PORTAbits.RA2 = 0;
            PORTDbits.RD0 = 0;
            break;
        case TRIGGER:
            modoTimer1 = ESPERA;
            T1CONbits.TCKPS = 3;
            PR1 = MAX_TIMER_SENS;

	    //Ponemos un 0 en el puerto trigger del sensor, ya está activo
	    TRISAbits.TRISA5 = 1;

            PORTBbits.RB0 = 0;
            PORTFbits.RF4 = 1;
            PORTAbits.RA2 = 0;
            PORTDbits.RD0 = 0;
            break;
        case ESPERA:
            modoTimer1 = MOTOR;
            PR1 = MAX_TIMER_SENS;
            PORTBbits.RB0 = 1;
            PORTFbits.RF4 = 1;
            PORTAbits.RA2 = 1;
            PORTDbits.RD0 = 1;
            break;
    }
    TMR1 = 0;
    T1CONbits.TON = 1;
}

/* Timer 2 interrupt service routine */
void __attribute__((interrupt, auto_psv)) _T2Interrupt(void) {
    IFS0bits.T2IF = 0;
    T2CONbits.TON = 0;
    TMR2 = 0;
    INTCON2bits.INT0EP = 0;
    dist2 = MAX_DIST;
}

/* Timer 4 interrupt service routine */
void __attribute__((interrupt, auto_psv)) _T4Interrupt(void) {
    IFS1bits.T4IF = 0;
    T4CONbits.TON = 0;
    TMR4 = 0;
    INTCON2bits.INT1EP = 0;
    dist4 = MAX_DIST;
}

/* Timer 6 interrupt service routine */
void __attribute__((interrupt, auto_psv)) _T6Interrupt(void) {
    IFS2bits.T6IF = 0;
    T6CONbits.TON = 0;
    TMR6 = 0;
    INTCON2bits.INT2EP = 0;
    dist6 = MAX_DIST;
}

/* Timer 8 interrupt service routine */
void __attribute__((interrupt, auto_psv)) _T8Interrupt(void) {
    IFS3bits.T8IF = 0;
    T8CONbits.TON = 0;
    TMR8 = 0;
    INTCON2bits.INT3EP = 0;
    dist8 = MAX_DIST;
}

/* External 0 interrupt service routine */
void __attribute__((interrupt, auto_psv)) _INT0Interrupt(void) {
    IFS0bits.T2IF = 0;
    IFS0bits.INT0IF = 0;
    INTCON2bits.INT0EP = !INTCON2bits.INT0EP;
    if (INTCON2bits.INT0EP) {
        distancias[grado / GIR] = TMR2;
        TMR2 = 0;
        flags[0] = 1;
    }
    INTCON2bits.INT0EP = !INTCON2bits.INT0EP;
    T2CONbits.TON = INTCON2bits.INT0EP;
}

/* External 1 interrupt service routine */
void __attribute__((interrupt, auto_psv)) _INT1Interrupt(void) {
    IFS1bits.T4IF = 0;
    IFS1bits.INT1IF = 0;
    INTCON2bits.INT1EP = !INTCON2bits.INT1EP;
    if (INTCON2bits.INT1EP) {
        distancias[grado / GIR + 360 / NUM_SENSORS * 1] = TMR4;
        TMR4 = 0;
        flags[1] = 1;
    }
    INTCON2bits.INT1EP = !INTCON2bits.INT1EP;
    T4CONbits.TON = INTCON2bits.INT1EP;
}

/* External 2 interrupt service routine */
void __attribute__((interrupt, auto_psv)) _INT2Interrupt(void) {
    IFS2bits.T6IF = 0;
    IFS1bits.INT2IF = 0;
    INTCON2bits.INT2EP = !INTCON2bits.INT2EP;
    if (INTCON2bits.INT2EP) {
        distancias[grado / GIR + 360 / NUM_SENSORS * 2] = TMR6;
        TMR6 = 0;
        flags[2] = 1;
    }
    INTCON2bits.INT2EP = !INTCON2bits.INT2EP;
    T6CONbits.TON = INTCON2bits.INT2EP;
}

//ECHO
/* External 3 interrupt service routine */
void __attribute__((interrupt, auto_psv)) _INT3Interrupt(void) {
    IFS3bits.T8IF = 0;
    IFS3bits.INT3IF = 0;
    if (INTCON2bits.INT3EP) {
        distancias[0] = TMR8;
        flags[3] = 1;
    }
    else{
        TMR8 = 0;
    }
    INTCON2bits.INT3EP = !INTCON2bits.INT3EP;
    T8CONbits.TON = INTCON2bits.INT3EP;
}
