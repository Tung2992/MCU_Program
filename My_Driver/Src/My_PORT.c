#include "My_PORT.h"

void My_PORT_Enable(PORT_Type * base) {
	if (base == PORTA) {
		PCC->PCCn[PCC_PORTA_INDEX] |= PCC_PCCn_CGC_MASK;
	}
	else if (base == PORTB) {
		PCC->PCCn[PCC_PORTB_INDEX] |= PCC_PCCn_CGC_MASK;
	}
	else if (base == PORTC) {
		PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC_MASK;
	}
	else if (base == PORTD) {
		PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;
	}
	else if (base == PORTE) {
		PCC->PCCn[PCC_PORTE_INDEX] |= PCC_PCCn_CGC_MASK;
	}
}

void MY_PORT_MUX(PORT_Type * base, uint8_t pin, PORT_MUX_t mux) {
	base->PCR[pin] &= ~PORT_PCR_MUX_MASK;
	if (mux != Analog) {
		base->PCR[pin] |= PORT_PCR_MUX(mux);
	}
}

void My_PORT_PullSelect(PORT_Type * base, uint8_t pin, PORT_PS_t ps) {
	switch (ps) {
	case PULL_Disable:
		base->PCR[pin] &= ~PORT_PCR_PE_MASK;
		break;
	case PULL_Down:
		base->PCR[pin] |= PORT_PCR_PE_MASK;
		base->PCR[pin] &= ~PORT_PCR_PS_MASK; //0 - pull down
		break;
	case PULL_Up:
		base->PCR[pin] |= PORT_PCR_PE_MASK;
		base->PCR[pin] |= PORT_PCR_PS_MASK;	//1 - pull up
		break;
	default:
		break;
	}
}

void My_PORT_IRQC(PORT_Type * base, uint8_t pin, PORT_IRQC_t irq) {
	base->PCR[pin] &= ~PORT_PCR_IRQC_MASK;
	if (irq != PORT_IRQ_Disable) {
		base->PCR[pin] |= PORT_PCR_IRQC(irq);
		if (base == PORTA) {
			S32_NVIC_EnableIRQ(PORTA_IRQn);
		}
		else if (base == PORTB) {
			S32_NVIC_EnableIRQ(PORTB_IRQn);
		}
		else if (base == PORTC) {
			S32_NVIC_EnableIRQ(PORTC_IRQn);
		}
		else if (base == PORTD) {
			S32_NVIC_EnableIRQ(PORTD_IRQn);
		}
		else if (base == PORTE) {
			S32_NVIC_EnableIRQ(PORTE_IRQn);
		}
	}
}

void My_PORT_Init(PORT_Type * base, uint8_t pin, PORT_PIN_Config_t * config) {
	My_PORT_Enable(base);
	MY_PORT_MUX(base, pin, config->MUX);
	My_PORT_PullSelect(base, pin, config->PS);
	My_PORT_IRQC(base, pin, config->IRQC);
}


