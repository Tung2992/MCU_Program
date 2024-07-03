#include "My_NVIC.h"

void S32_NVIC_EnableIRQ(IRQn_Type IRQn){
	S32_NVIC->ISER[IRQn / 32] = (1 << (IRQn % 32));  // Enable interrupt
}
