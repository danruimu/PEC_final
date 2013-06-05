#ifndef DEFINITIONS_H
#define	DEFINITIONS_H

#include "system.h"

#define ME_COMES_LOS_HUEVOS 1
/* indefinido para mover el motor */
#define TIME1_TRIGGER_MOTOR  0x7FFF
/* 10us para activar el ultrasonido */
#define TIME1_TRIGGER_SENSOR 5
/* Tiempo necesario para decidir si tenemos algo delante o no*/
#define TIME1_MAX_SENSORS    13875
/* Máximo valor para los timers 2,4,6,8 */
#define MAX_TIMER_SENS  0xFFFF

#define GIR 15
#define NUM_SENSORS 4
#define MAX_DIST 401

typedef char** map;
typedef short* array;
typedef struct{
    double x,y;
}Vector;

typedef char* Sflags;
#endif	/* DEFINITIONS_H */