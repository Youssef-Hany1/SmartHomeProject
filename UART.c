#include "tm4c123gh6pm.h"
#include "STD_TYPES.h"
#include "UART.h"

// Static variable to hold the user-defined callback function for UART0 interrupt
static void (*UART0_Callback) (void) = 0;

// Function to initialize UART0 with a callback function for RX interrupts
void UART0_Init(void (*callback)(void)) {
    SYSCTL_RCGCUART_R |= (1 << 0); // Enable clock for UART0
    SYSCTL_RCGCGPIO_R |= (1 << 0); // Enable clock for GPIOA
    while ((SYSCTL_PRUART_R & (1 << 0)) == 0); // Wait for UART0 to be ready
    while ((SYSCTL_PRGPIO_R & (1 << 0)) == 0); // Wait for GPIOA to be ready

    GPIO_PORTA_AFSEL_R |= (1 << 0) | (1 << 1); // Enable alternate function on PA0 and PA1
    GPIO_PORTA_PCTL_R &= ~0xFF;               // Clear PCTL register bits for PA0 and PA1
    GPIO_PORTA_PCTL_R |= (1 << 0) | (1 << 4); // Configure PA0 and PA1 for UART functionality
    GPIO_PORTA_DEN_R |= (1 << 0) | (1 << 1);  // Enable digital functionality on PA0 and PA1
    GPIO_PORTA_AMSEL_R &= ~((1 << 0) | (1 << 1)); // Disable analog functionality on PA0 and PA1

    UART0_CTL_R &= ~(1 << 0); // Disable UART0 during configuration
    UART0_IBRD_R = 104;       // Set integer part of the baud rate (9600 baud)
    UART0_FBRD_R = 11;        // Set fractional part of the baud rate (9600 baud)
    UART0_LCRH_R = (0x3 << 5); // Configure for 8-bit word length, no parity, 1 stop bit
    UART0_CC_R = 0x0;         // Use system clock for UART
    UART0_IM_R |= (1 << 4);   // Enable RX interrupt
    NVIC_EN0_R |= (1 << 5);   // Enable IRQ5 for UART0 in NVIC
    UART0_CTL_R |= (1 << 0) | (1 << 8) | (1 << 9); // Enable UART0, Tx, and Rx

    UART0_Callback = callback; // Store the user-defined callback function
}

// Function to transmit a single character over UART0
void UART0_TransmitChar(char c) {
    while ((UART0_FR_R & (1 << 5)) != 0); // Wait until the transmit FIFO is not full
    UART0_DR_R = c; // Write the character to the data register
}

// Function to transmit a string over UART0
void UART0_TransmitString(const char *str) {
    while (*str) { // Iterate through each character in the string
        UART0_TransmitChar(*str++); // Transmit the current character
    }
}

// Interrupt Service Routine (ISR) for UART0
void UART0_Handler(void) {
  if (UART0_Callback != 0) { // Check if a callback function is defined
    if (UART0_MIS_R & (1 << 4)) { // Check if RX interrupt occurred
        UART0_Callback();         // Call the user-defined callback function
        UART0_ICR_R |= (1 << 4);  // Clear the RX interrupt flag
    }
  }
}
