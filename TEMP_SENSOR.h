#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#include "STD_TYPES.h"

#define TEMPERATURE_THRESHOLD 40 // Temperature threshold in Celsius

void ADC_Init(void (*callback)(Vfloat32));

#endif