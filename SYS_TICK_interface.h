/*
 * SYS_TICK_interface.h
 *
 *  Created on: Nov 28, 2024
 *      Author: Youssef Hany
 */

#ifndef SYS_TICK_INTERFACE_H_
#define SYS_TICK_INTERFACE_H_

#include "STD_TYPES.h"
#include <stdbool.h>

void SysTick_Init (uint32 reloadValue);
void SysTick_Enable (void);
void SysTick_Disable (void);
void SysTick_PeriodSet (uint32 ui32PeriodValue);
uint32 SysTick_PeriodGet (void);
uint32 SysTick_ValueGet (void);
bool SysTick_Is_Time_Out (void);

void SysTick_InitInterrupt (uint32 reloadValue, void (*callback)(void));

#endif