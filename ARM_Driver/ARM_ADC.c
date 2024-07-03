#include "ARM_ADC.h"

static void Select_Source_Clock(ADC_Type *base) {
	if (base == ADC0) {
		PCC->PCCn[PCC_ADC0_INDEX] &= ~PCC_PCCn_CGC_MASK; /* Disable clock to change source */
		PCC->PCCn[PCC_ADC0_INDEX] &= ~PCC_PCCn_PCS_MASK;
//		SCG->FIRCDIV |= SCG_FIRCDIV_FIRCDIV2(1); /* Divide 1 */
		PCC->PCCn[PCC_ADC0_INDEX] |= PCC_PCCn_PCS(3); /* Peripheral Clock Source Select: FIRCDIV2_CLK */
		PCC->PCCn[PCC_ADC0_INDEX] |= PCC_PCCn_CGC_MASK;
	}
	else if (base == ADC1) {
		PCC->PCCn[PCC_ADC1_INDEX] &= ~PCC_PCCn_CGC_MASK; /* Disable clock to change source */
		PCC->PCCn[PCC_ADC1_INDEX] &= ~PCC_PCCn_PCS_MASK;
//		SCG->FIRCDIV |= SCG_FIRCDIV_FIRCDIV2(1); /* Divide 1 */
		PCC->PCCn[PCC_ADC1_INDEX] |= PCC_PCCn_PCS(3); /* Peripheral Clock Source Select: FIRCDIV2_CLK */
		PCC->PCCn[PCC_ADC1_INDEX] |= PCC_PCCn_CGC_MASK;
	}

}
adc_error_code_t ARM_ADC_Init (ADC_Type *base, const adc_config_t *adcConfig) {
	if (adcConfig == NULL) {
	        return ADC_ERROR;
	    }
	Select_Source_Clock(base);

	/* 4. Set ADC input clock source */
	base->CFG1 = (base->CFG1 & ~ADC_CFG1_ADICLK_MASK) | ((adcConfig->inputClock << ADC_CFG1_ADICLK_SHIFT) & ADC_CFG1_ADICLK_MASK);

	/* 1. Set ADC clock divide */
	base->CFG1 = (base->CFG1 & ~ADC_CFG1_ADIV_MASK) | ((adcConfig->clockDivide << ADC_CFG1_ADIV_SHIFT) & ADC_CFG1_ADIV_MASK);

	/* 2. Set ADC sample time */
	base->CFG2 = ADC_CFG2_SMPLTS(adcConfig->sampleTime);

	/* 3. Set ADC resolution */
	base->CFG1 = (base->CFG1 & ~ADC_CFG1_MODE_MASK) | ((adcConfig->resolution << ADC_CFG1_MODE_SHIFT) & ADC_CFG1_MODE_MASK);

	/* 5. Set ADC trigger type (software or hardware) */
	if (adcConfig->triggerType == 1) {
		base->SC2 |= ADC_SC2_ADTRG_MASK;  // Hardware trigger
	}
	else {
		base->SC2 &= ~ADC_SC2_ADTRG_MASK; // Software trigger
	}
	/* 6. Set ADC voltage reference */
	base->SC2 = (base->SC2 & ~ADC_SC2_REFSEL_MASK) | ((adcConfig->voltageRef << ADC_SC2_REFSEL_SHIFT) & ADC_SC2_REFSEL_MASK);

	/* 7. Enable/Disable DMA */
	if (adcConfig->dmaEnable) {
		base->SC2 |= ADC_SC2_DMAEN_MASK;  // Enable DMA
	}
	else {
		base->SC2 &= ~ADC_SC2_DMAEN_MASK; // Disable DMA
	}

	return ADC_SUCCESS;
}


adc_error_code_t ARM_ADC_DeInit(ADC_Type *base)
{
    // TODO: Reset all the configuration register and control register to the default value.
    base->CFG1 &= ~(ADC_CFG1_ADIV_MASK | ADC_CFG1_MODE_MASK | ADC_CFG1_ADICLK_MASK);
    base->CFG2 &= ~ADC_CFG2_SMPLTS_MASK;
    base->CFG2 |= ADC_CFG2_SMPLTS(0x0CU);
	uint8_t i = 0;
	for (i = 0; i < 16; i++)
	{
		base->SC1[i] &= ~(ADC_SC1_AIEN_MASK);
		base->SC1[i] |= ADC_SC1_ADCH_MASK;
	}
	base->SC2 &= ~(ADC_SC2_ADTRG_MASK | ADC_SC2_ACFE_MASK | ADC_SC2_ACFGT_MASK | ADC_SC2_ACREN_MASK | ADC_SC2_REFSEL_MASK | ADC_SC2_DMAEN_MASK);
	base->SC3 &= ~(ADC_SC3_CAL_MASK | ADC_SC3_ADCO_MASK | ADC_SC3_AVGE_MASK | ADC_SC3_AVGS_MASK);

    return 0xFFu; // success
}


adc_error_code_t ARM_ADC_ConfigChannel(ADC_Type *base, adc_channel_t * adcChannel)
{
	if (adcChannel == NULL)
	{
		return ADC_ERROR; // Null pointer error
	}

	// Check for valid controlChannel and inputChannel values
	if (adcChannel->controlChannel >= ADC_CHANNEL_COUNT || adcChannel->inputChannel >= ADC_INPUT_CHANNEL_COUNT)
	{
		return ADC_ERROR; // Invalid channel number error
	}

	if (adcChannel->enableInterrupt) {
		base->SC1[adcChannel->controlChannel] |= ADC_SC1_AIEN_MASK;
		if (base == ADC0) {
			NVIC_EnableIRQ(ADC0_IRQn);
		}
		else if (base == ADC1) {
			NVIC_EnableIRQ(ADC1_IRQn);
		}
	}
	else {
		base->SC1[adcChannel->controlChannel] &= ~ADC_SC1_AIEN_MASK;
	}

	// Configure the control channel
	base->SC1[adcChannel->controlChannel] &= ~ADC_SC1_ADCH_MASK;
	base->SC1[adcChannel->controlChannel] |= ADC_SC1_ADCH(adcChannel->inputChannel);

    return 0xFFu; // success
}

uint32_t ARM_ADC_GetConversionResult(ADC_Type *base, uint8_t controlChannel)
{
	if (controlChannel >= ADC_CHANNEL_COUNT) {
		return 0;
	}
    return (uint32_t)(base->R[controlChannel] & ADC_R_D_MASK);
}

void ARM_ADC_StartConversion(ADC_Type *base, uint8_t controlChannel, uint8_t inputChannel)
{
	base->SC1[controlChannel] |= ADC_SC1_ADCH(inputChannel);
}


