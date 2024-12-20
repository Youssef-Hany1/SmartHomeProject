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
volatile uint8 LAMP_STATUS = 0; // Current lamp status
volatile uint8 LAMP_SWITCH = 0; // Lamp status via switch
uint8 LAMP_UART = 0;            // Lamp status via UART
uint8 LAST_LAMP_STATUS = 0xFF;  // Track last sent lamp status to prevent redundant UART messages
uint8 BUZZER_STATUS = 0;        // Track the current buzzer state (0: OFF, 1: ON)

// Callback function for Buzzer Systick interrupt
void BUZZER_SYSTICK_Callback(void) {
    uint32 adcValue = ADC1_ReadValue();            // Read raw ADC value
    float temperature = ADC_To_Temperature(adcValue); // Convert to temperature

    char temp[10];                                 // Buffer for temperature string
    sprintf(temp, "t_%.1f#", temperature);         // Format temperature
    UART0_TransmitString(temp);                    // Transmit temperature string via UART

    // Control buzzer based on temperature threshold
    if (temperature > TEMPERATURE_THRESHOLD && BUZZER_STATUS == 0) {
        BUZZER_Control(High);                      // Turn buzzer ON
        UART0_TransmitChar('f');                   // Send high-temperature indication
        BUZZER_STATUS = 1;                         // Update buzzer status
    } else if (temperature <= TEMPERATURE_THRESHOLD && BUZZER_STATUS == 1) {
        BUZZER_Control(Low);                       // Turn buzzer OFF
        UART0_TransmitChar('e');                   // Send normal-temperature indication
        BUZZER_STATUS = 0;                         // Update buzzer status
    }
}

// Callback function for Door interrupt
void DOOR_Callback(void) {
    if (!(GPIO_PORTF_DATA_R & 0x01)) {
        UART0_TransmitChar('d'); // Door open
    } else {
        UART0_TransmitChar('c'); // Door closed
    }
}

// Callback function for UART0 interrupt
void UART0_Callback(void) {
    received_char = (char)(UART0_DR_R & 0xFF); // Read the received character
}

// Callback function for Switch interrupt
void SWITCH_LAMP_Callback(void) {
    if (!(GPIO_PORTF_DATA_R & 0x10)) {
        LAMP_SWITCH = 1; // Switch pressed
    } else {
        LAMP_SWITCH = 0; // Switch released
    }
}

// Main function
int main(void) {
    // Initialize components
    Relay_Init(RELAY1_PORT, RELAY1_PIN);
    Relay_Init(RELAY2_PORT, RELAY2_PIN);
    DOOR_Init(DOOR_Callback);
    SWITCH_LAMP_Init(SWITCH_LAMP_Callback);
    ADC1_Init();
    UART0_Init(UART0_Callback);
    BUZZER_Init(BUZZER_SYSTICK_Callback);

    // Main loop
    while (1) {
        // Calculate current lamp status based on UART and switch inputs
        LAMP_STATUS = LAMP_UART ^ LAMP_SWITCH;

        // Send status only if it has changed
        if (LAMP_STATUS != LAST_LAMP_STATUS) {
            LAST_LAMP_STATUS = LAMP_STATUS; // Update the last known status

            if (LAMP_STATUS) {
                UART0_TransmitChar('b'); // Lamp ON
                Relay_Control(RELAY1_PORT, RELAY1_PIN, RELAY_ON);
            } else {
                UART0_TransmitChar('a'); // Lamp OFF
                Relay_Control(RELAY1_PORT, RELAY1_PIN, RELAY_OFF);
            }
        }

        // Handle received UART characters
        if (received_char != 0) {
            char c = received_char; // Store received character
            received_char = 0;      // Clear the received character

            switch (c) {
                case '0': // LAMP OFF
                    LAMP_UART = 0;
                    break;

                case '1': // LAMP ON
                    LAMP_UART = 1;
                    break;

                case '8': // PLUG OFF
                    Relay_Control(RELAY2_PORT, RELAY2_PIN, RELAY_OFF);
                    break;

                case '9': // PLUG ON
                    Relay_Control(RELAY2_PORT, RELAY2_PIN, RELAY_ON);
                    break;

                default:
                    break;
            }
        }
    }
}
