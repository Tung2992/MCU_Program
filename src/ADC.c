/******************************************************************************
 * Includes
 ******************************************************************************/
#include "S32K144.h"
#include "system_S32K144.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/
#define LED_GREEN_PIN 16
#define LED_BLUE_PIN 0
#define LED_RED_PIN 15

/* Define switch pins */
#define SW2_PIN 12
#define SW3_PIN 13
/******************************************************************************
 * Global variables
 ******************************************************************************/
volatile uint32_t adcResult = 0;
volatile uint32_t dutyRed=0;
volatile uint32_t dutyBlue=0;
volatile uint32_t dutyGreen=0;
/******************************************************************************
 * Peripherals initialize
 ******************************************************************************/
void delay(volatile int count) {
    while(count--);
}

void PWM_SetDutyCycle() {
    int onTime1 = dutyGreen;        // Số chu kỳ bật
    int offTime1 = 100 - dutyGreen; // Số chu kỳ tắt
    for (int i = 0; i < onTime1; i++) {
        PTD->PSOR |= (1u << LED_GREEN_PIN);  // Bật đèn
        delay(100);             // Delay ngắn để giữ đèn bật
    }

    for (int i = 0; i < offTime1; i++) {
        PTD->PCOR |= (1u << LED_GREEN_PIN);  // Tắt đèn
        delay(100);             // Delay ngắn để giữ đèn tắt
    }

    int onTime2 = dutyBlue;        // Số chu kỳ bật
    int offTime2 = 100 - dutyBlue; // Số chu kỳ tắt
    for (int i = 0; i < onTime2; i++) {
        PTD->PSOR |= (1u << LED_BLUE_PIN);  // Bật đèn
        delay(100);             // Delay ngắn để giữ đèn bật
    }

    for (int i = 0; i < offTime2; i++) {
        PTD->PCOR |= (1u << LED_BLUE_PIN);  // Tắt đèn
        delay(100);             // Delay ngắn để giữ đèn tắt
    }

    int onTime3 = dutyRed;        // Số chu kỳ bật
    int offTime3 = 100 - dutyRed; // Số chu kỳ tắt
    for (int i = 0; i < onTime3; i++) {
        PTD->PSOR |= (1u << LED_RED_PIN);  // Bật đèn
        delay(100);             // Delay ngắn để giữ đèn bật
    }

    for (int i = 0; i < offTime3; i++) {
        PTD->PCOR |= (1u << LED_RED_PIN);  // Tắt đèn
        delay(100);             // Delay ngắn để giữ đèn tắt
    }
}

void init_ADC()
{
	/* Disable ADC0 clock for config */
	PCC->PCCn[PCC_ADC0_INDEX] &= ~PCC_PCCn_CGC_MASK;
	/**
	  \Changing FIRCDIV should be done when FAST IRC is disabled to prevent
	    glitches to output divided clock
	*/
	SCG->FIRCCSR &= ~SCG_FIRCCSR_FIRCEN_MASK;    /* Disable Fast IRC */
	SCG->FIRCDIV |= SCG_FIRCDIV_FIRCDIV2(1);	 /* Divider = 1 */
	SCG->FIRCCSR |= SCG_FIRCCSR_FIRCEN(1);       /* Enable Fast IRC */
	/* ADC0 clock Option = Fast IRC */
	PCC->PCCn[PCC_ADC0_INDEX] |= PCC_PCCn_PCS(3);
	/* Enable ADC clock */
	PCC->PCCn[PCC_ADC0_INDEX] |= PCC_PCCn_CGC(1);


	/* Conversion mode: 12bits */
	ADC0->CFG1 |= ADC_CFG1_MODE(1);
	/* Input clock: Alternate clock 1, Divide ratio = 1 */
	ADC0->CFG1 &= ~ (ADC_CFG1_ADICLK_MASK | ADC_CFG1_ADIV_MASK);
	/* Conversion trigger: software */
	ADC0->SC2 &= ~ADC_SC2_ADTRG_MASK;
	/* Voltage reference selection: default VREFH, VREFL */
	ADC0->SC2 &= ~ADC_SC2_REFSEL_MASK;
	/* One conversion mode */
	ADC0->SC3 &= ~ADC_SC3_ADCO_MASK;
	/* Enable interrupt */
	ADC0->SC1[0] |= ADC_SC1_AIEN(1);
	/* Clear ADCH field */
	ADC0->SC1[0] &= ~ADC_SC1_ADCH_MASK;
	/* Input channel 12: Potentiometer, start conversion */
	ADC0->SC1[0] |= ADC_SC1_ADCH(12);
	/* Enable ADC0 IRQ */
	S32_NVIC->ISER[1] = (uint32_t)(1UL << (((uint32_t)ADC0_IRQn-32) & 0x1FUL));
}


/******************************************************************************
 * Functions
 ******************************************************************************/

void Init_Switch_Interrupt(uint32_t pin)
{
    /* Enable clock for PORTC */
    PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC_MASK;

    /* Set MUX bit in PCR for specified pin to GPIO mode */
    PORTC->PCR[pin] &= ~PORT_PCR_MUX_MASK;
    PORTC->PCR[pin] |= PORT_PCR_MUX(1);

    /* Set port data direction bit for PTC as input */
    PTC->PDDR &= ~(1u << pin);

    /* Configure the pin for falling edge interrupt */
    PORTC->PCR[pin] |= PORT_PCR_IRQC(0xA); // Interrupt on falling edge

    /* Enable interrupt in NVIC */
    S32_NVIC->ISER[PORTC_IRQn / 32] = (1 << PORTC_IRQn % 32);
}

/* Interrupt Service Routine for PORTC */
void PORTC_IRQHandler(void)
{
    /* Check if the interrupt is for SW2_PIN */
    if (PORTC->ISFR & (1u << SW2_PIN)) {
        /* Clear the interrupt flag */
        PORTC->ISFR = (1u << SW2_PIN);
        if(dutyRed>=100) dutyRed=0;
        dutyRed+=10;
    }
    /* Check if the interrupt is for SW2_PIN */
    if (PORTC->ISFR & (1u << SW3_PIN)) {
        /* Clear the interrupt flag */
        PORTC->ISFR = (1u << SW3_PIN);
        if(dutyBlue>=100) dutyBlue=0;
        dutyBlue+=10;
    }
}

uint8_t Init_GPIO(uint8_t pinNumber)
{
    /*  Code your solution here */
    // Enable clock for PORTD
	PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;

    // Set MUX bit in Pin Control Register (PCR)
	PORTD->PCR[pinNumber] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[pinNumber] |= PORT_PCR_MUX(1);

    // Set PDD bit to configure as output
	PTD->PDDR |= (1u << pinNumber);
    return 0;
}

/******************************************************************************
 * IRQ Handler
 ******************************************************************************/
void ADC0_IRQHandler(void)
{
	adcResult = ADC0->R[0];
	ADC0->SC1[0] |= ADC_SC1_ADCH(12); // Start conversion on specified channel
}



/******************************************************************************
 * Main
 ******************************************************************************/
int main(void)
{
	Init_GPIO(LED_GREEN_PIN);
	Init_GPIO(LED_BLUE_PIN);
	Init_GPIO(LED_RED_PIN);
	init_ADC();
	Init_Switch_Interrupt(SW2_PIN);
	Init_Switch_Interrupt(SW3_PIN);
	while (1)
	{
    	dutyGreen=adcResult*100/4095;
        PWM_SetDutyCycle();
	}

    return 0;
}

/******************************************************************************
 * EOF
 ******************************************************************************/
