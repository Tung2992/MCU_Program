#include "system_S32K144.h"
#include "device_registers.h"
#include "ARM_ADC.h"
#include "ARM_GPIO.h"
#include "ARM_LPUART.h"
#include "ARM_SYSTICK.h"
#include "My_Device.h"


#define RANGE_RESOLUTION	4095
#define RANGE_LEVEL			512

#define TRANSMIT_POTENTIOMETER_FRAME		0x00u
#define TRANSMIT_BUTTON_SW2_FRAME			0xF0u
#define TRANSMIT_BUTTON_SW3_FRAME			0xFFu

#define BUTTON_SW2		0
#define BUTTON_SW3		1

#define RECEIVE_CONFIRM_FRAME	0x00u
#define CONFIRM_SUCCESS			0x00u
#define CONFIRM_ERROR			0xFFu

#define RECEIVE_STATUS_FRAME 	0xFFu
#define STATUS_PLAY				0xFFu
#define STATUS_STOP				0x00u


#define TIME_BUTTON_DEFAULT		500

typedef struct  {
	uint32_t type 		: 8;
	uint32_t data		: 8;
	uint32_t check		: 8;
	uint32_t reserved	: 8;

} Data_Element_t;


typedef union {
	uint8_t send_data[3];
	Data_Element_t infor;
} Data_Transmit_Frame_t;

