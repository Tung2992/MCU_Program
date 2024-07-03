#include "device_registers.h"
#include "stdio.h"
#include "stdint.h"
#include "ARM_GPIO.h"
#include "My_Device.h"

extern ARM_DRIVER_GPIO Driver_GPIO;
static ARM_DRIVER_GPIO * GPIOdrv = &Driver_GPIO;

uint8_t status_Led_Red = LED_OFF;
uint8_t status_Led_Green = LED_OFF;

void My_Button_Handler(uint8_t button_pin) {
	if (button_pin == BUTTON_SW2_PIN) {
		status_Led_Red = !status_Led_Red;
		if (status_Led_Red) {
			ARM_GPIO_SetOutput(LED_RED_DEVICE, LED_ON);
		}
		else {
			ARM_GPIO_SetOutput(LED_RED_DEVICE, LED_OFF);
		}
	}
	else if (button_pin == BUTTON_SW3_PIN) {
		status_Led_Green = !status_Led_Green;
		if (status_Led_Green) {
			ARM_GPIO_SetOutput(LED_GREEN_DEVICE, LED_ON);
		}
		else {
			ARM_GPIO_SetOutput(LED_GREEN_DEVICE, LED_OFF);
		}
	}
}

void GPIO_Event_Handler(ARM_GPIO_Pin_t pin, uint32_t event) {
	switch (event) {
	case ARM_GPIO_EVENT_RISING_EDGE:
		My_Button_Handler(pin);
		break;
	case ARM_GPIO_EVENT_FALLING_EDGE:
		break;
	case ARM_GPIO_EVENT_EITHER_EDGE:
		break;
	}
}

void Init_Devices() {

	/* LED_RED */
	GPIOdrv->Setup			(LED_RED_DEVICE, NULL);
	GPIOdrv->SetDirection	(LED_RED_DEVICE, ARM_GPIO_OUTPUT);
	GPIOdrv->SetOutput		(LED_RED_DEVICE, LED_OFF);

	/* LED_GREEN */
	GPIOdrv->Setup			(LED_GREEN_DEVICE, NULL);
	GPIOdrv->SetDirection	(LED_GREEN_DEVICE, ARM_GPIO_OUTPUT);
	GPIOdrv->SetOutput		(LED_GREEN_DEVICE, LED_OFF);

	/* BUTTON_SW2 */
	GPIOdrv->Setup			(BUTTON_SW2_DEVICE, GPIO_Event_Handler);
	GPIOdrv->SetDirection	(BUTTON_SW2_DEVICE, ARM_GPIO_INPUT);
	GPIOdrv->SetEventTrigger(BUTTON_SW2_DEVICE, ARM_GPIO_TRIGGER_RISING_EDGE);

	/* BUTTON_SW3 */
	GPIOdrv->Setup			(BUTTON_SW3_DEVICE, GPIO_Event_Handler);
	GPIOdrv->SetDirection	(BUTTON_SW3_DEVICE, ARM_GPIO_INPUT);
	GPIOdrv->SetEventTrigger(BUTTON_SW3_DEVICE, ARM_GPIO_TRIGGER_RISING_EDGE);

}

int main() {
	Init_Devices();
	while(true);
	return 0; // success
}
