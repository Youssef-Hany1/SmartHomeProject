#ifndef LED_H
#define LED_H

#include <stdint.h>
#include "STD_TYPES.h"
#include "DIO.h"
#include "SYS_TICK_interface.h"

#define LED_ON          1
#define LED_OFF         0

void LED_Init (char port, uint8 pin);
void LED_On (char port, uint8 pins);
void LED_Off (char port, uint8 pins);
void LED_Toggle (char port, uint8 pins);
void LED_Blank (char port, uint8 pin, uint32 delayMs);

#endif