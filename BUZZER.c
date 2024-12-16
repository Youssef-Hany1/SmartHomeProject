#include "BUZZER.h"
#include "STD_TYPES.h"

void BUZZER_Init(char port, uint8 pin)
{
  dio_init(port, pin, Output);
}

void BUZZER_Control(char port, uint8 pin, uint8 value)
{
  if(value == High){
    dio_writepin(port, pin, High);
  } else if(value == Low){
    dio_writepin(port, pin, Low);
  }
}
