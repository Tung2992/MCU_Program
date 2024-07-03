#ifndef __PORT__
#define __PORT__

#include "device_registers.h"
#include "My_NVIC.h"

typedef enum {
	Analog 		= 0U,
	GPIO		= 1U,
	Alter_1		= 2U,
	Alter_2		= 3U,
	Alter_3		= 4U,
	Alter_4		= 5U,
	Alter_5		= 6U,
	Alter_6		= 7U,
	Alter_7		= 8U,
} PORT_MUX_t;

typedef enum {
	PULL_Disable	= 0U,
	PULL_Down		= 1U,
	PULL_Up			= 2U,
} PORT_PS_t;

typedef enum {
	PORT_IRQ_Disable		= 0U,
	DMA_Rising_Edge			= 1U,
	DMA_Falling_Edge		= 2U,
	DMA_Either_Edge			= 3U,
	Interrupt_Logic_0		= 8U,
	Interrupt_Rising_Edge	= 9U,
	Interrupt_Falling_Edge	= 10U,
	Interrupt_Either_Edge	= 11U,
	Interrupt_Logic_1		= 12U,
}PORT_IRQC_t;

//typedef void (*PORT_IRQPtr)(uint8_t);

typedef struct {
	PORT_MUX_t MUX;
	PORT_IRQC_t IRQC;
	PORT_PS_t PS;
} PORT_PIN_Config_t;


void My_PORT_Init(PORT_Type * base, uint8_t pin, PORT_PIN_Config_t * config);

#endif
