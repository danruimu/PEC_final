#include "definitions.h"
#include <stdint.h>
/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

void InitApp(void);         /* I/O and Peripheral Initialization */
void Loop(void);/*Es la funcion que hace el w(1)*/
uint16_t get_sensor(uint16_t id); /* prendre dades del sensor id */
void pilla_distancies(uint16_t angle,array* distancies);/* Omple les posicions de distancies corresponent a aquell angle*/
void mapeja(uint16_t angle,array* distancies,map* mapa);/* Passa de distancies a mapa*/
int8_t avalua(map* mapa,Vector orientacio);/* El que torna son els graus que s'han de girar per a mirar l'objectiu*/

