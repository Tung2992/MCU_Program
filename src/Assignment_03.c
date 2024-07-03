#include "device_registers.h"
#include "system_S32K144.h"
#include "stdio.h"
#include "stdint.h"
#include "ARM_GPIO.h"
#include "ARM_SYSTICK.h"
#include "My_Device.h"


typedef struct {
    uint32_t period; /* ms */
    uint32_t lastTick;
    uint8_t state;
} SoftTimer_t;

static volatile uint32_t count = 0;

SoftTimer_t timer_Led_Red = { 1000, 0, 0 }; //  - LED_RED
SoftTimer_t timer_Led_Blue = { 5000, 0, 0 }; // - LED_GREEN


void SoftTimer_Update(SoftTimer_t *timer) {
	if ((count - timer->lastTick) >= timer->period) {
		timer->lastTick = count;
		timer->state = !timer->state;
		if (timer == &timer_Led_Red) {
			if (timer->state) {
				ARM_GPIO_SetOutput(LED_RED_DEVICE, LED_ON);
			}
			else {
				ARM_GPIO_SetOutput(LED_RED_DEVICE, LED_OFF);
			}
		}
		else if (timer == &timer_Led_Blue) {
			if (timer->state) {
				ARM_GPIO_SetOutput(LED_BLUE_DEVICE, LED_ON);
			}
			else {
				ARM_GPIO_SetOutput(LED_BLUE_DEVICE, LED_OFF);
			}
		}
	}
}
void Init_Devices() {
	ARM_GPIO_Setup(LED_RED_DEVICE, NULL);
	ARM_GPIO_SetDirection(LED_RED_DEVICE, ARM_GPIO_OUTPUT);
	ARM_GPIO_SetOutput(LED_RED_DEVICE, LED_OFF);

	ARM_GPIO_Setup(LED_BLUE_DEVICE, NULL);
	ARM_GPIO_SetDirection(LED_BLUE_DEVICE, ARM_GPIO_OUTPUT);
	ARM_GPIO_SetOutput(LED_BLUE_DEVICE, LED_OFF);

}

/* time is millisecond */
uint32_t Get_Ticks(uint32_t time) {
	SystemCoreClockUpdate();
	return time * SystemCoreClock / 1000;
}

void SysTick_Handler(void) {
	count++;
}


int main() {
	Init_Devices();
	if (SysTick_Config(Get_Ticks(1))) {
		return 1; //Error
	}
	//Supper Loop
	while(true) {
		SoftTimer_Update(&timer_Led_Red);
		SoftTimer_Update(&timer_Led_Blue);
	}
	return 0; //Success
}
