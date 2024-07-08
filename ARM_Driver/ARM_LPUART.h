#ifndef __ARM_UART__
#define __ARM_UART__

#define TIME_OUT			10000000;

typedef enum
{
	UART_7_BITS_PER_CHAR, /*!< 7-bit data characters */
    UART_8_BITS_PER_CHAR, /*!< 8-bit data characters */
    UART_9_BITS_PER_CHAR, /*!< 9-bit data characters */
    UART_10_BITS_PER_CHAR  /*!< 10-bit data characters */
} uart_bit_count_per_char_t;


typedef enum
{
    UART_PARITY_DISABLED, 	/*!< parity disabled */
    UART_PARITY_EVEN, 		/*!< parity enabled, type even, bit setting: PE|PT = 10 */
    UART_PARITY_ODD  		/*!< parity enabled, type odd,  bit setting: PE|PT = 11 */
} uart_parity_mode_t;

typedef enum
{
    UART_ONE_STOP_BIT, 		/*!< one stop bit */
    UART_TWO_STOP_BIT  		/*!< two stop bits */
} uart_stop_bit_count_t;

typedef enum
{
	UART_MSB,
	UART_LSB
} uart_msb_lsb_t;

typedef enum
{
    UART_USING_DMA,  		  /*!< The driver will use DMA to perform UART transfer */
    UART_USING_INTERRUPTS     /*!< The driver will use interrupts to perform UART transfer */
} uart_transfer_type_t;


typedef enum
{
	UART_SOSCDIV2_CLK,
	UART_SIRCDIV2_CLK,
	UART_FIRCDIV2_CLK,
	UART_SPLLDIV2_CLK
} uart_source_clock_t;

typedef struct
{
    uint32_t baudRate;
    uart_parity_mode_t parityMode;
    uart_stop_bit_count_t stopBitCount;
    uart_bit_count_per_char_t bitCountPerChar;
    uart_transfer_type_t transferType;
    uart_msb_lsb_t selectMSBLSB;
} uart_config_t;

uint8_t UART_SetParityMode(LPUART_Type* base, uart_parity_mode_t parityMode);
uint8_t UART_SetStopBit(LPUART_Type* base, uart_stop_bit_count_t stopBit);
uint8_t UART_SetBitCountPerChar(LPUART_Type* base, uart_bit_count_per_char_t bitCountPerChar);
void UART_Set_MSB_LSB(LPUART_Type* base, uart_msb_lsb_t select);
uint8_t UART_Set_Baudrate(LPUART_Type* base, uint32_t desiredBaudRate, uint32_t sourceClock);
uint8_t UART_SelectSourceClock(LPUART_Type* base, uart_source_clock_t source);
uint8_t UART_Init(LPUART_Type* base, uart_config_t * uartConfig);
uint8_t UART_Deinit(LPUART_Type* base);

uint8_t UART_ReceiveStringBlocking(LPUART_Type* base, uint8_t *rxBuff, uint32_t rxSize);
uint8_t UART_SendStringBlocking(LPUART_Type* base, const uint8_t *txBuff, uint32_t txSize);
char UART_ReceiveCharBlocking(LPUART_Type* base);
void UART_SendCharBlocking(LPUART_Type* base, char send);

uint8_t UART_ReceiveNonBlocking(LPUART_Type* base);

#endif
