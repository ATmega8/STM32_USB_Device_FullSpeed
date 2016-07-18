#include "stm32f10x_conf.h"
#include "led.h"

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(LED_GPIO_CLOCK, ENABLE);

	/* Configure PD0 and PD2 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = LED_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);

}

void LED_SetLED(void)
{
	GPIO_SetBits(LED_GPIO_PORT, LED_GPIO_PIN);
}

void LED_ResetLED(void)
{
	GPIO_ResetBits(LED_GPIO_PORT, LED_GPIO_PIN);
}

