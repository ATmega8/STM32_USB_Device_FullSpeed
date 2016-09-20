#include <stdint.h>

/*USB 状态*/
typedef enum
{
	USB_Success,
	USB_Error
} USB_StatusTypeDef;

/*USB 描述符*/
typedef enum 
{
	USB_DeviceDescriptor = 1,
	USB_ConfigDescriptor,
	USB_StringDescriptor,
	USB_InterfaceDescriptor,
	USB_EndpointDescriptor
} USB_DescriptorTypeDef;

/*USB 控制传输状态*/
typedef enum
{
	USB_ControlState_Setup,
	USB_ControlState_DataIn,
	USB_ControlState_LastDataIn,
	USB_ControlState_LastDataOut,
	USB_ControlState_StatusOut,
	USB_ControlState_StatusIn
}  USB_ControlStateTypeDef;

/* USB 标准请求*/
typedef enum
{
	GET_STATUS = 0,
	CLEAR_FEATURE,
	RESERVED1,
	SET_FEATURE,
	RESERVED2,
	SET_ADDRESS,
	GET_DESCRIPTOR,
	SET_DESCRIPTOR,
	GET_CONFIGURATION,
	SET_CONFIGURATION,
	GET_INTERFACE,
	SET_INTERFACE,
	TOTAL_sREQUEST,  /* Total number of Standard request */
	SYNCH_FRAME = 12
} USB_StandardRequests;

typedef struct
{
	uint8_t  USBbmRequestType;   /* bmRequestType */
	uint8_t  USBbRequest;        /* bRequest */
	uint16_t USBwValue;          /* wValue */
	uint16_t USBwIndex;          /* wIndex */
	uint16_t USBwLength;         /* wLength */
} USB_InformationTypeDef;

typedef enum
{
	DEFAULT,
	ADDRESS,
	CONFIGURE,
	CONFIGURED
} USB_DeviceStateTypeDef;

/*USB设备类型*/
typedef struct
{
	uint8_t* pBuf;				 /*端点缓冲区*/
	USB_InformationTypeDef* pInformation; /*USB 信息*/
	USB_DeviceStateTypeDef state;
	USB_ControlStateTypeDef controlState;
	uint32_t sendLength;
	const uint8_t* buffer;
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
	uint16_t rxState;
	uint16_t txState;
	uint32_t txCount;
	uint32_t rxCount;
} USB_CurrentTransTypeDef;

USB_StatusTypeDef
USB_GetCurrentTransaction(USB_CurrentTransTypeDef* trans);

void USB_CTR(USB_CurrentTransTypeDef* tran);

uint32_t USB_CDC_SendData(const uint8_t* pdata, uint32_t len);

