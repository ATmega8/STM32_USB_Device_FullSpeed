#include "main.h"

#include "led.h"
#include "usb_task.h"
#include "usb_init.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

const uint8_t str[12] = "He said Hi!\n";
const uint8_t str1[12] = "12 3456 789\n";

void IDLE_Task(void* parameters)
{
	while(1)
	{
		vTaskDelay(1000);

		if(usbDevice.state == CONFIGURED)
		{
			USB_SendData(&str[0], 12);
		}
	}
}

void IDLE1_Task(void* parameters)
{
	while(1)
	{
		vTaskDelay(1000);

		if(usbDevice.state == CONFIGURED)
		{
			USB_SendData(&str1[0], 12);
		}
	}
}

int main(void)
{
	/*无响应优先级*/
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

	LED_Init();
	USB_InterfaceInit();

	/*初始化环形缓冲区*/
	//usbTxCbuf = CircularBuffer_Create(128, sizeof(uint32_t));

	xTaskCreate(USB_Task, "USB Task", 1024, NULL, 3, NULL);
	xTaskCreate(IDLE_Task, "Idle Task", 130, NULL, 1, NULL);
	xTaskCreate(IDLE1_Task, "Idle Task", 130, NULL, 1, NULL);
	vTaskStartScheduler();

	while(1)
	{

	}
}
  
