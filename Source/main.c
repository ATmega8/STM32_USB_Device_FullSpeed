#include "main.h"

#include "led.h"
#include "usb_task.h"
#include "usb_init.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

const uint8_t str[12] = "He said Hi!\n";

void IDLE_Task(void* parameters)
{
	uint32_t i;
	uint32_t len = 12;

	while(1)
	{
		vTaskDelay(100);

		if(usbDevice.state == CONFIGURED)
		{
			for(i = 0; i < 12; i++)
			{
				usbTxBuffer[i] = str[i];
			}

			xQueueSend(usbTxBufferCountQueue, &len, 0);
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
	xTaskCreate(IDLE_Task, "Idle Task", 1024, NULL, 1, NULL);
	vTaskStartScheduler();

	while(1)
	{

	}
}
  
