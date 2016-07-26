#include <stdint.h>

/*USB 状态*/
typedef enum
{
	USB_Success,
	USB_Error
} USB_StatusTypeDef;

typedef struct
{
	uint8_t  USBbmRequestType;   /* bmRequestType */
	uint8_t  USBbRequest;        /* bRequest */
	uint16_t USBwValue;          /* wValue */
	uint16_t USBwIndex;          /* wIndex */
	uint16_t USBwLength;         /* wLength */
} USB_InformationTypeDef;

} USB_DeviceTypeDef;

/*USB事务类型*/
typedef enum
{
	USB_Transaction_IN,
	USB_Transaction_OUT,
	USB_Transaction_SETUP
} USB_TransactionTypeDef;

/*USB当前事务*/
typedef struct
{
	USB_TransactionTypeDef transaction;
	uint32_t ep;
} USB_CurrentTransTypeDef;

USB_StatusTypeDef
USB_GetCurrentTransaction(USB_CurrentTransTypeDef* trans);

