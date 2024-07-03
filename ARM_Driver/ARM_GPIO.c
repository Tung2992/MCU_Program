#include "ARM_GPIO.h"

static ARM_GPIO_SignalEvent_t gpio_event;

static GPIO_Type* Get_GPIO_Base(ARM_GPIO_Pin_t pin) {
	if (pin < 32) {
		return PTA;
	}
	else if (pin < 64) {
		return PTB;
	}
	else if (pin < 96) {
		return PTC;
	}
	else if (pin < 128) {
		return PTD;
	}
	else {
		return PTE;
	}
}

static PORT_Type* Get_PORT_Base(ARM_GPIO_Pin_t pin) {
	if (pin < 32) {
		return PORTA;
	}
	else if (pin < 64) {
		return PORTB;
	}
	else if (pin < 96) {
		return PORTC;
	}
	else if (pin < 128) {
		return PORTD;
	}
	else {
		return PORTE;
	}
}

int32_t ARM_GPIO_Setup (ARM_GPIO_Pin_t pin, ARM_GPIO_SignalEvent_t cb_event) {
	if (pin >= 160) {
		return ARM_DRIVER_ERROR_PARAMETER;
	}
	PORT_Type* PORT_base = Get_PORT_Base(pin);
	uint8_t set_pin = pin % 32;

	if (pin < 32) {
		PCC->PCCn[PCC_PORTA_INDEX] |= PCC_PCCn_CGC_MASK;
	}
	else if (pin < 64) {
		PCC->PCCn[PCC_PORTB_INDEX] |= PCC_PCCn_CGC_MASK;
	}
	else if (pin < 96) {
		PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC_MASK;
	}
	else if (pin < 128) {
		PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;
	}
	else {
		PCC->PCCn[PCC_PORTE_INDEX] |= PCC_PCCn_CGC_MASK;
	}

	PORT_base->PCR[set_pin] &= ~PORT_PCR_MUX_MASK;
	PORT_base->PCR[set_pin] |= PORT_PCR_MUX(1);

	gpio_event = cb_event;
	return ARM_DRIVER_OK;
}

int32_t ARM_GPIO_SetDirection (ARM_GPIO_Pin_t pin, ARM_GPIO_DIRECTION direction) {
	if (pin >= 160) {
		return ARM_DRIVER_ERROR_PARAMETER;
	}
	GPIO_Type* GPIO_base = Get_GPIO_Base(pin);
	uint8_t set_pin = pin % 32;

	if (direction == ARM_GPIO_INPUT) {
		GPIO_base->PDDR &= ~GPIO_PDDR_PDD(1 << set_pin);
	}
	else {
		GPIO_base->PDDR |= GPIO_PDDR_PDD(1 << set_pin);
	}
	return ARM_DRIVER_OK;
}

int32_t ARM_GPIO_SetOutputMode (ARM_GPIO_Pin_t pin, ARM_GPIO_OUTPUT_MODE mode) {
	 if (pin >= 160) {
		return ARM_DRIVER_ERROR_PARAMETER;
	}

	// Implementation based on the specific MCU capabilities
	return ARM_DRIVER_OK;
}

int32_t ARM_GPIO_SetPullResistor (ARM_GPIO_Pin_t pin, ARM_GPIO_PULL_RESISTOR resistor) {
	if (pin >= 160) {
		return ARM_DRIVER_ERROR_PARAMETER;
	}
	PORT_Type* PORT_base = Get_PORT_Base(pin);
	uint8_t set_pin = pin % 32;

	if (resistor == ARM_GPIO_PULL_UP) {
		PORT_base->PCR[set_pin] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	}
	else if (resistor == ARM_GPIO_PULL_DOWN) {
		PORT_base->PCR[set_pin] |= PORT_PCR_PE_MASK;
		PORT_base->PCR[set_pin] &= ~PORT_PCR_PS_MASK;
	}
	else if (resistor == ARM_GPIO_PULL_NONE){
		PORT_base->PCR[set_pin] &= ~PORT_PCR_PE_MASK;
	}
	else {
		return ARM_DRIVER_ERROR_PARAMETER;
	}
	return ARM_DRIVER_OK;
}

