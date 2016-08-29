#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "main.h"

SemaphoreHandle_t USB_CTR_Semaphore;

void USB_Task(void* parameters);

