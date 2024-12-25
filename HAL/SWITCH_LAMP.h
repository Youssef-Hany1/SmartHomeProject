#ifndef SWITCH_LAMP_H
#define SWITCH_LAMP_H

#include "..\UTILES_LIB\STD_TYPES.h"
#include "..\tm4c123gh6pm.h"
#include "..\MCAL\DIO.h"

#define SWITCH_pin Pin4
#define SWITCH_port 'F'

void SWITCH_LAMP_Init (void (*callback)(void));

#endif