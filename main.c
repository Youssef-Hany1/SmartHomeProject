#include "SYS_TICK_interface.h"
#include "STD_TYPES.h"
#include "Relay.h"
#include "DOOR.h"
#include "SWITCH_LAMP.h"
#include "BUZZER.h"
#include "TEMP_SENSOR.h"
#include "UART.h"
#include "LED.h"
#include "DIO.h"
#include "tm4c123gh6pm.h"

// Global Variables
volatile uint8 received_char = 0;

// Callback function for SysTick interrupt
void DOOR_Callback(void) {
  if (!(GPIO_PORTF_DATA_R & 0x10)) {
      LED_On('F', Pin2);
  } else {
      LED_Off('F', Pin2);
  }
}

// Callback function for UART0 interrupt
void UART0_Callback(void) {
  received_char = (uint8)(UART0_DR_R & 0xFF); // Read the received character
}

// Callback function for Switch interrupt
void SWITCH_LAMP_Callback(void) {
  if (!(GPIO_PORTF_DATA_R & 0x01)) {
      LED_Toggle('F', Pin1);
  } else {
      LED_Toggle('F', Pin1);
  }
}

// Callback function for Temperature Sensor interrupt
void Temperature_Callback(Vfloat32 temperature) {
    if (temperature > TEMPERATURE_THRESHOLD) {
        GPIO_PORTF_DATA_R |= 0x02; // Turn on the Red LED
    } else {
        GPIO_PORTF_DATA_R &= ~0x02; // Turn off the Red LED
    }
}

int main(void) {
    // Initialize the relay on Port F, Pin 1
    LED_Init('F', Pin1);
    LED_Init('F', Pin2);
    LED_Init('F', Pin3);
    DOOR_Init(DOOR_Callback);
    SWITCH_LAMP_Init(SWITCH_LAMP_Callback);
    ADC_Init(Temperature_Callback);
    UART0_Init(UART0_Callback);

    // Main loop
    while (1) {
      if (received_char != 0) {
            char c = received_char;
	    UART0_TransmitChar(c);
            received_char = 0; // Clear the received character
            
            // Control the LEDs based on received character
            switch (c) {
            case 'r':
                GPIO_PORTF_DATA_R = 0x2;
                break;
            case 'b':
                GPIO_PORTF_DATA_R = 0x4;
                break;
            case 'g':
                GPIO_PORTF_DATA_R = 0x8;
                break;
            case 'x':
                GPIO_PORTF_DATA_R = 0x00;
                break;
            case 'w':
                GPIO_PORTF_DATA_R = 0x2 | 0x4 | 0x8;
                break;
            default:
                break;
            }
        }
    }
}
