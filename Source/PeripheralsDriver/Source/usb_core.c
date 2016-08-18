#include "usb_regs.h"
#include "usb_init.h"
#include "usb_mem.h"
#include "usb_desc.h"

#include "led.h"

/*得到当前事务类型*/
USB_StatusTypeDef
USB_GetCurrentTransaction(USB_CurrentTransTypeDef* trans)
{
	uint16_t isr, epValue;

	/*获取当前状态*/
	isr = _GetISTR();

	/*获取端点索引*/
	trans->ep = isr & ISTR_EP_ID;	

	/*如果是控制端点*/
	if(trans->ep == 0)
	{
		trans->rxState = _GetENDPOINT(ENDP0);
		trans->txState = trans->rxState & EPTX_STAT;
		trans->rxState &= EPRX_STAT;

		_SetEPRxTxStatus(ENDP0, EP_RX_NAK, EP_TX_NAK);

		/*判断是否为 IN 事务*/
		if((isr & ISTR_DIR) == 0)
		{
			/*清除发送标志*/
			_ClearEP_CTR_TX(ENDP0);

			trans->transaction = USB_Transaction_IN;

			return USB_Success;
		}
		else 
		{
			/*获取端点状态寄存器内容*/
			epValue = _GetENDPOINT(ENDP0);

			/*判断是否为 SETUP 事务，否则为 OUT 事务*/
			if((epValue & EP_SETUP) != 0)
			{
				/*清除接收标志*/
				_ClearEP_CTR_RX(ENDP0);

				trans->transaction = USB_Transaction_SETUP;

				return USB_Success;
			}
			else if((epValue & EP_CTR_RX) != 0)
			{	
				/*清除接收标志*/
				_ClearEP_CTR_RX(ENDP0);

				trans->transaction = USB_Transaction_OUT;	

				return USB_Success;
			}
			else
			{
				return USB_Error;
			}
		}
	}
	else /*如果是非控制端点*/
	{

		/*获取端点状态寄存器内容*/
		epValue  = _GetENDPOINT(trans->ep);

		/*判断是否为 OUT 事务，否则为 IN 事务*/
		if((epValue & EP_CTR_RX) != 0)
		{
			/*清除接收标志*/
			_ClearEP_CTR_RX(ENDP0);

			trans->transaction = USB_Transaction_OUT;

			return USB_Success;
		}
		else if((epValue & EP_CTR_TX) != 0)
		{
			/*清除发送标志*/
			_ClearEP_CTR_TX(ENDP0);

			trans->transaction = USB_Transaction_IN;

			return USB_Success;
		}
		else
		{
			return USB_Error;
		}
	}

	return USB_Success;
}

/*
 *0       1   2   3   4   5    6   7   8   9    10  11  12  13   14  15  16
 *+-------+---+---+---+---+----+---+---+---+----+---+---+---+----+---+---+
 *|ReqType|Req|  NULL | wValue |  NULL | wIndex |  NULL |wLength |  NULL |
 *+-------+---+---+---+---+----+---+---+---+----+---+---+---+----+---+---+
 */
void USB_GetCurrentInfo(USB_InformationTypeDef* info)
{
	uint8_t* pdata;

	pdata = PMAAddr + (uint8_t*)(_GetEPRxAddr(ENDP0) * 2);

	info->USBbmRequestType = *pdata;
	info->USBbRequest      = *(pdata + 1);
	info->USBwValue        = (((uint16_t)*(pdata + 5) << 8)   | *(pdata + 4));
	info->USBwIndex        = (((uint16_t)*(pdata + 9) << 8)   | *(pdata + 8));
	info->USBwLength       = (((uint16_t)*(pdata + 13) << 8)  | *(pdata + 12));
}

void USB_NoDataSetup0(USB_CurrentTransTypeDef* tran)
{
	/*设置设备地址*/
	if(usbDevice.pInformation->USBbRequest == SET_ADDRESS)
	{
		/*分配地址状态*/
		usbDevice.state = ADDRESS;

		/*返回0字节数据*/
		SetEPTxCount(ENDP0, 0);
		tran->txState = EP_TX_VALID;
	}

	if(usbDevice.pInformation->USBbRequest == SET_CONFIGURATION)
	{
		/*分配地址状态*/
		usbDevice.state = CONFIGURE;

		/*返回0字节数据*/
		SetEPTxCount(ENDP0, 0);
		tran->txState = EP_TX_VALID;
	}
}

