#include "usb_task.h"
#include "usb_init.h"
#include "led.h"

void USB_Task(void* parameters)
{
	/*初始化信号量*/
	USB_CTR_Semaphore = xSemaphoreCreateBinary();


	/*信号量创建失败*/
	if(USB_CTR_Semaphore == NULL)
	{

	}
	else
	{
		while(1)
		{
			if(xSemaphoreTake(USB_CTR_Semaphore, (TickType_t)1000) == pdTRUE)
			{

			}
			else /*Timeout*/
			{

			}

			/*等待USB枚举成功*/
			if(usbDevice.state == CONFIGURED)
			{
				/*处理发送与接收缓冲区*/
				LED_SetLED();

			}
		}
	} /* if(USB_CTR_Semaphore == NULL)*/

}

