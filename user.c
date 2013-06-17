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
#include <stdio.h>
#include "user.h"            /* variables/params used by user.c               */
#include "definitions.h"
#include "interrupts.h"
#include <libpic30.h>
#include <delay.h>
#include <timer.h>



//When one access thus vectors one ought to divide the angles one wants to
//consult the value of between 15
double sinus[360 / GIR] = {0.0, 0.25881904510252074, 0.49999999999999994, 0.7071067811865475, 0.8660254037844386, 0.9659258262890683, 1.0, 0.9659258262890683, 0.8660254037844387, 0.7071067811865476, 0.49999999999999994, 0.258819045102521, 1.2246467991473532E-16, -0.25881904510252035, -0.5000000000000001, -0.7071067811865475, -0.8660254037844385, -0.9659258262890683, -1.0, -0.9659258262890682, -0.8660254037844386, -0.7071067811865477, -0.5000000000000004, -0.2588190451025207};
double cosinus[360 / GIR] = {1.0, 0.9659258262890683, 0.8660254037844387, 0.7071067811865476, 0.5000000000000001, 0.25881904510252074, 6.123233995736766E-17, -0.25881904510252085, -0.4999999999999998, -0.7071067811865475, -0.8660254037844387, -0.9659258262890682, -1.0, -0.9659258262890684, -0.8660254037844386, -0.7071067811865477, -0.5000000000000004, -0.25881904510252063, -1.8369701987210297E-16, 0.25881904510252113, 0.5000000000000001, 0.7071067811865474, 0.8660254037844384, 0.9659258262890683};

unsigned int dist2, dist4, dist6, dist8;
unsigned char SensorsFinished;
unsigned int grado = 0;
array distancias[360 / GIR];

/******************************************************************************/
/* Auxiliar functions                                                         */
/******************************************************************************/

/******************************************************************************/
/* This function sets the needed interrupts                                   */

/******************************************************************************/
void initInterrupts(void) {
    /* Disable nested interrupts */
    INTCON1bits.NSTDIS = 1;

    /* Set same priority to all interrupts */
    IPC0 = 0;
    IPC1 = 0;
    IPC2 = 0;
    IPC3 = 0;
    IPC4 = 0;
    IPC5 = 0;
    IPC6 = 0;
    IPC7 = 0;
    IPC8 = 0;
    IPC9 = 0;
    IPC10 = 0;
    IPC11 = 0;
    IPC12 = 0;
    IPC13 = 0;
    IPC14 = 0;
    IPC15 = 0;
    IPC16 = 0;
    IPC17 = 0;

    /* Set the highest priority for the timers and a little less one for the  */
    /* external ones */
    /* TODO We have to take a deeper look at the ports we are using to read from the "echo" */
    /* port from the sensor. We have to be sure that the input port supports 5V signal */
    IPC0bits.INT0IP = 6;
    IPC5bits.INT1IP = 6;
    IPC7bits.INT2IP = 6;
    IPC13bits.INT3IP = 6;

    IPC0bits.T1IP = 7; //trigger and max dist interrupt
    IPC1bits.T2IP = 7; //trigger interrupt for ultrasound sensor 0
    IPC6bits.T4IP = 7; //trigger interrupt for ultrasound sensor 1
    IPC11bits.T6IP = 7; //trigger interrupt for ultrasound sensor 2
    IPC12bits.T8IP = 7; //trigger interrupt for ultrasound sensor 3


    /* Clear all interrupts flags */
    IFS0 = 0;
    IFS1 = 0;
    IFS2 = 0;
    IFS3 = 0;
    IFS4 = 0;

    /* Unset all interrupts */
    IEC0 = 0;
    IEC1 = 0;
    IEC2 = 0;
    IEC3 = 0;
    IEC4 = 0;

    /* Set rising edge as the trigger for the external interrupts */
    INTCON2bits.INT0EP = 0;
    INTCON2bits.INT1EP = 0;
    INTCON2bits.INT2EP = 0;
    INTCON2bits.INT3EP = 0;

    /* Set timers interrupts */
//    IEC0bits.T1IE = 1; //timer 1
//    IEC0bits.T2IE = 1; //timer 2
//    IEC1bits.T4IE = 1; //timer 4
//    IEC2bits.T6IE = 1; //timer 6
//    IEC3bits.T8IE = 1; //timer 8
//
//    /* Set external interrupts */
//    IEC0bits.INT0IE = 1; //external interrupt 0 --> mapped to RF6  (pin 55)
//    IEC1bits.INT1IE = 1; //external interrupt 1 --> mapped to RA12 (pin 18)
//    IEC1bits.INT2IE = 1; //external interrupt 2 --> mapped to RA13 (pin 19)
//    IEC3bits.INT3IE = 1; //external interrupt 3 --> mapped to RA14 (pin 66)
}

