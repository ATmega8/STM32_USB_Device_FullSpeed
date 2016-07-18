//hardware drivers

#include "USBCDC.h"

//On STM32 Minikit
#define LED_FAULT_1   GPIO_SetBits(GPIOB, GPIO_Pin_8)
#define LED_FAULT_0   GPIO_ResetBits(GPIOB, GPIO_Pin_8)