void USB_GetDescriptor(USB_CurrentTransTypeDef* tran)
{
	USB_DescriptorTypeDef type
		= ((usbDevice.pInformation->USBwValue & 0xFF00) >> 8);

	switch(type)
	{
		case USB_DeviceDescriptor:

			/* 复制设备描述符至发送缓冲区*/
			UserToPMABufferCopy(&Virtual_Com_Port_DeviceDescriptor[0],
					GetEPTxAddr(ENDP0), 18);

			/*设置发送计数器为设备描述符长度*/
			SetEPTxCount(ENDP0, 18);

			/*发送使能*/
			tran->txState = EP_TX_VALID;
			tran->rxState = EP_RX_STALL;

			/*准备进入 Status  Out 状态 */
			usbDevice.controlState = USB_ControlState_LastDataIn;

			break;

		case USB_ConfigDescriptor:
			/*短的配置描述符*/
			if(usbDevice.pInformation->USBwLength == 9)
			{
				UserToPMABufferCopy(&Virtual_Com_Port_ConfigDescriptor[0],
						GetEPTxAddr(ENDP0), 9);

				SetEPTxCount(ENDP0, 9);
			}
			/*长的配置描述符， 需要分两次发送*/
			else if(usbDevice.pInformation->USBwLength == 67)
			{
				UserToPMABufferCopy(&Virtual_Com_Port_ConfigDescriptor[0],
						GetEPTxAddr(ENDP0), VIRTUAL_COM_PORT_DATA_SIZE);

				SetEPTxCount(ENDP0, VIRTUAL_COM_PORT_DATA_SIZE);
			}

			tran->txState = EP_TX_VALID;
			tran->rxState = EP_RX_STALL;

			usbDevice.controlState = USB_ControlState_DataIn;

			break;

		case USB_EndpointDescriptor:
			break;

		case USB_StringDescriptor:
			/**/
			if(((usbDevice.pInformation->USBwValue) & 0xFF) == 0x00)       /*LangID*/
			{
				UserToPMABufferCopy(&Virtual_Com_Port_StringLangID[0],
						GetEPTxAddr(ENDP0), VIRTUAL_COM_PORT_SIZ_STRING_LANGID);

				SetEPTxCount(ENDP0, VIRTUAL_COM_PORT_SIZ_STRING_LANGID);
			}
			else if(((usbDevice.pInformation->USBwValue) & 0xFF) == 0x01) /*Vendor*/
			{
				UserToPMABufferCopy(&Virtual_Com_Port_StringVendor[0],
						GetEPTxAddr(ENDP0), VIRTUAL_COM_PORT_SIZ_STRING_VENDOR);

				SetEPTxCount(ENDP0, VIRTUAL_COM_PORT_SIZ_STRING_VENDOR);
			}
			else if(((usbDevice.pInformation->USBwValue) & 0xFF) == 0x02) /*Product*/
			{
				UserToPMABufferCopy(&Virtual_Com_Port_StringProduct[0],
						GetEPTxAddr(ENDP0), VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT);

				SetEPTxCount(ENDP0, VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT);
			}
			else if(((usbDevice.pInformation->USBwValue) & 0xFF) == 0x03) /*Serial*/
			{
				UserToPMABufferCopy(&Virtual_Com_Port_StringSerial[0],
						GetEPTxAddr(ENDP0), VIRTUAL_COM_PORT_SIZ_STRING_SERIAL);

				SetEPTxCount(ENDP0, VIRTUAL_COM_PORT_SIZ_STRING_SERIAL);
			}

			tran->txState = EP_TX_VALID;
			tran->rxState = EP_RX_STALL;

			usbDevice.controlState = USB_ControlState_LastDataIn;

			break;

		/*不支持的描述符*/
		default:

			tran->rxState = EP_RX_VALID;
			tran->txState = EP_TX_STALL;

			break;
	}
}

