#include "SYS_TICK_interface.h"
#include "DIO.h"

int main()
{
  dio_init('F', Pin1, Output);
  SysTick_Init (15999999);
  while(1)
  {
    dio_writepin('F', Pin1, 1);
    while(!SysTick_Is_Time_Out());
    dio_writepin('F', Pin1, 0);
    while(!SysTick_Is_Time_Out());
  }
}