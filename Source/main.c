#include "main.h"

#include "led.h"
#include "usb_init.h"

#include "FreeRTOS.h"
#include "task.h"

void Task1(void* parameters)
{
	while(1)
	{
		vTaskDelay(50);
	}
}

void Task2(void* parameters)
{
	while(1)
	{
		LED_SetLED();
		vTaskDelay(50);
		LED_ResetLED();
		vTaskDelay(50);
	}
}

int main(void)
{
	/*无响应优先级*/
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

	LED_Init();
	USB_InterfaceInit();


	while(1)
	{

	}
}
  
