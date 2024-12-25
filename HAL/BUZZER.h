#ifndef BUZZER_H
#define BUZZER_H

#include "..\tm4c123gh6pm.h"
#include "..\UTILES_LIB\STD_TYPES.h"
#include "..\MCAL\DIO.h"

#define buzzer_pin Pin3
#define buzzer_port 'F'

#define High 1
#define Low 0

void BUZZER_Init(void (*callback)(void));
void BUZZER_Control(uint8 state);

#endif