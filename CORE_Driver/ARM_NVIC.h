#ifndef __ARM_NVIC__
#define __ARM_NVIC__

#include "device_registers.h"

#define CMSIS_NVIC_VIRTUAL //Virtualization of the NVIC API.
#define CMSIS_VECTAB_VIRTUAL //Virtualization of interrupt vector table access functions.

//enum  IRQn_Type {
//  NonMaskableInt_IRQn = -14 ,
//  HardFault_IRQn = -13 ,
//  MemoryManagement_IRQn = -12 ,
//  BusFault_IRQn = -11 ,
//  UsageFault_IRQn = -10 ,
//  SecureFault_IRQn = -9 ,
//  SVCall_IRQn = -5 ,
//  DebugMonitor_IRQn = -4 ,
//  PendSV_IRQn = -2 ,
//  SysTick_IRQn = -1 ,
//  WWDG_STM_IRQn = 0 ,
//  PVD_STM_IRQn = 1
//};

void NVIC_EnableIRQ (IRQn_Type IRQn);
//void NVIC_DisableIRQ (IRQn_Type IRQn);
void NVIC_SetPriority (IRQn_Type IRQn, uint32_t priority);
//uint32_t NVIC_GetPriority (IRQn_Type IRQn);
//void NVIC_SystemReset (void);

#endif
