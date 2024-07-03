#ifndef __ADC__
#define __ADC__

#include "stdio.h"
#include "stdint.h"
#include "device_registers.h"
#include "ARM_NVIC.h"

#define ADC_CHANNEL_COUNT			16
#define ADC_INPUT_CHANNEL_COUNT		63

typedef enum {
	ADIV_1 = 0,
	ADIV_2 = 1,
	ADIV_4 = 2,
	ADIV_8 = 3
} clockDivide_t;

typedef enum {
	ADC_AVG_SAMPLE_4 	= 4,
	ADC_AVG_SAMPLE_8	= 8,
	ADC_AVG_SAMPLE_16	= 16,
	ADC_AVG_SAMPLE_32	= 32
} sampleTime_t;


typedef enum {
	 ADC_MODE_8			= 0,
	 ADC_MODE_12		= 1,
	 ADC_MODE_10 		= 2,
} resolution_t;

typedef enum {
	ADICLK_ALTCLK_1		= 0,
} inputClock_t;

typedef enum {
	ADTRG_SW	= 0,
	ADTRG_HW	= 1
} triggerType_t;

typedef enum {
	ADC_REFSEL_DEF	= 0,
	ADC_REFSEL_ALT	= 1
} voltageRef_t;


typedef struct
{
	clockDivide_t clockDivide;
	sampleTime_t sampleTime;
	resolution_t resolution;
	inputClock_t inputClock;
	triggerType_t triggerType;
	voltageRef_t voltageRef;
	bool dmaEnable;
} adc_config_t;

typedef struct
{
    uint8_t controlChannel;
    uint8_t inputChannel;
    bool    enableInterrupt;
} adc_channel_t;


typedef enum
{
    ADC_ERROR = 0x01U,
    ADC_SUCCESS = 0xFFU,
} adc_error_code_t;


adc_error_code_t ARM_ADC_Init (ADC_Type *base, const adc_config_t *adcConfig);
adc_error_code_t ARM_ADC_DeInit(ADC_Type *base);
adc_error_code_t ARM_ADC_ConfigChannel(ADC_Type *base, adc_channel_t * adcChannel);
uint32_t ARM_ADC_GetConversionResult(ADC_Type *base, uint8_t controlChannel);
void ARM_ADC_StartConversion(ADC_Type *base, uint8_t controlChannel, uint8_t inputChannel);

#endif
