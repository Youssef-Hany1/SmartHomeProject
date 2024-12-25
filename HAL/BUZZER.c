#include "BUZZER.h"
#include "..\UTILES_LIB\STD_TYPES.h"
#include "..\MCAL\SYS_TICK_interface.h"

// Function to initialize the buzzer with a SysTick interrupt callback
void BUZZER_Init(void (*callback)(void))
{
  if(callback != 0) { // Ensure the callback function is not null
    dio_init(buzzer_port, buzzer_pin, Output); // Configure the buzzer pin as an output
    SysTick_InitInterrupt(15999999, callback); // Initialize SysTick with a specified interval and callback
  }
}

// Function to control the buzzer state (ON/OFF)
void BUZZER_Control(uint8 state)
{
  dio_writepin(buzzer_port, buzzer_pin, state); // Write the desired state to the buzzer pin
}
