#include "LED.h"

void LED_Init (char port, uint8 pin)
{
  dio_init(port,pin,Output);
}

void LED_On (char port, uint8 pins)
{
  dio_writeport(port,dio_readport(port) | pins);
}

void LED_Off (char port, uint8 pins)
{
  dio_writeport(port,dio_readport(port) & ~pins);
}

void LED_Toggle (char port, uint8 pins)
{
  dio_writeport(port,dio_readport(port) ^ pins);
}

void LED_Blank (char port, uint8 pin, uint32 delayMs)
{
  LED_On(port,pin);
  SysTick_Init ((delayMs*16000)-1);
  while(!SysTick_Is_Time_Out());
  LED_Off(port,pin);
  SysTick_Init ((delayMs*16000)-1);
  while(!SysTick_Is_Time_Out());
}