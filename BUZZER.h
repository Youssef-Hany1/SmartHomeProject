#ifndef BUZZER_H
#define BUZZER_H

#include "tm4c123gh6pm.h"
#include "DIO.h"

#define buzzer_pin Pin7
#define buzzer_port 'A'

#define High 1
#define Low 0

void BUZZER_Init(void);
void BUZZER_Control(uint8 value);

#endif