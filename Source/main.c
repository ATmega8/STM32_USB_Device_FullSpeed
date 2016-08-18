#include "main.h"

#include "led.h"
#include "usb_task.h"
#include "usb_init.h"

#include "FreeRTOS.h"
#include "task.h"

const char str[12] = "He said Hi!\n";

void IDLE_Task(void* parameters)
{
	while(1)
	{

	}
}

int main(void)
{
	/*无响应优先级*/
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

	LED_Init();
	USB_InterfaceInit();

	xTaskCreate(USB_Task, "USB Task", 1024, NULL, 3, NULL);
	xTaskCreate(IDLE_Task, "Idle Task", 1024, NULL, 1, NULL);
	vTaskStartScheduler();

	while(1)
	{

	}
}
  
