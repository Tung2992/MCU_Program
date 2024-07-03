#ifndef __SYSTICK__
#define __SYSTICK__

#include "device_registers.h"
extern uint32_t SystemCoreClock;

typedef void (*My_Systik_Handler)(void);

void My_Systick_Enable();
void My_Systick_SetTime(uint32_t time);
void My_Systick_Disable();


#endif
