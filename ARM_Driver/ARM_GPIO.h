#ifndef __ARM_GPIO__
#define __ARM_GPIO__

#include "stdint.h"
#include "ARM_NVIC.h"
#include "device_registers.h"

#define 	ARM_DRIVER_OK 					0	//Operation succeeded.
#define 	ARM_DRIVER_ERROR  				-1	//Unspecified error.
#define 	ARM_DRIVER_ERROR_BUSY   		-2	//Driver is busy.
#define 	ARM_DRIVER_ERROR_TIMEOUT   		-3	//Timeout occurred.
#define 	ARM_DRIVER_ERROR_UNSUPPORTED   	-4	//Operation not supported.
#define 	ARM_DRIVER_ERROR_PARAMETER   	-5	//Parameter error.
#define 	ARM_DRIVER_ERROR_SPECIFIC   	-6	//Start of driver specific errors.


#define 	ARM_GPIO_EVENT_RISING_EDGE    (1UL << 0) //Rising-edge detected.
#define 	ARM_GPIO_EVENT_FALLING_EDGE   (1UL << 1) //Falling-edge detected.
#define 	ARM_GPIO_EVENT_EITHER_EDGE    (1UL << 2) //Either edge detected (only when hardware cannot distinguish between rising and falling edge)


typedef uint32_t ARM_GPIO_Pin_t;
typedef void(* 	ARM_GPIO_SignalEvent_t) (ARM_GPIO_Pin_t pin, uint32_t event);

typedef enum ARM_GPIO_DIRECTION {
  ARM_GPIO_INPUT ,
  ARM_GPIO_OUTPUT
} ARM_GPIO_DIRECTION;

typedef enum ARM_GPIO_OUTPUT_MODE {
	  ARM_GPIO_PUSH_PULL ,
	  ARM_GPIO_OPEN_DRAIN
} ARM_GPIO_OUTPUT_MODE;

typedef enum ARM_GPIO_PULL_RESISTOR {
  ARM_GPIO_PULL_NONE ,
  ARM_GPIO_PULL_UP ,
  ARM_GPIO_PULL_DOWN
} ARM_GPIO_PULL_RESISTOR;

typedef enum ARM_GPIO_EVENT_TRIGGER {
  ARM_GPIO_TRIGGER_NONE ,
  ARM_GPIO_TRIGGER_RISING_EDGE ,
  ARM_GPIO_TRIGGER_FALLING_EDGE ,
  ARM_GPIO_TRIGGER_EITHER_EDGE
} ARM_GPIO_EVENT_TRIGGER;


typedef struct {
    int32_t (*Setup)(ARM_GPIO_Pin_t pin, ARM_GPIO_SignalEvent_t cb_event);
    int32_t (*SetDirection)(ARM_GPIO_Pin_t pin, ARM_GPIO_DIRECTION direction);
    int32_t (*SetOutputMode)(ARM_GPIO_Pin_t pin, ARM_GPIO_OUTPUT_MODE mode);
    int32_t (*SetPullResistor)(ARM_GPIO_Pin_t pin, ARM_GPIO_PULL_RESISTOR resistor);
    int32_t (*SetEventTrigger)(ARM_GPIO_Pin_t pin, ARM_GPIO_EVENT_TRIGGER trigger);
    void (*SetOutput)(ARM_GPIO_Pin_t pin, uint32_t val);
    uint32_t (*GetInput)(ARM_GPIO_Pin_t pin);
} ARM_DRIVER_GPIO;


int32_t ARM_GPIO_Setup (ARM_GPIO_Pin_t pin, ARM_GPIO_SignalEvent_t cb_event);
int32_t ARM_GPIO_SetDirection (ARM_GPIO_Pin_t pin, ARM_GPIO_DIRECTION direction);
int32_t ARM_GPIO_SetOutputMode (ARM_GPIO_Pin_t pin, ARM_GPIO_OUTPUT_MODE mode);
int32_t ARM_GPIO_SetPullResistor (ARM_GPIO_Pin_t pin, ARM_GPIO_PULL_RESISTOR resistor);
int32_t ARM_GPIO_SetEventTrigger (ARM_GPIO_Pin_t pin, ARM_GPIO_EVENT_TRIGGER trigger);
void ARM_GPIO_SetOutput (ARM_GPIO_Pin_t pin, uint32_t val);
uint32_t ARM_GPIO_GetInput (ARM_GPIO_Pin_t pin);
void ARM_GPIO_SignalEvent (ARM_GPIO_Pin_t pin, uint32_t event);

#endif
