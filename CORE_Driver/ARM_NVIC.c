#include "ARM_NVIC.h"

void NVIC_EnableIRQ(IRQn_Type IRQn){
	S32_NVIC->ISER[IRQn / 32] |= (1 << (IRQn % 32));  // Enable interrupt
}

void NVIC_SetPriority (IRQn_Type IRQn, uint32_t priority) {
	S32_NVIC->IP[IRQn / 4] |= priority << ((IRQn % 4) * 8);
}
