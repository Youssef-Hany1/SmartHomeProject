#include "tm4c123gh6pm.h"
#include "STD_TYPES.h"
#include "UART.h"

static void (*UART0_Callback) (void) = 0;

void UART0_Init(void (*callback)(void)) {
    SYSCTL_RCGCUART_R |= (1 << 0); // Enable clock for UART0
    SYSCTL_RCGCGPIO_R |= (1 << 0); // Enable clock for GPIOA
    while ((SYSCTL_PRUART_R & (1 << 0)) == 0);
    while ((SYSCTL_PRGPIO_R & (1 << 0)) == 0);
    
    GPIO_PORTA_AFSEL_R |= (1 << 0) | (1 << 1);
    GPIO_PORTA_PCTL_R &= ~0xFF;
    GPIO_PORTA_PCTL_R |= (1 << 0) | (1 << 4);
    GPIO_PORTA_DEN_R |= (1 << 0) | (1 << 1);
    GPIO_PORTA_AMSEL_R &= ~((1 << 0) | (1 << 1));
    
    UART0_CTL_R &= ~(1 << 0); // Disable UART0
    UART0_IBRD_R = 104;       // Integer part for 9600 baud
    UART0_FBRD_R = 11;        // Fractional part for 9600 baud
    UART0_LCRH_R = (0x3 << 5); // 8-bit word length, no parity, 1 stop bit
    UART0_CC_R = 0x0;         // Use system clock
    UART0_IM_R |= (1 << 4);   // Enable RX interrupt
    NVIC_EN0_R |= (1 << 5);   // Enable IRQ5 for UART0 in NVIC
    UART0_CTL_R |= (1 << 0) | (1 << 8) | (1 << 9); // Enable UART0, Tx, Rx
    
    UART0_Callback = callback;
}

void UART0_TransmitChar(char c) {
    while ((UART0_FR_R & (1 << 5)) != 0); // Wait until the transmit FIFO is not full
    UART0_DR_R = c; // Write the character to the data register
}

void UART0_TransmitString(const char *str) {
    while (*str) {
        UART0_TransmitChar(*str++);
    }
}

// Interrupt Service Routine for UART0
void UART0_Handler(void) {
  if(UART0_Callback != 0){
    if (UART0_MIS_R & (1 << 4)) { // Check if RX interrupt occurred
        UART0_Callback();
        UART0_ICR_R |= (1 << 4); // Clear RX interrupt flag
    }
  }
}