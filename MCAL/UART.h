#ifndef UART_H
#define UART_H

#include "..\UTILES_LIB\STD_TYPES.h"
#include "..\tm4c123gh6pm.h"

// Function Prototypes
void UART0_Init(void (*callback)(void));
void UART0_TransmitChar(char c);
void UART0_TransmitString(const char *str);

#endif