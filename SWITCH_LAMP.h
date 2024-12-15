#ifndef SWITCH_LAMP_H
#define SWITCH_LAMP_H

#include "STD_TYPES.h"
#include "tm4c123gh6pm.h"
#include "DIO.h"

#define SWITCH_pin Pin0
#define SWITCH_port 'F'

void SWITCH_LAMP_Init (void (*callback)(void));

#endif