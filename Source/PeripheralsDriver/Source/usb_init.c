#include "usb_init.h"

void USB_InterfaceInit(void)
{
	uint16_t wRegVal;

	usbDevice.pInformation = &usbDeviceInfo;

	NVIC_InitTypeDef NVIC_InitStructure;

	/* USB 时钟初始化 */
	/* 选择 USB 时钟为 PLL 分频 1.5 倍 72 / 1.5 = 48 */
	RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
	/* 使能 USB 时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);

	/* USB 中断初始化 */
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/*** CNTR_PWDN = 0 ***/
	wRegVal = CNTR_FRES;
	_SetCNTR(wRegVal);

	/*** CNTR_FRES = 0 ***/
	wInterrupt_Mask = 0;
	_SetCNTR(wInterrupt_Mask);
	/*** Clear pending interrupts ***/
	_SetISTR(0);
	/*** Set interrupt mask ***/
	wInterrupt_Mask = CNTR_RESETM | CNTR_SUSPM | CNTR_WKUPM;
	_SetCNTR(wInterrupt_Mask);

	 _SetISTR(0);
	wInterrupt_Mask = IMR_MSK;   
	/* set interrupts mask */    
	_SetCNTR(wInterrupt_Mask);
}

void USB_Reset(void)
{
	/*设置USB缓冲区地址偏移量*/
	SetBTABLE(BTABLE_ADDRESS);

	/* 初始化端点0 */
	SetEPType(ENDP0, EP_CONTROL);

	SetEPTxStatus(ENDP0, EP_TX_STALL);

	SetEPRxAddr(ENDP0, ENDP0_RXADDR);
	SetEPTxAddr(ENDP0, ENDP0_TXADDR);

	Clear_Status_Out(ENDP0);

	SetEPRxCount(ENDP0, MAXPACKETSIZE);
	SetEPRxValid(ENDP0);

	/* 初始化端点1 */
	SetEPType(ENDP1, EP_BULK);
	SetEPTxAddr(ENDP1, ENDP1_TXADDR);
	SetEPTxStatus(ENDP1, EP_TX_VALID);
	SetEPRxStatus(ENDP1, EP_RX_DIS);

	/* 初始化端点 2 */
	SetEPType(ENDP2, EP_INTERRUPT);
	SetEPTxAddr(ENDP2, ENDP2_TXADDR);
	SetEPRxStatus(ENDP2, EP_RX_DIS);
	SetEPTxStatus(ENDP2, EP_TX_NAK);

	/* 初始化端点 3 */
	SetEPType(ENDP3, EP_BULK);
	SetEPRxAddr(ENDP3, ENDP3_RXADDR);
	SetEPRxCount(ENDP3, VIRTUAL_COM_PORT_DATA_SIZE);
	SetEPRxStatus(ENDP3, EP_RX_VALID);
	SetEPTxStatus(ENDP3, EP_TX_DIS);

	_SetEPAddress(0, 0);
	_SetEPAddress(1, 1);
	_SetEPAddress(2, 2);
	_SetEPAddress(3, 3);

	_SetDADDR(DADDR_EF | 0); 
}

