#include "usb_regs.h"
#include "usb_core.h"

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