/******************************************************************************/
/* Initializing timers 2, 4, 6 y 8                                            */
/* Habría que mirar si no es mejor utilizar 32, al fin y al cabo              */
/* no se pueden utilizar los dos timers emparejados de forma independiente    */

/******************************************************************************/
void initTimers(void) {
    //unset timer flags
    IFS0bits.T1IF = 0;
    IFS0bits.T2IF = 0;
    IFS1bits.T4IF = 0;
    IFS2bits.T6IF = 0;
    IFS3bits.T8IF = 0;

    // Init timer 1
    T1CONbits.TON = 0; //stop
    T1CONbits.TCKPS = 3; //1:1,8,64,256 (0,1,2,3) (2bits)
    T1CONbits.TCS = 0; //internal clock
    T1CONbits.TGATE = 0; //not working in gated mode (ignored if tcs = 1)
    T1CONbits.TSIDL = 1; //not working in idle mode
    T1CONbits.TSYNC = 1; //syncrhonous
    //TMR1 = 0;
    PR1 = TIME1_TRIGGER_MOTOR; //contador a 0

    // Init timer 2
    T2CONbits.TON = 0; //stop
    T2CONbits.T32 = 0; //timer con 16 bits
    T2CONbits.TCKPS = 0; //1:1,8,64,256 (0,1,2,3) (2bits)
    T2CONbits.TCS = 0; //internal clock
    T2CONbits.TGATE = 0; //not working in gated mode (ignored if tcs = 1)
    T2CONbits.TSIDL = 1; //not working in idle mode
    TMR2 = 0;
    PR2 = TIME1_MAX_SENSORS; //contador a 0

    // Init timer 4
    T4CONbits.TON = 0; //stop
    T4CONbits.T32 = 0; //timer con 16 bits
    T4CONbits.TCKPS = 0; //1:1,8,64,256 (0,1,2,3) (2bits)
    T4CONbits.TCS = 0; //internal clock
    T4CONbits.TGATE = 0; //not working in gated mode (ignored if tcs = 1)
    T4CONbits.TSIDL = 1; //not working in idle mode
    TMR4 = 0;
    PR4 = TIME1_MAX_SENSORS; //contador a 0

    // Init timer 6
    T6CONbits.TON = 0; //stop
    T6CONbits.T32 = 0; //timer con 16 bits
    T6CONbits.TCKPS = 0; //1:1,8,64,256 (0,1,2,3) (2bits)
    T6CONbits.TCS = 0; //internal clock
    T6CONbits.TGATE = 0; //not working in gated mode (ignored if tcs = 1)
    T6CONbits.TSIDL = 1; //not working in idle mode
    TMR6 = 0;
    PR6 = TIME1_MAX_SENSORS; //contador a 0

    // Init timer 8
    T8CONbits.TON = 0; //stop
    T8CONbits.T32 = 0; //timer con 16 bits
    T8CONbits.TCKPS = 0; //1:1,8,64,256 (0,1,2,3) (2bits)
    T8CONbits.TCS = 0; //internal clock
    T8CONbits.TGATE = 0; //not working in gated mode (ignored if tcs = 1)
    T8CONbits.TSIDL = 1; //not working in idle mode
    TMR8 = 0;
    PR8 = TIME1_MAX_SENSORS; //contador a 0
}

/******************************************************************************/
/* Rutina de inicialización de los sensores ultrasonicos y de los puertos     */
/* necesarios                                                                 */

/******************************************************************************/
void initSensors(void) {

    SensorsFinished = 0;

    /* Configure PORTA and PORTF as inputs for read sensors signals
     * RA12 = INT1
     * RA13 = INT2
     * RA14 = INT3
     * RF6  = INT0 */
    TRISA = 0;
    TRISAbits.TRISA12 = TRISAbits.TRISA13 = TRISAbits.TRISA14 = 1;
    TRISF = 0;
    TRISFbits.TRISF6 = 1;
    TRISBbits.TRISB0 = 0;
}

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */

