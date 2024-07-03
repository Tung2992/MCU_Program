//#include "system_S32K144.h"
//#include ""
//
//void LPIT_Init(void)
//{
//	/* Step 1: Enable the peripheral clock by setting the M_CEN bit in the MCR register */
//	    LPIT0->MCR |= LPIT_MCR_M_CEN_MASK;
//
//	    /* Step 2: Wait for 4 peripheral clock cycles (this is a very short delay, so a simple loop will suffice) */
//	    for (volatile int i = 0; i < 4; i++) {
//	        __asm("nop");
//	    }
//
//	    /* Step 3: Configure timer control bits for each timer channel */
//	    /* Example configuration for channel 0: */
//	    LPIT0->TCTRL[0] = LPIT_TCTRL_MODE(0) |  /* Timer mode: 32-bit periodic counter */
//	                      LPIT_TCTRL_TRG_SEL(0) | /* Trigger source selection: No trigger */
//	                      LPIT_TCTRL_TROT_MASK |  /* Trigger rotation: enabled */
//	                      LPIT_TCTRL_TSOT_MASK |  /* Timer start on trigger: enabled */
//	                      LPIT_TCTRL_TSOI_MASK;   /* Timer stop on interrupt: enabled */
//
//	    /* Step 4: Configure channels that are to be chained */
//	    /* Example: No chaining for channel 0 */
//	    // LPIT0->TCTRL[0] &= ~LPIT_TCTRL_CHAIN_MASK;
//
//	    /* Step 5: Set the timer timeout value for channels configured in Compare Mode */
//	    /* Example: Timeout value for channel 0 */
//	    LPIT0->TVAL[0] = 1000;  /* Timeout value */
//
//	    /* Step 6: Configure TIEn bits in MIER register for those channels that need to generate interrupts */
//	    LPIT0->MIER |= LPIT_MIER_TIE0_MASK;  /* Enable interrupt for channel 0 */
//
//	    /* Step 7: Configure low power modes by setting DBG_EN and DOZE_EN bits in the MCR register */
//	    LPIT0->MCR |= LPIT_MCR_DBG_EN_MASK | LPIT_MCR_DOZE_EN_MASK;
//
//	    /* Step 8: Enable the channel timers by setting the T_EN bit in the corresponding channel's TCTRL register */
//	    LPIT0->TCTRL[0] |= LPIT_TCTRL_T_EN_MASK;
//}
//
//void LPIT_Deinit(void)
//{
//    //TODO: Read more information in reference manual to know how to de-initialize LPIT0 module.
//}

