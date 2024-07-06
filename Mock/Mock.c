#include "Mock.h"



extern ARM_DRIVER_GPIO Driver_GPIO;
static ARM_DRIVER_GPIO * GPIOdrv = &Driver_GPIO;

Data_Frame_t data_transmit_frame, data_receive_frame, data_temp_frame;

volatile uint8_t transmit_flag = 0;
volatile uint8_t receive_flag = 0;
volatile uint8_t request_resend_flag = 0;

volatile uint32_t button_time = 0;
volatile uint32_t receive_time = 0;
volatile uint32_t transmit_time = 0;

volatile uint8_t button_status = 0;
volatile uint8_t button_type;
volatile uint8_t button_count = 0;

volatile uint32_t adcResult;
volatile uint8_t idx;


uint8_t buf_Rx[3] = {0};
uint8_t buf_Tx[3] = {0};
uint8_t Error_Frame[3] = {0x00, 0xFF, 0xFF};


volatile uint8_t current_status;
volatile uint8_t previous_status;

uint8_t Caculate_Checksum(uint8_t *ptr, uint8_t num) {
	uint8_t idx = 0;
	uint8_t checksum = 0;
	for (idx = 0; idx < num; idx++) {
		checksum += ptr[idx];
	}
	return (uint8_t)checksum;
}

void My_Button_Handler(uint8_t button_pin) {
	if (button_status == RELEASE) {
		button_status = PRESS;
		button_time = 0;
	}
	else {
		button_status = RELEASE;
	}
	switch (button_pin) {
	case BUTTON_SW2_PIN:
		switch (button_status) {
		case PRESS:
			GPIOdrv->SetOutput (LED_BLUE_DEVICE, LED_ON);
			if (button_count == 0) {
				button_type = BUTTON_SW2;
				button_count++;
			}
			else if (button_type == BUTTON_SW3) {
				data_temp_frame.infor.type = TRANSMIT_BUTTON_SW2_FRAME;
				data_temp_frame.infor.data = button_count;
				data_temp_frame.infor.check = Caculate_Checksum(data_temp_frame.array_data, 2);

				button_count = 0;
				transmit_flag = 1;
			}
			else {
				button_count++;
			}
			break;
		case RELEASE:
			GPIOdrv->SetOutput (LED_BLUE_DEVICE, LED_OFF);
			if ((button_time >= BUTTON_TIME)) {
				if (button_count == 2) {
					button_count--;
				}
				else if (button_count > 2) {
					button_count -= 2;
				}
			}
			break;
		}
		break;
	case BUTTON_SW3_PIN:
		switch (button_status) {
		case PRESS:
			GPIOdrv->SetOutput (LED_BLUE_DEVICE, LED_ON);
			if (button_count == 0) {
				button_type = BUTTON_SW3;
				button_count++;
			}
			else if (button_type == BUTTON_SW2) {
				data_temp_frame.infor.type = TRANSMIT_BUTTON_SW3_FRAME;
				data_temp_frame.infor.data = button_count;
				data_temp_frame.infor.check = Caculate_Checksum(data_temp_frame.array_data, 2);
				button_count = 0;
				transmit_flag = 1;
			}
			else {
				button_count++;
			}
			break;
		case RELEASE:
			GPIOdrv->SetOutput (LED_BLUE_DEVICE, LED_OFF);
			if ((button_time >= BUTTON_TIME)) {
				if (button_count == 2) {
					button_count--;
				}
				else if (button_count > 2) {
					button_count -= 2;
				}
			}
			break;
		}
		break;
	}
}


void GPIO_Event_Handler(ARM_GPIO_Pin_t pin, uint32_t event) {
	switch (event) {
	case ARM_GPIO_EVENT_RISING_EDGE:
		break;
	case ARM_GPIO_EVENT_FALLING_EDGE:
		break;
	case ARM_GPIO_EVENT_EITHER_EDGE:
		My_Button_Handler(pin);
		break;
	}
}