void InitApp(void) {
    initInterrupts();

    initTimers();

    initSensors();
}

void moveMotor(unsigned int grade) {
//    TODO rutina para mover motores a un grado determinado
}

void Loop(void) {
//    T1CONbits.TON = 1;
    unsigned int iniTime0, iniTime1, iniTime2, iniTime3;
    unsigned int fiTime0, fiTime1, fiTime2, fiTime3;
    unsigned int time0, time1, time2, time3;
    float dist0, dist1, dist2, dist3;
    
    TRISBbits.TRISB0 = 0;
    
    PR1 = TIME1_TRIGGER_MOTOR;
    TMR1 = 0;

    //leds
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD9 = 0;
    TRISDbits.TRISD11 = 0;
    PORTDbits.RD0 = 1;
    PORTDbits.RD9 = 0;
    PORTDbits.RD11 = 0;

    //trigger at RB0
    //echo 0 at RA15
    //echo 1 at RA14
    //echo 2 at RF4 --> not working?
    //echo 3 at RF5 --> not working?
    TRISAbits.TRISA14 = 1;
    PORTAbits.RA14 = 0;
    TRISAbits.TRISA15 = 1;
    PORTAbits.RA15 = 0;
    TRISFbits.TRISF4 = 1;
    PORTFbits.RF4 = 0;
    TRISFbits.TRISF5 = 1;
    PORTFbits.RF5 = 0;

    while (ME_COMES_LOS_HUEVOS) {
        TMR1 = 0;
        PR1 = 0xFFFF;

        T1CONbits.TCKPS = 1;

        //Sensor 0 RA15
        PORTBbits.RB0 = 1;
        __delay_ms(10);
        PORTBbits.RB0 = 0;
        __delay_us(20);
        PORTBbits.RB0 = 1;

        while(!PORTAbits.RA15);
        
        T1CONbits.TON = 1;
        iniTime0 = 0;

        while(PORTAbits.RA15);

        fiTime0 = TMR1;
        T1CONbits.TON = 0;
        TMR1 = 0;

        __delay_us(100);

        //Sensor 1 RA14
        PORTBbits.RB0 = 1;
        __delay_ms(10);
        PORTBbits.RB0 = 0;
        __delay_us(20);
        PORTBbits.RB0 = 1;

        while(!PORTAbits.RA14);

        iniTime1 = 0;
        T1CONbits.TON = 1;

        while(PORTAbits.RA14);

        fiTime1 = TMR1;
        T1CONbits.TON = 0;
        TMR1 = 0;

        __delay_us(100);

        //Sensor 2 RF4
        PORTBbits.RB0 = 1;
        __delay_ms(10);
        PORTBbits.RB0 = 0;
        __delay_us(20);
        PORTBbits.RB0 = 1;

        while(!PORTFbits.RF4);

        T1CONbits.TON = 1;
        iniTime2 = 0;

        while(PORTFbits.RF4);

        fiTime2 = TMR1;
        T1CONbits.TON = 0;
        TMR1 = 0;

        __delay_us(100);

        //Sensor 2 RF5
        PORTBbits.RB0 = 1;
        __delay_ms(10);
        PORTBbits.RB0 = 0;
        __delay_us(20);
        PORTBbits.RB0 = 1;

        while(!PORTFbits.RF5);

        iniTime3 = 0;
        T1CONbits.TON = 1;

        while(PORTFbits.RF5);

        fiTime3 = TMR1;
        T1CONbits.TON = 0;
        TMR1 = 0;

        __delay_us(100);

        time0 = fiTime0 - iniTime0;
        time1 = fiTime1 - iniTime1;
        time2 = fiTime2 - iniTime2;
        time3 = fiTime3 - iniTime3;
        
        dist0 = ((float)time0)/28.0;
        dist1 = ((float)time1)/28.0;
        dist2 = ((float)time2)/28.0;
        dist3 = ((float)time3)/28.0;

        __delay_ms(500);
    }

}

/* TODO solo deberá girar los sensores, poner un 1 en RA11 y activar timer1,  */
/* y esperar a que lleguen las interrupciones                                 */
uint16_t get_sensor(uint16_t id) {
    return 1;
}
