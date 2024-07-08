#include "ARM_LPIT.h"


void LPIT_Init(lpit_chnl_t channel)
{
	// Enable clock for LPTMR
	PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_PCS(0b011);			// Choose clock source: Fast IRC
	PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC(1u);				// Enable clock

	/*Enable M_CEN first*/
	LPIT0->MCR |= (1 << LPIT_MCR_M_CEN_SHIFT) | (1u << LPIT_MCR_DBG_EN_SHIFT);

	/*Compare mode*/
//	LPIT0->TMR[0].TCTRL &= ~LPIT_TMR_TCTRL_MODE_MASK; /* 32-bit Periodic Counter */
	LPIT0->TMR[channel].TCTRL |= LPIT_TMR_TCTRL_MODE(0u);

	/*Set LPIT channel 0 period*/
	LPIT0->TMR[channel].TVAL = LPIT_CLK;
//	LPIT0->TMR[0].TVAL = 23999999u; /*Fast IRC = 48000000*/

	switch (channel) {
	case LPIT_Chnl_0:
		LPIT0->MIER |= (1 << LPIT_MIER_TIE0_SHIFT);
//		NVIC_EnableIRQ(LPIT0_Ch0_IRQn);
		break;
	case LPIT_Chnl_1:
		LPIT0->MIER |= (1 << LPIT_MIER_TIE1_SHIFT);
		NVIC_EnableIRQ(LPIT0_Ch1_IRQn);
		break;
	case LPIT_Chnl_2:
		LPIT0->MIER |= (1 << LPIT_MIER_TIE2_SHIFT);
		NVIC_EnableIRQ(LPIT0_Ch2_IRQn);
		break;
	case LPIT_Chnl_3:
		LPIT0->MIER |= (1 << LPIT_MIER_TIE3_SHIFT);
		NVIC_EnableIRQ(LPIT0_Ch3_IRQn);
		break;
	}
	// Enable LPIT channel 0 Interrupt
//	LPIT0->MIER |= (1 << LPIT_MIER_TIE0_SHIFT);
//    S32_NVIC->ISER[LPIT0_Ch0_IRQn / 32] = (1 << LPIT0_Ch0_IRQn % 32);

	TRGMUX->TRGMUXn[TRGMUX_ADC0_INDEX] |= TRGMUX_TRGMUXn_SEL0(TRGMUX_TRIG_SOURCE_LPIT_CH0);
	SIM->ADCOPT |= SIM_ADCOPT_ADC0TRGSEL(1); /*1b - TRGMUX output */
	SIM->ADCOPT |= SIM_ADCOPT_ADC0PRETRGSEL(1);
	/*Enable Timer*/
//    LPIT0->SETTEN |= LPIT_SETTEN_SET_T_EN_0(1);
}

void LPIT_ResetVal(lpit_chnl_t channel,uint32_t value) {
	LPIT0->TMR[channel].TVAL = value;
}
void LPIT_Start(lpit_chnl_t channel) {
	LPIT0->TMR[channel].TCTRL |= LPIT_TMR_TCTRL_T_EN_MASK;
}

void LPIT_Stop(lpit_chnl_t channel) {
	LPIT0->TMR[channel].TCTRL &= ~LPIT_TMR_TCTRL_T_EN_MASK;
}

void LPIT_Deinit(void)
{
    //TODO: Read more information in reference manual to know how to de-initialize LPIT0 module.
}