void Init_Devices() {
	Source_Clock(FAST_IRC);

	/* LED_RED */
	GPIOdrv->Setup			(LED_RED_DEVICE, NULL);
	GPIOdrv->SetDirection	(LED_RED_DEVICE, ARM_GPIO_OUTPUT);
	GPIOdrv->SetOutput		(LED_RED_DEVICE, LED_OFF);

	/* LED_GREEN */
	GPIOdrv->Setup			(LED_GREEN_DEVICE, NULL);
	GPIOdrv->SetDirection	(LED_GREEN_DEVICE, ARM_GPIO_OUTPUT);
	GPIOdrv->SetOutput		(LED_GREEN_DEVICE, LED_OFF);

	/* LED_BLUE */
	GPIOdrv->Setup			(LED_BLUE_DEVICE, NULL);
	GPIOdrv->SetDirection	(LED_BLUE_DEVICE, ARM_GPIO_OUTPUT);
	GPIOdrv->SetOutput		(LED_BLUE_DEVICE, LED_OFF);

	/* BUTTON_SW2 */
	GPIOdrv->Setup			(BUTTON_SW2_DEVICE, GPIO_Event_Handler);
	GPIOdrv->SetDirection	(BUTTON_SW2_DEVICE, ARM_GPIO_INPUT);
	GPIOdrv->SetEventTrigger(BUTTON_SW2_DEVICE, ARM_GPIO_TRIGGER_EITHER_EDGE);

	/* BUTTON_SW3 */
	GPIOdrv->Setup			(BUTTON_SW3_DEVICE, GPIO_Event_Handler);
	GPIOdrv->SetDirection	(BUTTON_SW3_DEVICE, ARM_GPIO_INPUT);
	GPIOdrv->SetEventTrigger(BUTTON_SW3_DEVICE, ARM_GPIO_TRIGGER_EITHER_EDGE);

	/* POTENTIOMETER */
	adc_config_t adc_config = {
			.clockDivide 	= ADIV_1,
			.dmaEnable 		= false,
			.inputClock		= ADICLK_ALTCLK_1,
			.resolution 	= ADC_MODE_12,
			.sampleTime 	= ADC_AVG_SAMPLE_4,
			.triggerType	= ADTRG_HW,
			.voltageRef 	= ADC_REFSEL_DEF
	};
	ARM_ADC_Init (POTENTIOMETER_ADC, &adc_config);

	adc_channel_t adc_channel = {
			.controlChannel 	= 0,
			.inputChannel 		= POTENTIOMETER_INPUT_CHANNEL,
			.enableInterrupt 	= true
	};
	ARM_ADC_ConfigChannel(POTENTIOMETER_ADC, &adc_channel);
}


void Init_Communication(void) {
	uart_config_t config = {
			.baudRate 			= 57600,
			.bitCountPerChar 	= UART_8_BITS_PER_CHAR,
			.parityMode 		= UART_PARITY_DISABLED,
			.stopBitCount 		= UART_ONE_STOP_BIT,
	};
	UART_Init(LPUART1, &config);
	UART_ReceiveNonBlocking(LPUART1);
}


void SysTick_Handler(void) {
	button_time++;
	receive_time++;
	transmit_time++;
}


void LPUART1_RxTx_IRQHandler(void) {
	if (LPUART1->STAT & LPUART_STAT_RDRF_MASK) {
		if (idx == 0) {
			buf_Rx[idx] = LPUART1->DATA;
			idx++;
			receive_time = 0;
		}
		else if (receive_time >= 200) {
			idx = 0;
		}
		else if (idx == 2) {
			buf_Rx[idx] = LPUART1->DATA;
			idx = 0;
			if (receive_flag == 0) {
				data_receive_frame.array_data[0] = buf_Rx[0];
				data_receive_frame.array_data[1] = buf_Rx[1];
				data_receive_frame.array_data[2] = buf_Rx[2];
				receive_flag = 1;
			}
		}
		else {
			buf_Rx[idx] = LPUART1->DATA;
			idx++;
		}
	}
	else if (LPUART1->STAT & LPUART_STAT_TDRE_MASK) {

	}
}

