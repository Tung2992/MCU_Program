#include "My_Systick.h"

//My_Systik_Handler Myfunc = Null;

void My_Systick_Enable() {
	S32_SysTick->CSR |= S32_SysTick_CSR_CLKSOURCE(1);	//Select Core Clock
	S32_SysTick->CSR |= S32_SysTick_CSR_TICKINT(1);		//Enable Interrupt
	S32_SysTick->CSR |= S32_SysTick_CSR_ENABLE(1);		//Enable Systick
}

void My_Systick_SetTime(uint32_t time) {
//	SystemCoreClockUpdate();
	S32_SysTick->RVR = time * SystemCoreClock/1000 - 1;
}

void My_Systick_Disable() {
	S32_SysTick->CSR &= ~S32_SysTick_CSR_TICKINT_MASK;		//Disable Interrupt
	S32_SysTick->CSR &= ~S32_SysTick_CSR_ENABLE_MASK;
}



