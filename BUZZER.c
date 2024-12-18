#include "BUZZER.h"
#include "STD_TYPES.h"
#include "SYS_TICK_interface.h"

void BUZZER_Init(void (*callback)(void))
{
  if(callback != 0){
    dio_init(buzzer_port, buzzer_pin, Output);
    SysTick_InitInterrupt(15999999,callback);
  }
}

void BUZZER_Control(uint8 state)
{
  dio_writepin(buzzer_port, buzzer_pin, state);
}
