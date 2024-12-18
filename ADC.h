#ifndef ADC_H
#define ADC_H

#include "STD_TYPES.h"

#define TEMPERATURE_THRESHOLD 40 // Temperature threshold in Celsius

void ADC1_Init(void);
uint32 ADC1_ReadValue(void);

#endif