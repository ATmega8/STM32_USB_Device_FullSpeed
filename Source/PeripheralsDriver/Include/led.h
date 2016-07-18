#define LED_GPIO_CLOCK     RCC_APB2Periph_GPIOA 
#define LED_GPIO_PORT      GPIOA
#define LED_GPIO_PIN       GPIO_Pin_6

void LED_Init(void);
void LED_SetLED(void);
void LED_ResetLED(void);