void USB_CDC_SetLineCode(USB_CurrentTransTypeDef* tran)
{
	tran->txState = EP_TX_STALL;
	tran->rxState = EP_RX_VALID;

	SetEPRxCount(ENDP0, 64);

	usbDevice.controlState = USB_ControlState_LastDataOut;
	usbDevice.state = CONFIGURED;
}

void USB_DataSetup0(USB_CurrentTransTypeDef* tran)
{
	/* 传输方向： Device ---> Host */
	if((usbDevice.pInformation->USBbmRequestType & 0x80) != 0)
	{
		/*获取设备描述符*/
		if(usbDevice.pInformation->USBbRequest == GET_DESCRIPTOR)
		{
			USB_GetDescriptor(tran);
		}
	}
	/* Set Line Code*/
	else if((usbDevice.pInformation->USBbmRequestType & 0x21) != 0)
	{

		if(usbDevice.pInformation->USBbRequest  == 0x20)
		{
			USB_CDC_SetLineCode(tran);
		}
	}
}

void USB_CTR(USB_CurrentTransTypeDef* tran)
{

		if(tran->ep == 0) /*控制端点*/
		{
			switch(tran->transaction)
			{
				case USB_Transaction_SETUP:

					usbDevice.controlState = USB_ControlState_Setup;

					USB_GetCurrentInfo(usbDevice.pInformation);

					/*控制传输非数据阶段*/
					if(usbDevice.pInformation->USBwLength == 0)
					{
						USB_NoDataSetup0(tran);
					}
					else /*控制传输数据阶段*/
					{
						USB_DataSetup0(tran);
					}

					_SetEPRxTxStatus(ENDP0, tran->rxState, tran->txState);

					break;

				case USB_Transaction_OUT:
					if(usbDevice.controlState == USB_ControlState_LastDataOut)
					{
						tran->rxState = EP_RX_STALL;
						tran->txState = EP_TX_VALID;

						SetEPTxCount(ENDP0, 0);

						usbDevice.controlState = USB_ControlState_StatusIn;
					}

					_SetEPRxTxStatus(ENDP0, tran->rxState, tran->txState);

					break;

				case USB_Transaction_IN:

					/* No Data Stage */
					if(usbDevice.state == ADDRESS)
					{
						USB_GetCurrentInfo(usbDevice.pInformation);

						/*设置设备地址*/
						SetDADDR(usbDevice.pInformation->USBwValue | DADDR_EF);

						SetEPRxCount(ENDP0, 64);

						usbDevice.state = CONFIGURE;

						_SetEPRxTxStatus(ENDP0, EP_RX_VALID, EP_TX_STALL);

						return;
					}

					/* Data Stage */
					if(usbDevice.controlState == USB_ControlState_LastDataIn)
					{
						usbDevice.controlState = USB_ControlState_StatusOut;

						_SetEPRxTxStatus(ENDP0, EP_RX_VALID, EP_TX_STALL);
					}
					else if(usbDevice.controlState == USB_ControlState_DataIn)
					{
						UserToPMABufferCopy(
								&Virtual_Com_Port_ConfigDescriptor[VIRTUAL_COM_PORT_DATA_SIZE],
								GetEPTxAddr(ENDP0),
							    VIRTUAL_COM_PORT_SIZ_CONFIG_DESC - VIRTUAL_COM_PORT_DATA_SIZE);

						SetEPTxCount(ENDP0,
								VIRTUAL_COM_PORT_SIZ_CONFIG_DESC - VIRTUAL_COM_PORT_DATA_SIZE);

						usbDevice.controlState = USB_ControlState_LastDataIn;

						_SetEPRxTxStatus(ENDP0, EP_RX_VALID, EP_TX_VALID);
					}
					else if(usbDevice.controlState == USB_ControlState_StatusIn)
					{
						_SetEPRxTxStatus(ENDP0, EP_RX_VALID, EP_TX_STALL);
					}

					break;

				default:
					break;
			}
		}
		else /*非控制端点*/
		{
			if(tran->ep ==1)
			{
				if(tran->transaction == USB_Transaction_IN)
				{
					/*UserToPMABufferCopy(&Virtual_Com_Port_StringProduct[0],
							GetEPTxAddr(ENDP1), VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT);

					SetEPTxCount(ENDP1, 1);*/
				    SetEPTxValid(ENDP1);
				}
			}
		}
	}
}
