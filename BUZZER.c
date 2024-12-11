#include "BUZZER.h"
#include "STD_TYPES.h"

void BUZZER_Init(void)
{
  dio_init(buzzer_port, buzzer_pin, Output);
}

void BUZZER_Control(uint8 value)
{
  if(value == High){
    dio_writepin(buzzer_port, buzzer_pin, High);
  } else if(value == Low){
    dio_writepin(buzzer_port, buzzer_pin, Low);
  }
}
