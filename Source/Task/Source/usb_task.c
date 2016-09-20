#include "usb_task.h"
#include "usb_init.h"
#include "led.h"

void USB_Task(void* parameters)
{
	uint32_t txCount, rxCount;
	/*初始化信号量*/
	USB_CTR_Semaphore = xSemaphoreCreateBinary();

	/*初始化队列*/
	usbTxBufferCountQueue = xQueueCreate(1, sizeof(uint32_t));

	/*初始化Mutex*/
	USB_TxBuffer_Mutex = xSemaphoreCreateMutex();

	/*信号量或队列创建失败*/
	if(USB_CTR_Semaphore == NULL /*&& (usbTxBufferCountQueue == NULL)*/)
	{

	}
	else
	{
		while(1)
		{
			if(xSemaphoreTake(USB_CTR_Semaphore, portMAX_DELAY) == pdTRUE)
			{
				USB_CTR(&tran);

				/*等待USB枚举成功*/
				if(usbDevice.state == CONFIGURED)
				{
					/*处理发送与接收缓冲区*/
					/*获取发送缓冲区长度*/
					if(xQueueReceive(usbTxBufferCountQueue, &txCount, (TickType_t)0))
					{
						tran.txCount = txCount;
					}
					else /*Timeout*/
					{

					}
				}
			}
			else /*Timeout*/
			{

			}
		}
	} /* if(USB_CTR_Semaphore == NULL)*/

}

void USB_SendData(const uint8_t* pdata, uint32_t len)
{
	uint32_t i;

	/*上锁*/
	if(xSemaphoreTake(USB_TxBuffer_Mutex, portMAX_DELAY) == pdTRUE)
	{
		/*写入数据长度*/
		if(xQueueSend(usbTxBufferCountQueue, &len, portMAX_DELAY) == pdPASS)
		{
			/*复制数据*/
			for(i = 0; i < len; i++)
			{
				usbTxBuffer[i] = *(pdata + i);
			}
		}

		/*开锁*/
		xSemaphoreGive(USB_TxBuffer_Mutex);
	}
}

