#include "S32K144.h"
#include "ARM_ADC.h"
#include "My_Device.h"

void LPTMR0_IRQHandler(void);
void ADC0_IRQHandler(void);
void InitLPTMR(void);
void InitADC(void);

volatile uint32_t adc_result = 0;

int main(void) {
    // Initialize LPTMR and ADC

    InitADC();
	InitLPTMR();
	LPIT0->SETTEN |= LPIT_SETTEN_SET_T_EN_0(1);

    while (1) {
        // Main loop
    }

    return 0;
}

void InitLPTMR(void) {
//	SCG->FIRCCSR &= ~SCG_FIRCCSR_FIRCEN_MASK;    /* Disable Fast IRC */
//	SCG->FIRCDIV |= SCG_FIRCDIV_FIRCDIV2(1);	 /* Divider = 1 */
//	SCG->FIRCCSR |= SCG_FIRCCSR_FIRCEN(1);       /* Enable Fast IRC */

    // Enable clock for LPTMR
	PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_PCS(0b011);			// Choose clock source: Fast IRC
	PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC(1u);				// Enable clock

    /*Enable M_CEN first*/
    LPIT0->MCR |= (1 << LPIT_MCR_M_CEN_SHIFT) | (1u << LPIT_MCR_DBG_EN_SHIFT);

    /*Compare mode*/
    LPIT0->TMR[0].TCTRL &= ~LPIT_TMR_TCTRL_MODE_MASK; /* 32-bit Periodic Counter */

    /*Set LPIT channel 0 period*/
    LPIT0->TMR[0].TVAL = 23999999u; /*Fast IRC = 48000000*/

    // Enable LPIT channel 0 Interrupt
    LPIT0->MIER |= (1 << LPIT_MIER_TIE0_SHIFT);
//    S32_NVIC->ISER[LPIT0_Ch0_IRQn / 32] = (1 << LPIT0_Ch0_IRQn % 32);

    TRGMUX->TRGMUXn[TRGMUX_ADC0_INDEX] |= TRGMUX_TRGMUXn_SEL0(TRGMUX_TRIG_SOURCE_LPIT_CH0);
    SIM->ADCOPT |= SIM_ADCOPT_ADC0TRGSEL(1); /*1b - TRGMUX output */
    SIM->ADCOPT |= SIM_ADCOPT_ADC0PRETRGSEL(1);
    /*Enable Timer*/
//    LPIT0->SETTEN |= LPIT_SETTEN_SET_T_EN_0(1);

}

void InitADC(void) {
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

void LPIT0_Ch0_IRQHandler(void) {				// clear LPIT interrupt flag
	LPIT0->MSR |= LPIT_MSR_TIF0(1);
}

void ADC0_IRQHandler(void) {
    if (ADC0->SC1[0] & ADC_SC1_COCO_MASK) {
        adc_result = ADC0->R[POTENTIOMETER_INPUT_CHANNEL];
    }
}
