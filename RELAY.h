#ifndef RELAY_H
#define RELAY_H

#include "STD_TYPES.h"
#include "DIO.h"

#define RELAY1_PORT     'F'
#define RELAY1_PIN      Pin1

#define RELAY2_PORT     'F'
#define RELAY2_PIN      Pin2

// Relay States
#define RELAY_ON  1
#define RELAY_OFF 0

// Function Prototypes
void Relay_Init(char port, uint8 pin);
void Relay_Control(char port, uint8 pin, uint8 state);

#endif // RELAY_H
