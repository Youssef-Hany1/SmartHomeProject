#include "..\MCAL\SYS_TICK_interface.h"
#include "..\UTILES_LIB\STD_TYPES.h"
#include "..\HAL\Relay.h"
#include "..\HAL\DOOR.h"
#include "..\HAL\SWITCH_LAMP.h"
#include "..\HAL\BUZZER.h"
#include "..\MCAL\UART.h"
#include "..\MCAL\DIO.h"
#include "..\tm4c123gh6pm.h"
#include "..\MCAL\ADC.h"
#include <stdio.h>

// Global Variables
volatile uint8 received_char = 0;    // Stores the last received UART character
volatile uint8 LAMP_STATUS = 0;      // Current lamp status (ON/OFF)
volatile uint8 LAMP_SWITCH = 0;      // Lamp status controlled by the physical switch
uint8 LAMP_UART = 0;                 // Lamp status controlled via UART
uint8 LAST_LAMP_STATUS = 0xFF;       // Last sent lamp status to avoid redundant UART messages
uint8 BUZZER_STATUS = 0;             // Current buzzer state (0: OFF, 1: ON)

// Callback function for the buzzer SysTick interrupt
void BUZZER_SYSTICK_Callback(void) {
    uint32 adcValue = ADC1_ReadValue();            // Read raw ADC value
    float temperature = ADC_To_Temperature(adcValue); // Convert ADC value to temperature

    char temp[10];                                 // Buffer to store the temperature string
    sprintf(temp, "t_%.1f#", temperature);         // Format temperature for UART transmission
    UART0_TransmitString(temp);                    // Send temperature data via UART

    // Control buzzer based on the temperature threshold
    if (temperature > TEMPERATURE_THRESHOLD && BUZZER_STATUS == 0) {
        BUZZER_Control(High);                      // Turn the buzzer ON
        UART0_TransmitChar('f');                   // Notify high-temperature state via UART
        BUZZER_STATUS = 1;                         // Update buzzer status to ON
    } else if (temperature <= TEMPERATURE_THRESHOLD && BUZZER_STATUS == 1) {
        BUZZER_Control(Low);                       // Turn the buzzer OFF
        UART0_TransmitChar('e');                   // Notify normal-temperature state via UART
        BUZZER_STATUS = 0;                         // Update buzzer status to OFF
    }
}

// Callback function for the door status interrupt
void DOOR_Callback(void) {
    if (!(GPIO_PORTF_DATA_R & 0x01)) {
        UART0_TransmitChar('d'); // Send 'd' when the door is open
    } else {
        UART0_TransmitChar('c'); // Send 'c' when the door is closed
    }
}

// Callback function for UART0 interrupt
void UART0_Callback(void) {
    received_char = (char)(UART0_DR_R & 0xFF); // Read and store the received UART character
}

// Callback function for the lamp switch interrupt
void SWITCH_LAMP_Callback(void) {
    if (!(GPIO_PORTF_DATA_R & 0x10)) {
        LAMP_SWITCH = 1; // Lamp switch is pressed
    } else {
        LAMP_SWITCH = 0; // Lamp switch is released
    }
}

// Main function
int main(void) {
    // Initialize all components
    Relay_Init(RELAY1_PORT, RELAY1_PIN);          // Initialize Relay 1 (Lamp control)
    Relay_Init(RELAY2_PORT, RELAY2_PIN);          // Initialize Relay 2 (Plug control)
    DOOR_Init(DOOR_Callback);                     // Initialize door with its callback
    SWITCH_LAMP_Init(SWITCH_LAMP_Callback);       // Initialize lamp switch with its callback
    ADC1_Init();                                  // Initialize ADC module
    UART0_Init(UART0_Callback);                   // Initialize UART0 with its callback
    BUZZER_Init(BUZZER_SYSTICK_Callback);         // Initialize buzzer with SysTick callback

    // Main loop
    while (1) {
        // Determine the current lamp status based on UART and switch inputs
        LAMP_STATUS = LAMP_UART ^ LAMP_SWITCH;

        // Send status change only if it differs from the last known status
        if (LAMP_STATUS != LAST_LAMP_STATUS) {
            LAST_LAMP_STATUS = LAMP_STATUS; // Update the last known lamp status

            if (LAMP_STATUS) {
                UART0_TransmitChar('b'); // Notify Lamp ON via UART
                Relay_Control(RELAY1_PORT, RELAY1_PIN, RELAY_ON); // Turn lamp ON
            } else {
                UART0_TransmitChar('a'); // Notify Lamp OFF via UART
                Relay_Control(RELAY1_PORT, RELAY1_PIN, RELAY_OFF); // Turn lamp OFF
            }
        }

        // Process received UART characters
        if (received_char != 0) {
            char c = received_char; // Store the received character locally
            received_char = 0;      // Clear the received character buffer

            switch (c) {
                case '0': // Command to turn the lamp OFF
                    LAMP_UART = 0;
                    break;

                case '1': // Command to turn the lamp ON
                    LAMP_UART = 1;
                    break;

                case '8': // Command to turn the plug OFF
                    Relay_Control(RELAY2_PORT, RELAY2_PIN, RELAY_OFF);
                    break;

                case '9': // Command to turn the plug ON
                    Relay_Control(RELAY2_PORT, RELAY2_PIN, RELAY_ON);
                    break;

                default:
                    break; // Ignore invalid commands
            }
        }
    }
}
