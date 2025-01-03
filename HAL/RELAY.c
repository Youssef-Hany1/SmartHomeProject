#include "RELAY.h"
#include "..\MCAL\DIO.h"
#include "..\UTILES_LIB\STD_TYPES.h"

// Initialize the relay pins
void Relay_Init(char port, uint8 pin) {
    dio_init(port, pin, Output);
}

// Control the relay state (ON/OFF)
void Relay_Control(char port, uint8 pin, uint8 state) {
    dio_writepin(port, pin, state);
}
