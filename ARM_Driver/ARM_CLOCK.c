#include "ARM_CLOCK.h"

void Source_Clock(uint32_t source) {
	switch (source) {
	case FAST_IRC:
		SCG->FIRCCSR &= ~SCG_FIRCCSR_FIRCEN_MASK;    	/* Disable Fast IRC */
		SCG->FIRCDIV |= SCG_FIRCDIV_FIRCDIV2(1); 		/* Divide 1 */
		SCG->FIRCCSR |= SCG_FIRCCSR_FIRCEN(1);       	/* Enable Fast IRC */
		break;
	}

}
