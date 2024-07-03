#include <stdint.h>
#include <stdio.h>
#include "device_registers.h"
#include "ARM_LPUART.h"
#include "ARM_NVIC.h"



uint8_t UART_SetParityMode(LPUART_Type* base, uart_parity_mode_t parityMode)
{
	switch (parityMode) {
	case UART_PARITY_DISABLED:
		base->CTRL &= ~(LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK);
		base->CTRL &= ~(LPUART_CTRL_PE_MASK | LPUART_CTRL_PT_MASK); /* Clear PE and PT */
		break;
	case UART_PARITY_EVEN:
		base->CTRL &= ~(LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK);
		base->CTRL |= LPUART_CTRL_PE_MASK;	/* Enable Parity */
		base->CTRL &= ~LPUART_CTRL_PT_MASK;	/* PT = 0 -> Even */
		break;
	case UART_PARITY_ODD:
		base->CTRL &= ~(LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK);
		base->CTRL |= LPUART_CTRL_PE_MASK;	/* Enable Parity */
		base->CTRL |= LPUART_CTRL_PT_MASK;	/* PT = 1 -> Odd */
		break;
	default:
		return 1;
	}
	return 0;
}

uint8_t UART_SetStopBit(LPUART_Type* base, uart_stop_bit_count_t stopBit)
{
	switch (stopBit) {
	case UART_ONE_STOP_BIT:
		base->CTRL &= ~(LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK);
		base->BAUD &= ~LPUART_BAUD_SBNS_MASK; /* SBNS = 0 -> One */
		break;
	case UART_TWO_STOP_BIT:
		base->CTRL &= ~(LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK);
		base->BAUD |= LPUART_BAUD_SBNS_MASK; /* SBNS = 1 -> Two */
		break;
	default:
		return 1;
	}
    return 0;
}

uint8_t UART_SetBitCountPerChar(LPUART_Type* base, uart_bit_count_per_char_t bitCountPerChar)
{
	switch (bitCountPerChar) {
	case UART_7_BITS_PER_CHAR:
		base->CTRL &= ~(LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK); /* Disable Transmitter and Receiver */
		base->BAUD &= ~LPUART_BAUD_M10_MASK; /* 0b - Receiver and transmitter use 7-bit to 9-bit data characters */
		base->CTRL |= LPUART_CTRL_M7_MASK;	/* 1b - Receiver and transmitter use 7-bit data characters*/
		break;
	case UART_8_BITS_PER_CHAR:
		base->CTRL &= ~(LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK); /* Disable Transmitter and Receiver */
		base->BAUD &= ~LPUART_BAUD_M10_MASK; /* 0b - Receiver and transmitter use 7-bit to 9-bit data characters */
		base->CTRL &= ~LPUART_CTRL_M7_MASK; /* 0b - Receiver and transmitter use 8-bit to 10-bit data characters.*/
		base->CTRL &= ~LPUART_CTRL_M_MASK;
		break;
	case UART_9_BITS_PER_CHAR:
		base->CTRL &= ~(LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK); /* Disable Transmitter and Receiver */
		base->BAUD &= ~LPUART_BAUD_M10_MASK; /* 0b - Receiver and transmitter use 7-bit to 9-bit data characters */
		base->CTRL &= ~LPUART_CTRL_M7_MASK;
		base->CTRL |= LPUART_CTRL_M_MASK; /* 1b - Receiver and transmitter use 9-bit data characters */
		break;
	case UART_10_BITS_PER_CHAR:
		base->CTRL &= ~(LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK); /* Disable Transmitter and Receiver */
		base->BAUD |= LPUART_BAUD_M10_MASK; /* 1b - Receiver and transmitter use 10-bit data characters */
		break;
	default:
		return 1;
	}
	return 0;
}

uint8_t UART_Set_Baudrate(LPUART_Type* base, uint32_t desiredBaudRate)
{
	base->CTRL &= ~(LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK); /* Disable Transmitter and Receiver */
//	uint32_t clockFrequency = FAST_IRC;
//	uint32_t sbr = clockFrequency / (desiredBaudRate * (OSR_DEFAULT + 1)); /* OSR = 16 -> Default value */
//	base->BAUD &= ~LPUART_BAUD_OSR_MASK; /* 00000b - Writing 0 to this field results in an oversampling ratio of 16 */
//	base->BAUD &= ~LPUART_BAUD_SBR_MASK;
//	base->BAUD |= (sbr & LPUART_BAUD_SBR_MASK);

	base->BAUD &= ~(LPUART_BAUD_OSR_MASK | LPUART_BAUD_SBNS_MASK);
	base->BAUD |= LPUART_BAUD_OSR(15);
	base->BAUD |= LPUART_BAUD_SBR(52);

	return 0;
}

