#include "SYS_TICK_interface.h"
#include "STD_TYPES.h"
#include "Relay.h"
#include "DOOR.h"
#include "SWITCH_LAMP.h"
#include "BUZZER.h"
#include "UART.h"
#include "LED.h"
#include "DIO.h"
#include "tm4c123gh6pm.h"
#include "ADC.h"
#include <stdio.h>

// Global Variables
volatile uint8 received_char = 0;

volatile uint8 LAMP_STATUS = 0;
volatile uint8 LAMP_SWITCH = 0;
uint8 LAMP_UART = 0;

// Callback function for Buzzer Systick interrupt
void BUZZER_SYSTICK_Callback(void) {
    uint32 adcValue = ADC1_ReadValue(); // Read raw ADC value
    float temperature = ADC_To_Temperature(adcValue); // Convert to temperature

    char temp[10]; // Adjusted buffer size for formatted string
    sprintf(temp, "t_%.1f#", temperature); // Format temperature with one decimal point
    UART0_TransmitString(temp); // Transmit temperature string over UART

    // Control buzzer based on temperature threshold
    if (temperature > TEMPERATURE_THRESHOLD) {
        BUZZER_Control(High); // Turn buzzer ON
        UART0_TransmitChar('5'); // Send indication for high temperature
    } else {
        BUZZER_Control(Low); // Turn buzzer OFF
        UART0_TransmitChar('4'); // Send indication for normal temperature
    }
}

// Callback function for Door interrupt
void DOOR_Callback(void) {
  if (!(GPIO_PORTF_DATA_R & 0x01)) {
      UART0_TransmitChar('3');
  } else {
      UART0_TransmitChar('2');
  }
}

// Callback function for UART0 interrupt
void UART0_Callback(void) {
  received_char = (char)(UART0_DR_R & 0xFF); // Read the received character
}

// Callback function for Switch interrupt
void SWITCH_LAMP_Callback(void) {
  if (!(GPIO_PORTF_DATA_R & 0x10)) {
      LAMP_SWITCH=1;
  } else {
      LAMP_SWITCH=0;
  }
}

int main(void) {
    Relay_Init(RELAY1_PORT, RELAY1_PIN);
    Relay_Init(RELAY2_PORT, RELAY2_PIN);
    DOOR_Init(DOOR_Callback);
    SWITCH_LAMP_Init(SWITCH_LAMP_Callback);
    ADC1_Init();
    UART0_Init(UART0_Callback);
    BUZZER_Init(BUZZER_SYSTICK_Callback);
    
    // Main loop
    while (1) {
      
      LAMP_STATUS = LAMP_UART^LAMP_SWITCH;
      if(LAMP_STATUS){
        //UART0_TransmitChar('1');
        Relay_Control(RELAY1_PORT, RELAY1_PIN, RELAY_ON);
      }else{
        //UART0_TransmitChar('0');
        Relay_Control(RELAY1_PORT, RELAY1_PIN, RELAY_OFF);
      }
      
      if (received_char != 0) {
          char c = received_char;
          UART0_TransmitChar(c);
          received_char = 0; // Clear the received character
          
          switch (c) {
            case '0': //LAMP OFF
                LAMP_UART=0;
                break;
            case '1': //LAMP ON
                LAMP_UART=1;
                break;
            case '2': //BLUG OFF
                Relay_Control(RELAY2_PORT, RELAY2_PIN, RELAY_OFF);
                break;
            case '3': //BLUG ON
                Relay_Control(RELAY2_PORT, RELAY2_PIN, RELAY_ON);
                break;    
            default:
                break;
          }
      }
    }
}
