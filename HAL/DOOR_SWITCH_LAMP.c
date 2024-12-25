#include "SWITCH_LAMP.h"
#include "DOOR.h"
#include "..\UTILES_LIB\BIT_WISE_OPS.h"
#include "..\tm4c123gh6pm.h"

static void (*SWITCH_LAMP_Callback)(void) = 0;
static void (*DOOR_Callback)(void) = 0;

void SWITCH_LAMP_Init(void (*callback)(void)) {
    // Unlock PF4 (it is locked by default)
    GPIO_PORTF_LOCK_R = 0x4C4F434B; // Unlock GPIO Port F
    GPIO_PORTF_CR_R |= 0x10;        // Allow changes to PF4

    dio_init(SWITCH_port, SWITCH_pin, Input);  // Initialize the switch pin as input

    // Configure PF4 for edge-sensitive interrupt on both edges
    CLR_BIT(GPIO_PORTF_IS_R, 4);   // Edge-sensitive
    SET_BIT(GPIO_PORTF_IBE_R, 4);  // Interrupt on both edges
    SET_BIT(GPIO_PORTF_ICR_R, 4);  // Clear any prior interrupt
    SET_BIT(GPIO_PORTF_IM_R, 4);   // Unmask interrupt for PF4
    SET_BIT(GPIO_PORTF_PUR_R, 4);   // Enable pull-up resistors
    

    // Enable interrupt for Port F in NVIC
    NVIC_EN0_R |= (1 << 30);    // Enable interrupt #30 (Port F)

    // Set the callback function
    SWITCH_LAMP_Callback = callback;
}

void DOOR_Init(void (*callback)(void)) {
    // Unlock PF0 (it is locked by default)
    GPIO_PORTF_LOCK_R = 0x4C4F434B; // Unlock GPIO Port F
    GPIO_PORTF_CR_R |= 0x01;        // Allow changes to PF0

    dio_init(door_port, door_pin, Input);  // Initialize the door pin as input

    // Configure PF0 for edge-sensitive interrupt on both edges
    CLR_BIT(GPIO_PORTF_IS_R, 0);   // Edge-sensitive
    SET_BIT(GPIO_PORTF_IBE_R, 0);  // Interrupt on both edges
    SET_BIT(GPIO_PORTF_ICR_R, 0);  // Clear any prior interrupt
    SET_BIT(GPIO_PORTF_IM_R, 0);   // Unmask interrupt for PF0
    SET_BIT(GPIO_PORTF_PUR_R, 0);   // Enable pull-up resistors

    // Enable interrupt for Port F in NVIC
    NVIC_EN0_R |= (1 << 30);    // Enable interrupt #30 (Port F)

    // Set the callback function
    DOOR_Callback = callback;
}

void GPIOF_Handler(void) {
    if (SWITCH_LAMP_Callback != 0 && (GPIO_PORTF_MIS_R & 0x10)) {
        // Call the provided callback function
        SWITCH_LAMP_Callback();

        for (volatile int i = 0; i < 50000; i++); // Debouncing delay

        // Clear the interrupt flag for PF4
        GPIO_PORTF_ICR_R |= 0x10;
    }

    if (DOOR_Callback != 0 && (GPIO_PORTF_MIS_R & 0x01)) {
        // Call the provided callback function
        DOOR_Callback();

        for (volatile int i = 0; i < 50000; i++); // Debouncing delay

        // Clear the interrupt flag for PF0
        GPIO_PORTF_ICR_R |= 0x01;
    }
}
