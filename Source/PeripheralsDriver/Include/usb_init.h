#include "usb_regs.h"
#include "usb_core.h"

uint32_t wInterrupt_Mask;

USB_DeviceTypeDef                    usbDevice;
USB_InformationTypeDef               usbDeviceInfo;
USB_CurrentTransTypeDef tran;

#define USB_ENABLE_GPIO_CLOCK        RCC_APB2Periph_GPIOA 
#define USB_ENABLE_GPIO_PORT         GPIOA
#define USB_ENABLE_GPIO_PIN          GPIO_Pin_3

#define BTABLE_ADDRESS               0x00
#define ENDP0_RXADDR                 0x40
#define ENDP0_TXADDR                 0x80
#define ENDP1_TXADDR                 0xC0
#define ENDP2_TXADDR                 0x100
#define ENDP3_RXADDR                 0x110

#define MAXPACKETSIZE                0x40
#define VIRTUAL_COM_PORT_DATA_SIZE   64


#define IMR_MSK (CNTR_CTRM  | /*正确传输中断使能*/ \
		         CNTR_SOFM  | /*起始帧中断使能*/ \
		         CNTR_RESETM ) /*USB 复位中断使能*/

void USB_InterfaceInit(void);
void USB_Reset(void);
void USB_Resume(void);
void USB_Enable(void);

