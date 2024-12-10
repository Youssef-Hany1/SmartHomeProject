#include "SYS_TICK_interface.h"
#include "BIT_WISE_OPS.h"
#include "tm4c123gh6pm.h"
#include "STD_TYPES.h"

static void (*SysTick_Callback) (void) = 0;

void SysTick_Init (uint32 reloadValue)
{
  NVIC_ST_CTRL_R = 0;
  NVIC_ST_RELOAD_R = reloadValue;
  NVIC_ST_CURRENT_R = 0;
  NVIC_ST_CTRL_R = 0x05;
}

void SysTick_InitInterrupt (uint32 reloadValue, void (*callback) (void))
{
  NVIC_ST_CTRL_R = 0;
  NVIC_ST_RELOAD_R = reloadValue;
  NVIC_ST_CURRENT_R = 0;
  SysTick_Callback = callback;
  NVIC_ST_CTRL_R = 0x07;
}

void SysTick_Enable (void)
{
  SET_BIT(NVIC_ST_CTRL_R,0);
}

void SysTick_Disable (void)
{
  CLR_BIT(NVIC_ST_CTRL_R,0);
}

void SysTick_PeriodSet (uint32 ui32PeriodValue)
{
  NVIC_ST_RELOAD_R = ui32PeriodValue;
  NVIC_ST_CURRENT_R = 0;
}

uint32 SysTick_PeriodGet (void)
{
  return NVIC_ST_RELOAD_R + 1;
}

uint32 SysTick_ValueGet (void)
{
  return NVIC_ST_CURRENT_R;
}

bool SysTick_Is_Time_Out (void)
{
  if(GET_BIT(NVIC_ST_CTRL_R,16))
  {
    NVIC_ST_CURRENT_R = 0;
    return true;
  }
  return false;
}

void SysTick_Handler (void)
{
  if (SysTick_Callback != 0)
    SysTick_Callback();
}
