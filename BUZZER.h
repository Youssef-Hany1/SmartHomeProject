#ifndef BUZZER_H
#define BUZZER_H

#include "tm4c123gh6pm.h"
#include "STD_TYPES.h"
#include "DIO.h"

#define buzzer_pin Pin3
#define buzzer_port 'F'

#define High 1
#define Low 0

void BUZZER_Init(char port, uint8 pin);
void BUZZER_Control(char port, uint8 pin, uint8 value);

#endif