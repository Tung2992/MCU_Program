/*
 * main implementation: use this 'C' sample to create your own application
 *
 */
#include "S32K144.h"
#include "stdint.h"

volatile uint32_t g_sys_tick = 0;
volatile uint32_t red_last_tick = 0;
int i = 720000;

void Init_LPIT() {
    SCG->FIRCCSR &= ~SCG_FIRCCSR_FIRCEN_MASK;    /* Disable Fast IRC */
	SCG->FIRCDIV |= SCG_FIRCDIV_FIRCDIV2(1);	 /* Divider = 1 */
	SCG->FIRCCSR |= SCG_FIRCCSR_FIRCEN(1);       /* Enable Fast IRC */

	while(i--);

    PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_PCS(0b011);			// Choose clock source: Fast IRC
    PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC(1u);				// Enable clock

    /*Enable M_CEN first*/
    LPIT0->MCR |= (1 << LPIT_MCR_M_CEN_SHIFT) | (1u << LPIT_MCR_DBG_EN_SHIFT);

    /*Compare mode*/
    LPIT0->TMR[0].TCTRL |= LPIT_TMR_TCTRL_MODE(0u);

    /*Set LPIT channel 0 period*/
    LPIT0->TMR[0].TVAL = 479u;

    /*---------------------------------------------------------------------
     * Setup control register
     * - compare mode
     * - select triger source: LPIT channel 0, internal triger
     * - start on triger
     * - chain with LPIT channel 0
     * ----------------------------------------------------------------------*/
    LPIT0->TMR[1].TCTRL |= LPIT_TMR_TCTRL_MODE(0u) | LPIT_TMR_TCTRL_TRG_SRC_MASK | LPIT_TMR_TCTRL_TRG_SEL(0) | LPIT_TMR_TCTRL_TSOT_MASK | LPIT_TMR_TCTRL_CHAIN_MASK;

    /*Set LPIT channel 1 period*/
    LPIT0->TMR[1].TVAL = 9u;

    /*---------------------------------------------------------------------
	 * Setup control register
	 * - compare mode
	 * - select triger source: LPIT channel 1, internal triger
	 * - start on triger
	 * - chain with LPIT channel 1
	 * ----------------------------------------------------------------------*/
    LPIT0->TMR[2].TCTRL |= LPIT_TMR_TCTRL_MODE(0u) | LPIT_TMR_TCTRL_TRG_SRC_MASK | LPIT_TMR_TCTRL_TRG_SEL(1) | LPIT_TMR_TCTRL_TSOT_MASK | LPIT_TMR_TCTRL_CHAIN_MASK;

    /*Set LPIT channel 2 period*/
    LPIT0->TMR[2].TVAL = 9u;

    // Enable LPIT channel 2 Interrupt
    LPIT0->MIER |= (1 << LPIT_MIER_TIE2_SHIFT);

    // Enable LPIT channel 0 - 2
	LPIT0->SETTEN |= 0x07;

    S32_NVIC->ISER[LPIT0_Ch2_IRQn / 32] = (1 << LPIT0_Ch2_IRQn % 32);
}

void LPIT0_Ch2_IRQHandler(void) {				// clear LPIT interrupt flag
	LPIT0->MSR |= LPIT_MSR_TIF2(1);
    g_sys_tick++;
//    PTD->PTOR |= (1 << 15);
}


void GPIO_Init(void) {
	PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC(1);

	PORTD->PCR[16] &= ~PORT_PCR_MUX_MASK;				// setup green led pin
	PORTD->PCR[16] |= PORT_PCR_MUX(1);
	PTD->PDDR |= (1u << 16);

	PTD->PSOR |= (1u << 16);
}

//void LPIT0_Ch2_IRQHandler(void) {
//	LPIT0->MSR |= LPIT_MSR_TIF2(1);
//    g_sys_tick++;
////    PTD->PTOR |= (1 << 15);
//}

int main(void) {
	GPIO_Init();
	Init_LPIT();
    while(1) {
		if (g_sys_tick - red_last_tick >= 2000) {
			red_last_tick = g_sys_tick;
			PTD->PTOR |= (1 << 16);
		}
    }
    return 0;
}