void ADC0_IRQHandler(void) {
	if (ADC0->SC1[0] & ADC_SC1_COCO_MASK) {
		adcResult = ADC0->R[0];
		if (adcResult < 100) {
			current_status = 0;
		}
		else if (adcResult <= RANGE_LEVEL * 1) {
			current_status = 1;
		}
		else if (adcResult <= RANGE_LEVEL * 2) {
			current_status = 2;
		}
		else if (adcResult <= RANGE_LEVEL * 3) {
			current_status = 3;
		}
		else if (adcResult <= RANGE_LEVEL * 4) {
			current_status = 4;
		}
		else if (adcResult <= RANGE_LEVEL * 5) {
			current_status = 5;
		}
		else if (adcResult <= RANGE_LEVEL * 6) {
			current_status = 6;
		}
		else if (adcResult <= RANGE_LEVEL * 7) {
			current_status = 7;
		}
		else {
			current_status = 8;
		}

		if (current_status != previous_status) {
			data_temp_frame.infor.type = TRANSMIT_POTENTIOMETER_FRAME;
			data_temp_frame.infor.data = current_status;
			data_temp_frame.infor.check = Caculate_Checksum(data_temp_frame.array_data, 2);
			previous_status = current_status;
			transmit_flag = 1;
		}
	}
}


int main() {
	Init_Devices();
	Init_Communication();
	SysTick_Config(47999);
	LPIT_Init();

	while(true) {
		if ((transmit_flag == 1) && (transmit_time >= WAIT_TIME)) {
			data_transmit_frame.array_data[0] = data_temp_frame.array_data[0];
			data_transmit_frame.array_data[1] = data_temp_frame.array_data[1];
			data_transmit_frame.array_data[2] = data_temp_frame.array_data[2];
			UART_SendStringBlocking(LPUART1, data_transmit_frame.array_data, 3);
			transmit_time = 0;
			transmit_flag = 0;
			request_resend_flag = 0;
		}
		else if ((request_resend_flag == 1) && (transmit_time < WAIT_TIME)) {
			UART_SendStringBlocking(LPUART1, data_transmit_frame.array_data, 3);
			transmit_time = 0;
			request_resend_flag = 0;
		}



		if ((receive_flag == 1) && (data_receive_frame.array_data[2] == Caculate_Checksum(data_receive_frame.array_data, 2))) {
			switch (data_receive_frame.infor.type) {
			case RECEIVE_STATUS_FRAME:
				if (data_receive_frame.infor.data == STATUS_PLAY) {
					GPIOdrv->SetOutput (LED_BLUE_DEVICE, LED_OFF);
					GPIOdrv->SetOutput (LED_RED_DEVICE, LED_OFF);
					GPIOdrv->SetOutput (LED_GREEN_DEVICE, LED_ON);
				}
				else if (data_receive_frame.infor.data == STATUS_STOP) {
					GPIOdrv->SetOutput (LED_BLUE_DEVICE, LED_OFF);
					GPIOdrv->SetOutput (LED_RED_DEVICE, LED_ON);
					GPIOdrv->SetOutput (LED_GREEN_DEVICE, LED_OFF);
				}
				break;
			case RECEIVE_CONFIRM_FRAME:
				if (data_receive_frame.infor.data == CONFIRM_ERROR) {
					request_resend_flag = 1;
				}
				break;
			}
			receive_flag = 0;
		}
		else if ((receive_flag == 1) && (data_receive_frame.array_data[2] != Caculate_Checksum(data_receive_frame.array_data, 2))) {
			UART_SendStringBlocking(LPUART1, Error_Frame, 3);
			receive_flag = 0;
		}
	}
	return 0;
}
