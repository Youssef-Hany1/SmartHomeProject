#include "DOOR.h"
#include "BIT_WISE_OPS.h"
#include "tm4c123gh6pm.h"

static void (*DOOR_Callback) (void) = 0;

void DOOR_Init(void (*callback)(void))
{
  dio_init(door_port, door_pin, Input);  // Initialize the door pin as input
  
  // Configure PF4 for edge-sensitive interrupt on both edges
  CLR_BIT(GPIO_PORTF_IS_R, 4);   // Edge-sensitive
  SET_BIT(GPIO_PORTF_IBE_R, 4);  // Interrupt on both edges
  SET_BIT(GPIO_PORTF_ICR_R, 4);  // Clear any prior interrupt
  SET_BIT(GPIO_PORTF_IM_R, 4);   // Unmask interrupt for PF4
  
  // Enable interrupt for Port F in NVIC
  NVIC_EN0_R |= (1 << 30);    // Enable interrupt #30 (Port F)
  
  // Set the callback function
  DOOR_Callback = callback;
}

void GPIOF_Handler(void)
{
  if (DOOR_Callback != 0)
  {
    // Check if PF4 caused the interrupt
    if (GPIO_PORTF_MIS_R & 0x10)
    {
      // Clear the interrupt flag for PF4
      GPIO_PORTF_ICR_R |= 0x10;
      
      // Call the provided callback function
      DOOR_Callback();
    }
  }
}

/*
// Callback function for Door interrupt
void DOOR_Callback(void) {
  if (!(GPIO_PORTF_DATA_R & 0x10)) {
      LED_On('F', Pin1);
  } else {
      LED_Off('F', Pin1);
  }
}
*/
