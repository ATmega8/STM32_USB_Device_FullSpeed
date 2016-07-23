#include <stdint.h>

/*USB设备类型*/
typedef struct
{

} USB_DeviceTypeDef;

typedef enum
{
	USB_Transaction_IN,
	USB_Transaction_OUT,
	USB_Transaction_SETUP
} USB_TransactionTypeDef;

typedef struct
{
	USB_TransactionTypeDef transaction;
	uint32_t ep;
} USB_CurrentTransTypeDef;