int32_t ARM_GPIO_SetEventTrigger (ARM_GPIO_Pin_t pin, ARM_GPIO_EVENT_TRIGGER trigger) {
	if (pin >= 160) {
		return ARM_DRIVER_ERROR_PARAMETER;
	}

	PORT_Type* PORT_base = Get_PORT_Base(pin);
	uint8_t set_pin = pin % 32;

	PORT_base->PCR[set_pin] &= ~PORT_PCR_IRQC_MASK;
	if (trigger == ARM_GPIO_TRIGGER_RISING_EDGE) {
		PORT_base->PCR[set_pin] |= PORT_PCR_IRQC(9);
	}
	else if (trigger == ARM_GPIO_TRIGGER_FALLING_EDGE) {
		PORT_base->PCR[set_pin] |= PORT_PCR_IRQC(10);
	}
	else if (trigger == ARM_GPIO_TRIGGER_EITHER_EDGE) {
		PORT_base->PCR[set_pin] |= PORT_PCR_IRQC(11);
	}
	else {
		return ARM_DRIVER_ERROR_PARAMETER;
	}

	if (PORT_base == PORTA && trigger != ARM_GPIO_TRIGGER_NONE) {
		NVIC_EnableIRQ(PORTA_IRQn);
	}
	else if (PORT_base == PORTB && trigger != ARM_GPIO_TRIGGER_NONE) {
		NVIC_EnableIRQ(PORTB_IRQn);
	}
	else if (PORT_base == PORTC && trigger != ARM_GPIO_TRIGGER_NONE) {
		NVIC_EnableIRQ(PORTC_IRQn);
	}
	else if (PORT_base == PORTD && trigger != ARM_GPIO_TRIGGER_NONE) {
		NVIC_EnableIRQ(PORTD_IRQn);
	}
	else if (PORT_base == PORTE && trigger != ARM_GPIO_TRIGGER_NONE) {
		NVIC_EnableIRQ(PORTE_IRQn);
	}
	else {
		return ARM_DRIVER_ERROR_PARAMETER;
	}

	return ARM_DRIVER_OK;
}

void ARM_GPIO_SetOutput (ARM_GPIO_Pin_t pin, uint32_t val) {
	if (pin >= 160) {
		return;
	}

	GPIO_Type* GPIO_base = Get_GPIO_Base(pin);
	uint8_t set_pin = pin % 32;
	if (val) {
		GPIO_base->PSOR |= GPIO_PSOR_PTSO(1 << set_pin);
	}
	else {
		GPIO_base->PCOR |= GPIO_PCOR_PTCO(1 << set_pin);
	}
}

uint32_t ARM_GPIO_GetInput (ARM_GPIO_Pin_t pin) {
	if (pin >= 160) {
		return 0;
	}

	GPIO_Type* GPIO_base = Get_GPIO_Base(pin);
	uint8_t set_pin = pin % 32;

	return (GPIO_base->PDIR & (1 << set_pin)) ? 1 : 0;
}

void ARM_GPIO_SignalEvent (ARM_GPIO_Pin_t pin, uint32_t event) {
	if (gpio_event) {
		gpio_event(pin, event);
	}
}


ARM_DRIVER_GPIO Driver_GPIO =
{
	.Setup = ARM_GPIO_Setup,
	.SetDirection = ARM_GPIO_SetDirection,
	.SetOutputMode =ARM_GPIO_SetOutputMode,
	.SetPullResistor = ARM_GPIO_SetPullResistor,
	.SetEventTrigger = ARM_GPIO_SetEventTrigger,
	.SetOutput = ARM_GPIO_SetOutput,
    .GetInput = ARM_GPIO_GetInput
};


void PORTC_IRQHandler(void) {
	uint8_t pin;
	for (pin = 0; pin < 32; pin++) {
		if(PORTC->ISFR & (1 << pin)) {
			PORTC->ISFR |= (1 << pin);
			break;
		}
	}
	ARM_GPIO_SignalEvent(pin, ARM_GPIO_EVENT_EITHER_EDGE);
}


