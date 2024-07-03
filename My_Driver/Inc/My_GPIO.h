#ifndef __GPIO__
#define __GPIO__


#include <stdint.h>
#include "device_registers.h"


typedef enum {
	GPIO_INPUT		= 0,
	GPIO_OUTPUT		= 1,
}GPIO_DIRECTION;

void My_GPIO_Init (GPIO_Type* base,  uint8_t pin, GPIO_DIRECTION direction);
void My_GPIO_Set (GPIO_Type* base,  uint8_t pin);
void My_GPIO_Clear (GPIO_Type* base,  uint8_t pin);
void My_GPIO_Toggle (GPIO_Type* base,  uint8_t pin);
uint8_t My_GPIO_Read (GPIO_Type* base,  uint8_t pin);

#endif
