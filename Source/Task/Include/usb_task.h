#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include "main.h"

SemaphoreHandle_t USB_CTR_Semaphore;

/*USB发送缓冲区*/
uint8_t usbTxBuffer[64];

/*USB发送长度队列*/
QueueHandle_t usbTxBufferCountQueue;

/*USB发送缓冲区锁*/
SemaphoreHandle_t USB_TxBuffer_Mutex;

void USB_Task(void* parameters);
void USB_SendData(const uint8_t* pdata, uint32_t len);