uint8_t UART_SelectSourceClock(LPUART_Type* base, uart_source_clock_t source) {
	if (base == LPUART0) {
		switch (source) {
		case UART_SOSCDIV2_CLK:
			break;
		case UART_SIRCDIV2_CLK:
			break;
		case UART_FIRCDIV2_CLK:
			PCC->PCCn[PCC_LPUART0_INDEX] &= ~PCC_PCCn_CGC_MASK;
			PCC->PCCn[PCC_LPUART0_INDEX] &= ~PCC_PCCn_PCS_MASK;
			PCC->PCCn[PCC_LPUART0_INDEX] |= PCC_PCCn_PCS(3); /* Peripheral Clock Source Select: FIRCDIV2_CLK */
			PCC->PCCn[PCC_LPUART0_INDEX] |= PCC_PCCn_CGC_MASK;
			break;
		case UART_SPLLDIV2_CLK:
			break;
		default:
			return 1;
		}
	}
	else if (base == LPUART1) {
		switch (source) {
		case UART_SOSCDIV2_CLK:
			break;
		case UART_SIRCDIV2_CLK:
			break;
		case UART_FIRCDIV2_CLK:
			PCC->PCCn[PCC_LPUART1_INDEX] &= ~PCC_PCCn_CGC_MASK;
			PCC->PCCn[PCC_LPUART1_INDEX] &= ~PCC_PCCn_PCS_MASK;
			PCC->PCCn[PCC_LPUART1_INDEX] |= PCC_PCCn_PCS(3); /* Peripheral Clock Source Select: FIRCDIV2_CLK */
			PCC->PCCn[PCC_LPUART1_INDEX] |= PCC_PCCn_CGC_MASK;
			break;
		case UART_SPLLDIV2_CLK:
			break;
		default:
			return 1;
		}
	}
	else if (base == LPUART2) {
		switch (source) {
		case UART_SOSCDIV2_CLK:
			break;
		case UART_SIRCDIV2_CLK:
			break;
		case UART_FIRCDIV2_CLK:
			PCC->PCCn[PCC_LPUART2_INDEX] &= ~PCC_PCCn_CGC_MASK;
			PCC->PCCn[PCC_LPUART2_INDEX] &= ~PCC_PCCn_PCS_MASK;
			PCC->PCCn[PCC_LPUART2_INDEX] |= PCC_PCCn_PCS(3); /* Peripheral Clock Source Select: FIRCDIV2_CLK */
			PCC->PCCn[PCC_LPUART2_INDEX] |= PCC_PCCn_CGC_MASK;
			break;
		case UART_SPLLDIV2_CLK:
			break;
		default:
			return 1;
		}
	}
	else {
		return 1;
	}
	return 0;
}

uint8_t UART_Init(LPUART_Type* base, uart_config_t * uartConfig)
{
	UART_SelectSourceClock(base, UART_FIRCDIV2_CLK);
	UART_Set_Baudrate(base, uartConfig->baudRate);
	UART_SetParityMode(base, uartConfig->parityMode);
	UART_SetStopBit(base, uartConfig->stopBitCount);
	UART_SetBitCountPerChar(base , uartConfig->bitCountPerChar);
	if (base == LPUART1) {
		PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC_MASK;
		PORTC->PCR[6] &= ~PORT_PCR_MUX_MASK;
		PORTC->PCR[7] &= ~PORT_PCR_MUX_MASK;
		PORTC->PCR[6] |= PORT_PCR_MUX(2);
		PORTC->PCR[7] |= PORT_PCR_MUX(2);
	}
	return 0;
}


uint8_t UART_Deinit(LPUART_Type* base) {
	return 0;
}

uint8_t UART_ReceiveStringBlocking(LPUART_Type* base, uint8_t *rxBuff, uint32_t rxSize)
{
	uint32_t i;
	uint32_t time;
	base->CTRL |= LPUART_CTRL_RE_MASK;
    for (i = 0; i < rxSize; i++)
	{
    	time = TIME_OUT;
		while (!((base->STAT & LPUART_STAT_RDRF_MASK) || (time == 0))) // Wait RDRF in STAT register set 1
		{
			time--;
		}
		if (time == 0) {
			return 1;
		}
		else {
			rxBuff[i] = base->DATA;
		}
	}
    return 0;
}

char UART_ReceiveCharBlocking(LPUART_Type* base) {
	char receive;
	receive = base->DATA;
	base->CTRL |= LPUART_CTRL_RE_MASK;
	while(!(base->STAT & LPUART_STAT_RDRF_MASK));
	receive = base->DATA;
	return receive;
}

uint8_t UART_SendStringBlocking(LPUART_Type* base, const uint8_t *txBuff, uint32_t txSize)
{
	uint32_t i;
	uint32_t time;
	base->CTRL |= LPUART_CTRL_TE_MASK;
    for (i = 0; i < txSize; i++)
    {
    	time = TIME_OUT;
        while (!((base->STAT & LPUART_STAT_TDRE_MASK) || (time == 0))) // Assuming TDRE is bit 23 in STAT register
        {
            time--;
        }
        if (time == 0) {
        	return 1;
        }
        else {
        	base->DATA = txBuff[i];
        }
    }
    return 0; // Return success
}

void UART_SendCharBlocking(LPUART_Type* base, char send)
{
	base->CTRL |= LPUART_CTRL_TE_MASK;
	while(!(base->STAT & LPUART_STAT_TDRE_MASK));
	base->DATA = send;
}


uint8_t UART_ReceiveNonBlocking(LPUART_Type* base) {
	if (base == LPUART1) {
		base->CTRL &= ~LPUART_CTRL_RE_MASK;
		base->CTRL |= LPUART_CTRL_RIE_MASK;	 /* Enable Receiver Interrupt */
		NVIC_EnableIRQ(LPUART1_RxTx_IRQn);
		base->CTRL |= LPUART_CTRL_RE_MASK;
	}
	else {
		return 1;
	}
	return 0;
}

void LPUART0_RxTx_IRQHandler(void) {
	while(1);
}

