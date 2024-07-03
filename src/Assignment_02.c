#include "device_registers.h"
#include "stdio.h"
#include "stdint.h"
#include "ARM_GPIO.h"
#include "My_Device.h"

void Init_Devices() {
	ARM_GPIO_Setup(LED_RED_DEVICE, NULL);
	ARM_GPIO_SetDirection(LED_RED_DEVICE, ARM_GPIO_OUTPUT);
	ARM_GPIO_SetOutput(LED_RED_DEVICE, LED_OFF); //Turn off Led

	ARM_GPIO_Setup(BUTTON_SW2_DEVICE, NULL);
	ARM_GPIO_SetDirection(BUTTON_SW2_DEVICE, ARM_GPIO_INPUT);
}


int main() {
	Init_Devices();
	while(true) {
		if(ARM_GPIO_GetInput(BUTTON_SW2_DEVICE) == BUTTON_OFF) {
			ARM_GPIO_SetOutput(LED_RED_DEVICE, LED_OFF);
		}
		else {
			ARM_GPIO_SetOutput(LED_RED_DEVICE, LED_ON);
		}
	}
	return 1;
}
