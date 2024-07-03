#include "device_registers.h"

/*
 * RGB_BLUE 	PTD0
 * RGB_RED		PTD15
 * RGB_GREEN	PTD16
 *
 * SW2	BTN0	PTC12
 * SW3	BTN1	PTC13
 *
 * */

#define LED_BLUE_PIN	0
#define LED_RED_PIN		15
#define LED_GREEN_PIN	16

#define BUTTON_SW2_PIN	12
#define BUTTON_SW3_PIN	13

#define DEBOUNCE_DELAY	20

volatile uint8_t button_press_count = 0;
volatile uint8_t button_state = 0;
volatile uint8_t button_last_state = 0;
volatile uint8_t led_state = 0;

void init_pin() {
	/* Open clock gate for PORTD and PORTC */
	PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;
	PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC_MASK;

	/* Configuration LED: MUX - GPIO, GPIO - Out */
	PORTD->PCR[LED_BLUE_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[LED_BLUE_PIN] |= PORT_PCR_MUX(1); // Alternative 1 - GPIO
	PTD->PDDR |= GPIO_PDDR_PDD(1 << LED_BLUE_PIN);
	PTD->PSOR |= GPIO_PSOR_PTSO(1 << LED_BLUE_PIN); // Turn off led

	PORTD->PCR[LED_GREEN_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[LED_GREEN_PIN] |= PORT_PCR_MUX(1); // Alternative 1 - GPIO
	PTD->PDDR |= GPIO_PDDR_PDD(1 << LED_GREEN_PIN);
	PTD->PSOR |= GPIO_PSOR_PTSO(1 << LED_GREEN_PIN);

	PORTD->PCR[LED_RED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[LED_RED_PIN] |= PORT_PCR_MUX(1); // Alternative 1 - GPIO
	PTD->PDDR |= GPIO_PDDR_PDD(1 << LED_RED_PIN);
	PTD->PSOR |= GPIO_PSOR_PTSO(1 << LED_RED_PIN);

	/* Configuration BUTTON SW2: MUX - GPIO, GPIO - Input */
	PORTC->PCR[BUTTON_SW2_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[BUTTON_SW2_PIN] |= PORT_PCR_MUX(1); // Alternative 1 - GPIO
	PTC->PDDR &= ~GPIO_PDDR_PDD(1 << BUTTON_SW2_PIN);

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
	init_pin();
	// Supper Loop
	while(1) {
		debounce_button();
		if (led_state) {
			PTD->PTOR |= GPIO_PTOR_PTTO(1 << LED_RED_PIN); //Turn toggle Led
			led_state = 0;
		}
	}
	return 1;
}
