#include "SYS_TICK_interface.h"
#include "Relay.h"
#include "DOOR.h"
#include "LED.h"


// Callback function for SysTick interrupt
void DOOR_Callback(void) {
  if (!(GPIO_PORTF_DATA_R & 0x10)) {
      LED_On('F', Pin1);
  } else {
      LED_Off('F', Pin1);
  }
}

int main(void) {
    // Initialize the relay on Port F, Pin 1
    LED_Init('F', Pin1);
    DOOR_Init(DOOR_Callback);

    // Initialize SysTick with interrupt mode and 1-second interval
    // Using a 16 MHz clock, 1 second corresponds to 16,000,000 cycles
    //SysTick_InitInterrupt(16000000, SysTick_Callback);

    // Main loop
    while (1) {
        // Main loop remains empty as the SysTick interrupt handles relay toggling
    }
}
