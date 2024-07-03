#include "My_GPIO.h"


void My_GPIO_Init (GPIO_Type* base,  uint8_t pin, GPIO_DIRECTION direction) {

	if (direction == GPIO_INPUT) {
		base->PDDR &= ~GPIO_PDDR_PDD(1 << pin);
	}
	else {
		base->PDDR |= GPIO_PDDR_PDD(1 << pin);
		My_GPIO_Clear(base, pin);
	}
}

void My_GPIO_Set (GPIO_Type* base,  uint8_t pin) {
	base->PSOR |= GPIO_PSOR_PTSO(1 << pin);
}

void My_GPIO_Clear (GPIO_Type* base,  uint8_t pin) {
	base->PCOR |= GPIO_PCOR_PTCO(1 << pin);
}

void My_GPIO_Toggle (GPIO_Type* base,  uint8_t pin) {
	base->PTOR |= GPIO_PTOR_PTTO(1 << pin);
}

uint8_t My_GPIO_Read (GPIO_Type* base,  uint8_t pin) {
	return (base->PDIR & (1 << pin)) ? 1 : 0;
}

