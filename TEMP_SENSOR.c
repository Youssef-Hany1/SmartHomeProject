#include "STD_TYPES.h"
#include "BIT_WISE_OPS.h"
#include "DIO.h"
#include "tm4c123gh6pm.h"
#include "TEMP_SENSOR.h"

static void (*TEMP_Callback) (Vfloat32) = 0;

// Function to initialize ADC for temperature sensor
void ADC_Init(void (*callback)(Vfloat32)) {
    // Enable the clock for ADC0 module
    SYSCTL_RCGCADC_R |= 0x01;
    
    // Enable the clock for GPIO Port E (assuming temperature sensor is connected to PE3)
    SYSCTL_RCGCGPIO_R |= 0x10;
    while ((SYSCTL_PRGPIO_R & 0x10) == 0) {}

    // Configure PE3 as an analog input
    SET_BIT(GPIO_PORTE_AFSEL_R,Pin3);
    CLR_BIT(GPIO_PORTE_DEN_R,Pin3);
    SET_BIT(GPIO_PORTE_AMSEL_R,Pin3);

    // Disable sample sequencer 3 during configuration
    CLR_BIT(ADC0_ACTSS_R,Pin3);

    // Configure ADC0 sequencer 3 for a single sample, triggered by the processor
    ADC0_EMUX_R = (ADC0_EMUX_R & ~0xF000) | 0x0000; // Processor trigger
    ADC0_SSMUX3_R = 0;                             // Select AIN0 (PE3)
    ADC0_SSCTL3_R = 0x06;                          // Single-ended, end of sequence

    // Enable interrupt for sequencer 3
    ADC0_IM_R |= 0x08;

    // Enable sequencer 3
    ADC0_ACTSS_R |= 0x08;

    // Enable ADC0 interrupt in NVIC
    NVIC_EN0_R |= 1 << 17; // Interrupt number 17 for ADC0
    
    TEMP_Callback= callback;
}

// ADC0 Sequencer 3 interrupt handler
void ADC0Seq3_Handler(void) {
  if(TEMP_Callback!= 0){
    uint32 adcValue;
    Vfloat32 voltage, temperature;

    // Clear the interrupt flag
    ADC0_ISC_R = 0x08;

    // Read the ADC value from the FIFO
    adcValue = ADC0_SSFIFO3_R;

    // Convert the ADC value to a temperature
    voltage = (adcValue * 3.3) / 4096; // Calculate the input voltage
    temperature = voltage * 100;      // Convert voltage to temperature (10 mV per degree)

    // Check if the temperature exceeds the threshold
    TEMP_Callback(temperature);
  }
}

//ADCProcessorTrigger(ADC0_BASE, 3);

/*
// User-defined callback function
void Temperature_Callback(Vfloat32 temperature) {
    if (temperature > TEMPERATURE_THRESHOLD) {
        GPIO_PORTF_DATA_R |= 0x02; // Turn on the Red LED
    } else {
        GPIO_PORTF_DATA_R &= ~0x02; // Turn off the Red LED
    }
}
*/