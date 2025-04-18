#include "ADC.h"
#include "..\tm4c123gh6pm.h"

void ADC1_Init(void)
{
    /* Using PD1 -> AIN6 */

    // 1. Enable the ADC clock using the RCGCADC register
    SYSCTL_RCGCADC_R |= (1 << 1);
    volatile int delay = 0; // Allow stabilization time
    for (volatile int i = 0; i < 1000; i++); // Proper delay for clock stabilization

    // 2. Enable the clock to the GPIO module for PORTD
    SYSCTL_RCGCGPIO_R |= (1 << 3);

    // 3. Configure PD1 as an analog input
    GPIO_PORTD_AFSEL_R |= (1 << 1);    // Enable alternate function for PD1
    GPIO_PORTD_DEN_R &= ~(1 << 1);     // Disable digital function for PD1
    GPIO_PORTD_AMSEL_R |= (1 << 1);    // Enable analog function for PD1

    // 4. Disable sample sequencer 3 during configuration
    ADC1_ACTSS_R &= ~(1 << 3);

    // 5. Configure the trigger event for sample sequencer 3
    ADC1_EMUX_R = (0xF << 12); // Always trigger (change if required)

    // 6. Configure input source for sample sequencer 3
    ADC1_SSMUX3_R = 6; // AIN6

    // 7. Configure sample control: end of sequence and interrupt enable
    ADC1_SSCTL3_R = (1 << 1) | (1 << 2); // Enable END and IE bits

    // ** New Code: Set ADC sampling time for stability **
    ADC1_SAC_R = 0x06; // Hardware oversampling set to 64x for improved stability

    // 8. Disable interrupts for now (unless required)
    ADC1_IM_R &= ~(1 << 3);

    // 9. Enable sample sequencer 3
    ADC1_ACTSS_R |= (1 << 3);
}


uint32 ADC1_ReadValue(void)
{
    ADC1_PSSI_R |= (1 << 3);           // Start SS3 Conversion
    while ((ADC1_RIS_R & (1 << 3)) == 0); // Wait for conversion to complete
    uint32 ADC_Value = ADC1_SSFIFO3_R; // Read the ADC value
    ADC1_ISC_R = (1 << 3);             // Clear the interrupt flag
    return ADC_Value;                  // Return the result
}

// Function to convert ADC value to temperature
float ADC_To_Temperature(uint32 adcValue) {
    // Convert raw ADC value to voltage (assuming 3.3V reference)
    float voltage = (adcValue * 3.3) / 4095;

    // Convert voltage to temperature (10mV/�C -> 0.01V/�C)
    float temperature = voltage / 0.01;

    return temperature; // Return the temperature in Celsius
}
