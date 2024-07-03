#include <stdint.h>
#include <stdio.h>
#include "device_registers.h"
#include "My_Device.h"
#include "ARM_GPIO.h"
#include "ARM_LPUART.h"


void Init() {
	ARM_GPIO_Setup(LED_RED_DEVICE, NULL);
	ARM_GPIO_SetDirection(LED_RED_DEVICE, ARM_GPIO_OUTPUT);
	ARM_GPIO_SetOutput(LED_RED_DEVICE, LED_ON); //Turn off Led

	uart_config_t config = {
			.baudRate = 9600,
			.bitCountPerChar = UART_8_BITS_PER_CHAR,
			.parityMode = UART_PARITY_DISABLED,
			.stopBitCount = UART_ONE_STOP_BIT,
	};
	UART_Init(LPUART1, &config);
}
int main(void)
{
	Init();
	char data;
	uint8_t string[15] = "Hello World";
	ARM_GPIO_SetOutput(LED_RED_DEVICE, LED_OFF);
	data = UART_ReceiveCharBlocking(LPUART1);
	if (data == 'A') {
		ARM_GPIO_SetOutput(LED_RED_DEVICE, LED_ON);
		UART_SendStringBlocking(LPUART1, string, 10);
		while(true);
	}
	while(true);
	return 0;
}
