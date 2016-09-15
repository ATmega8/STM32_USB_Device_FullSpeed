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

void USB_Task(void* parameters);

