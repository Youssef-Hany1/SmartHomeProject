#ifndef DOOR_H
#define DOOR_H

#include "STD_TYPES.h"
#include "tm4c123gh6pm.h"
#include "DIO.h"

#define door_pin Pin0
#define door_port 'F'

void DOOR_Init (void (*callback)(void));

#endif