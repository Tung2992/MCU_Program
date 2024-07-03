#include "device_registers.h"
#include "My_NVIC.h"
#include "My_Device.h"
#include "My_GPIO.h"
#include "My_PORT.h"


#define DEBOUNCE_DELAY	20

volatile uint8_t button_press_count = 0;
volatile uint8_t button_state = 0;
volatile uint8_t button_last_state = 0;
volatile uint8_t led_state = 0;

void init_button(){
	PORT_PIN_Config_t config = {
			.IRQC = PORT_IRQ_Disable,
			.MUX = GPIO,
			.PS = PULL_Disable,
	};
	My_PORT_Init(BUTTON_SW2_PORT, BUTTON_SW2_PIN, &config);
	My_GPIO_Init(BUTTON_SW2_GPIO, BUTTON_SW2_PIN, GPIO_INPUT);
}

void init_led(){
	PORT_PIN_Config_t config = {
			.IRQC = PORT_IRQ_Disable,
			.MUX = GPIO,
			.PS = PULL_Disable,
	};
	My_PORT_Init(LED_RED_PORT, LED_RED_PIN, &config);
	My_GPIO_Init(LED_RED_GPIO, LED_RED_PIN, GPIO_OUTPUT);
}

//void init_pin() {
//	/* Open clock gate for PORTD and PORTC */
//	PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;
//	PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC_MASK;
//
//	/* Configuration LED: MUX - GPIO, GPIO - Out */
//	PORTD->PCR[LED_BLUE_PIN] &= ~PORT_PCR_MUX_MASK;
//	PORTD->PCR[LED_BLUE_PIN] |= PORT_PCR_MUX(1); // Alternative 1 - GPIO
//	PTD->PDDR |= GPIO_PDDR_PDD(1 << LED_BLUE_PIN);
//	PTD->PSOR |= GPIO_PSOR_PTSO(1 << LED_BLUE_PIN); // Turn off led
//
//	PORTD->PCR[LED_GREEN_PIN] &= ~PORT_PCR_MUX_MASK;
//	PORTD->PCR[LED_GREEN_PIN] |= PORT_PCR_MUX(1); // Alternative 1 - GPIO
//	PTD->PDDR |= GPIO_PDDR_PDD(1 << LED_GREEN_PIN);
//	PTD->PSOR |= GPIO_PSOR_PTSO(1 << LED_GREEN_PIN);
//
//	PORTD->PCR[LED_RED_PIN] &= ~PORT_PCR_MUX_MASK;
//	PORTD->PCR[LED_RED_PIN] |= PORT_PCR_MUX(1); // Alternative 1 - GPIO
//	PTD->PDDR |= GPIO_PDDR_PDD(1 << LED_RED_PIN);
//	PTD->PSOR |= GPIO_PSOR_PTSO(1 << LED_RED_PIN);
//
//}

void LPIT0_Ch0_IRQHandler(void) {
	if (LPIT0->MSR & LPIT_MSR_TIF0_MASK) {
		/* Clear timer interrupt flag */
		LPIT0->MSR = LPIT_MSR_TIF0_MASK;

		/* Toggle LED or perform desired action */
		PTD->PTOR |= GPIO_PTOR_PTTO(1 << LED_RED_PIN);
	}
}

void Init_LPIT() {
    PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC(0u);

    SCG->FIRCCSR &= ~SCG_FIRCCSR_FIRCEN_MASK;    /* Disable Fast IRC */
	SCG->FIRCDIV |= SCG_FIRCDIV_FIRCDIV2(1);	 /* Divider = 1 */
	SCG->FIRCCSR |= SCG_FIRCCSR_FIRCEN(1);       /* Enable Fast IRC */

    PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_PCS(0b011);
    PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC(1u);

    LPIT0->MCR |= (1 << LPIT_MCR_M_CEN_SHIFT) | (1u << LPIT_MCR_DBG_EN_SHIFT);

    LPIT0->TMR[0].TCTRL |= LPIT_TMR_TCTRL_MODE(0u);
    LPIT0->TMR[0].TVAL = 47999u;
    LPIT0->MIER |= (1 << LPIT_MIER_TIE0_SHIFT);
    LPIT0->TMR[0].TCTRL |= (1u << LPIT_TMR_TCTRL_T_EN_SHIFT);

    S32_NVIC->ISER[LPIT0_Ch0_IRQn / 32] = (1 << LPIT0_Ch0_IRQn % 32);
}


void debounce_button() {
	uint8_t button_current_state = (PTC->PDIR & (1 << BUTTON_SW2_PIN)) ? 1 : 0;

	if (button_current_state != button_last_state) {
		button_press_count = 0;
	}
	else {
		if (button_press_count < DEBOUNCE_DELAY) {
			button_press_count++;
		}
		else {
			if (button_state != button_current_state) {
				button_state = button_current_state;
				if (button_state == 1) {
					led_state = 1;
				}
			}
		}
	}
	button_last_state = button_current_state;
}
int main() {
	init_button();
	init_led();

	// Supper Loop
	while(1) {
		debounce_button();
		if (led_state) {
			My_GPIO_Toggle(LED_RED_GPIO, LED_RED_PIN); //Turn toggle Led
			led_state = 0;
		}
	}
	return 1;
}
