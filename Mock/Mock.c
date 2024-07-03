#include "Mock.h"

extern ARM_DRIVER_GPIO Driver_GPIO;
static ARM_DRIVER_GPIO * GPIOdrv = &Driver_GPIO;

Data_Transmit_Frame_t data_button_frame, data_potentiometer_frame, receive_frame;


static volatile uint8_t ready_sent_flag = 0;
static volatile uint8_t receive_flag = 0;

static volatile uint32_t count_time = 0;
static volatile uint32_t button_time = 0;

static volatile uint8_t button_status = 0;
static volatile uint8_t button_type;
static volatile uint8_t button_count = 0;

static uint32_t adcResult;
static uint8_t idx;

uint8_t Caculate_Checksum(uint8_t *ptr, uint8_t num) {
	uint8_t idx = 0;
	uint8_t checksum = 0;
	for (idx = 0; idx < num; idx++) {
		checksum += ptr[idx];
	}
	return (uint8_t)checksum;
}

void My_Button_Handler(uint8_t button_pin) {
	switch (button_pin) {
	case BUTTON_SW2_PIN:
		switch (button_status) {
		case 1:
			GPIOdrv->SetOutput (LED_BLUE_DEVICE, LED_ON);
			if (button_count == 0) {
				button_type = BUTTON_SW2;
				button_count++;
			}
			else if (button_type == BUTTON_SW3) {
				data_button_frame.infor.type = TRANSMIT_BUTTON_SW2_FRAME;
				data_button_frame.infor.data = button_count;
				data_button_frame.infor.check = Caculate_Checksum(data_button_frame.send_data, 2);
				button_count = 0;
				ready_sent_flag = 1;
			}
			else {
				button_count++;
			}
			break;
		case 0:
			GPIOdrv->SetOutput (LED_BLUE_DEVICE, LED_OFF);
			if ((button_time >= TIME_BUTTON_DEFAULT)) {
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
		case 1:
			GPIOdrv->SetOutput (LED_BLUE_DEVICE, LED_ON);
			if (button_count == 0) {
				button_type = BUTTON_SW3;
				button_count++;
			}
			else if (button_type == BUTTON_SW2) {
				data_button_frame.infor.type = TRANSMIT_BUTTON_SW3_FRAME;
				data_button_frame.infor.data = button_count;
				data_button_frame.infor.check = Caculate_Checksum(data_button_frame.send_data, 2);
				button_count = 0;
				ready_sent_flag = 1;
			}
			else {
				button_count++;
			}
			break;
		case 0:
			GPIOdrv->SetOutput (LED_BLUE_DEVICE, LED_OFF);
			if ((button_time >= TIME_BUTTON_DEFAULT)) {
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
		if (button_status == 0) {
			button_status = 1;
			button_time = 0;
		}
		else {
			button_status = 0;
		}
		My_Button_Handler(pin);
		break;
	}
}

void Source_Clock(void) {
	SCG->FIRCCSR &= ~SCG_FIRCCSR_FIRCEN_MASK;    	/* Disable Fast IRC */
	SCG->FIRCDIV |= SCG_FIRCDIV_FIRCDIV2(1); 		/* Divide 1 */
	SCG->FIRCCSR |= SCG_FIRCCSR_FIRCEN(1);       	/* Enable Fast IRC */
}

void Init_Devices() {
	Source_Clock();

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
			.clockDivide = ADIV_1,
			.dmaEnable = false,
			.inputClock = ADICLK_ALTCLK_1,
			.resolution = ADC_MODE_12,
			.sampleTime = ADC_AVG_SAMPLE_4,
			.triggerType = ADTRG_SW,
			.voltageRef = ADC_REFSEL_DEF
	};
	ARM_ADC_Init (POTENTIOMETER_ADC, &adc_config);

	adc_channel_t adc_channel = {
			.controlChannel = 0,
			.inputChannel = POTENTIOMETER_INPUT_CHANNEL,
			.enableInterrupt = false
	};
	ARM_ADC_ConfigChannel(POTENTIOMETER_ADC, &adc_channel);
}


void Init_Communication(void) {
	uart_config_t config = {
			.baudRate = 57600,
			.bitCountPerChar = UART_8_BITS_PER_CHAR,
			.parityMode = UART_PARITY_DISABLED,
			.stopBitCount = UART_ONE_STOP_BIT,
	};
	UART_Init(LPUART1, &config);
	UART_ReceiveNonBlocking(LPUART1);
}


void SysTick_Handler(void) {
	count_time++;
	button_time++;
}


void LPUART1_RxTx_IRQHandler(void) {
	if (LPUART1->STAT & LPUART_STAT_RDRF_MASK) {
		receive_frame.send_data[idx] = LPUART1->DATA;
		if (idx == 2) {
			idx = 0;
			receive_flag = 1;
		}
		else {
			idx++;
		}
	}
	else if (LPUART1->STAT & LPUART_STAT_TDRE_MASK) {
	}
}


int main() {
	Init_Devices();
	Init_Communication();
	uint8_t status;

	SysTick_Config(47999);

	while(true) {
		if (ready_sent_flag == 1) {
			UART_SendStringBlocking(LPUART1, data_button_frame.send_data, 3);
			ready_sent_flag = 0;
		}

		if (receive_flag == 1) {
			switch (receive_frame.infor.type) {
			case RECEIVE_STATUS_FRAME:
				if (receive_frame.infor.data == STATUS_PLAY) {
					GPIOdrv->SetOutput (LED_BLUE_DEVICE, LED_OFF);
					GPIOdrv->SetOutput (LED_RED_DEVICE, LED_OFF);
					GPIOdrv->SetOutput (LED_GREEN_DEVICE, LED_ON);
				}
				else if (receive_frame.infor.data == STATUS_STOP) {
					GPIOdrv->SetOutput (LED_BLUE_DEVICE, LED_OFF);
					GPIOdrv->SetOutput (LED_RED_DEVICE, LED_ON);
					GPIOdrv->SetOutput (LED_GREEN_DEVICE, LED_OFF);
				}
				break;
			case RECEIVE_CONFIRM_FRAME:

				break;
			}
			receive_flag = 0;
		}
		if (count_time >= 500) {
			adcResult = ARM_ADC_GetConversionResult(POTENTIOMETER_ADC, 0);

			if (adcResult < 100) {
				status = 0;
			}
			else if (adcResult <= RANGE_LEVEL * 1) {
				status = 1;
			}
			else if (adcResult <= RANGE_LEVEL * 2) {
				status = 2;
			}
			else if (adcResult <= RANGE_LEVEL * 3) {
				status = 3;
			}
			else if (adcResult <= RANGE_LEVEL * 4) {
				status = 4;
			}
			else if (adcResult <= RANGE_LEVEL * 5) {
				status = 5;
			}
			else if (adcResult <= RANGE_LEVEL * 6) {
				status = 6;
			}
			else if (adcResult <= RANGE_LEVEL * 7) {
				status = 7;
			}
			else {
				status = 8;
			}

			if (status != data_potentiometer_frame.infor.data) {
				data_potentiometer_frame.infor.type = TRANSMIT_POTENTIOMETER_FRAME;
				data_potentiometer_frame.infor.data = status;
				data_potentiometer_frame.infor.check = Caculate_Checksum(data_potentiometer_frame.send_data, 2);
				UART_SendStringBlocking(LPUART1, data_potentiometer_frame.send_data, 3);
			}

			count_time = 0;
			ARM_ADC_StartConversion(POTENTIOMETER_ADC, 0, POTENTIOMETER_INPUT_CHANNEL);
		}
	}
	return 0;
}
