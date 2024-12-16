#include "RELAY.h"
#include "DIO.h"
#include "STD_TYPES.h"

// Initialize the relay pins
void Relay_Init(char port, uint8 pin) {
    dio_init(port, pin, Output);
}

// Control the relay state (ON/OFF)
void Relay_Control(char port, uint8 pin, uint8 state) {
    if (state == RELAY_ON) {
        dio_writepin(port, pin, RELAY_ON); // Activate relay
    } else if (state == RELAY_OFF) {
        dio_writepin(port, pin, RELAY_OFF); // Deactivate relay
    }
}
