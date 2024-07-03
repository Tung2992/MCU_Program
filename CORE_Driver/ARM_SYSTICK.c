#include "ARM_SYSTICK.h"

uint32_t SysTick_Config(uint32_t ticks) {
    if ((ticks - 1UL) > S32_SysTick_RVR_RELOAD_MASK) {
        return 1; //Error
    }

    S32_SysTick->RVR  = (uint32_t)(ticks - 1UL);
    S32_SysTick->CVR  = 0UL;  // Load the SysTick counter value

    S32_SysTick->CSR |= S32_SysTick_CSR_CLKSOURCE(1);	//Select Core Clock
	S32_SysTick->CSR |= S32_SysTick_CSR_TICKINT(1);		//Enable Interrupt
	S32_SysTick->CSR |= S32_SysTick_CSR_ENABLE(1);		//Enable Systick

    return 0;  // Function successful
}
