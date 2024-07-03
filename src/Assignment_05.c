#include "system_S32K144.h"
#include "device_registers.h"
#include "ARM_ADC.h"
#include "ARM_GPIO.h"
#include "ARM_SYSTICK.h"
#include "My_Device.h"

#define RANGE_RESOLUTION	4095
#define PMW_CYCLE_TIME		100


extern ARM_DRIVER_GPIO Driver_GPIO;
static ARM_DRIVER_GPIO * GPIOdrv = &Driver_GPIO;


volatile uint32_t adcResult;
volatile uint32_t count;


void Init_Devices() {

	/* LED_RED */
	GPIOdrv->Setup			(LED_RED_DEVICE, NULL);
	GPIOdrv->SetDirection	(LED_RED_DEVICE, ARM_GPIO_OUTPUT);
	GPIOdrv->SetOutput		(LED_RED_DEVICE, LED_OFF);

	/* LED_GREEN */
	GPIOdrv->Setup			(LED_GREEN_DEVICE, NULL);
	GPIOdrv->SetDirection	(LED_GREEN_DEVICE, ARM_GPIO_OUTPUT);
	GPIOdrv->SetOutput		(LED_GREEN_DEVICE, LED_OFF);


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


void ADC0_IRQHandler(void)
{
	adcResult = ARM_ADC_GetConversionResult(POTENTIOMETER_ADC, 0) * PMW_CYCLE_TIME / RANGE_RESOLUTION;
	ARM_ADC_StartConversion(POTENTIOMETER_ADC, 0, POTENTIOMETER_INPUT_CHANNEL);
}

void SysTick_Handler(void) {
	count++;
}


void PMW_Update() {
	if (count <= adcResult) {
		GPIOdrv->SetOutput(LED_RED_DEVICE, LED_ON);
	}
	else if ((adcResult < count) && (count <= PMW_CYCLE_TIME)) {
		GPIOdrv->SetOutput(LED_RED_DEVICE, LED_OFF);
	}
	else {
		count = 0;
	}
}


int main() {
	Init_Devices();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 100000);
	while(true) {
		PMW_Update();
	}
	return 1;
}
