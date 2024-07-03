//#ifndef __ARM_UART__
//#define __ARM_UART__
//
//#define 	ARM_USART_ERROR_MODE   (ARM_DRIVER_ERROR_SPECIFIC - 1)
//#define 	ARM_USART_ERROR_BAUDRATE   (ARM_DRIVER_ERROR_SPECIFIC - 2)
//#define 	ARM_USART_ERROR_DATA_BITS   (ARM_DRIVER_ERROR_SPECIFIC - 3)
//#define 	ARM_USART_ERROR_PARITY   (ARM_DRIVER_ERROR_SPECIFIC - 4)
//#define 	ARM_USART_ERROR_STOP_BITS   (ARM_DRIVER_ERROR_SPECIFIC - 5)
//#define 	ARM_USART_ERROR_FLOW_CONTROL   (ARM_DRIVER_ERROR_SPECIFIC - 6)
//#define 	ARM_USART_ERROR_CPOL   (ARM_DRIVER_ERROR_SPECIFIC - 7)
//#define 	ARM_USART_ERROR_CPHA   (ARM_DRIVER_ERROR_SPECIFIC - 8)
//
//
//#define 	ARM_USART_EVENT_SEND_COMPLETE   (1UL << 0)
//#define 	ARM_USART_EVENT_RECEIVE_COMPLETE   (1UL << 1)
//#define 	ARM_USART_EVENT_TRANSFER_COMPLETE   (1UL << 2)
//#define 	ARM_USART_EVENT_TX_COMPLETE   (1UL << 3)
//#define 	ARM_USART_EVENT_TX_UNDERFLOW   (1UL << 4)
//#define 	ARM_USART_EVENT_RX_OVERFLOW   (1UL << 5)
//#define 	ARM_USART_EVENT_RX_TIMEOUT   (1UL << 6)
//#define 	ARM_USART_EVENT_RX_BREAK   (1UL << 7)
//#define 	ARM_USART_EVENT_RX_FRAMING_ERROR   (1UL << 8)
//#define 	ARM_USART_EVENT_RX_PARITY_ERROR   (1UL << 9)
//#define 	ARM_USART_EVENT_CTS   (1UL << 10)
//#define 	ARM_USART_EVENT_DSR   (1UL << 11)
//#define 	ARM_USART_EVENT_DCD   (1UL << 12)
//#define 	ARM_USART_EVENT_RI   (1UL << 13)
//
//#define 	ARM_USART_MODE_ASYNCHRONOUS   (0x01UL << ARM_USART_CONTROL_Pos)
//#define 	ARM_USART_MODE_SYNCHRONOUS_MASTER   (0x02UL << ARM_USART_CONTROL_Pos)
//#define 	ARM_USART_MODE_SYNCHRONOUS_SLAVE   (0x03UL << ARM_USART_CONTROL_Pos)
//#define 	ARM_USART_MODE_SINGLE_WIRE   (0x04UL << ARM_USART_CONTROL_Pos)
//#define 	ARM_USART_MODE_IRDA   (0x05UL << ARM_USART_CONTROL_Pos)
//#define 	ARM_USART_MODE_SMART_CARD   (0x06UL << ARM_USART_CONTROL_Pos)
//
//#define 	ARM_USART_SET_DEFAULT_TX_VALUE   (0x10UL << ARM_USART_CONTROL_Pos)
//#define 	ARM_USART_SET_IRDA_PULSE   (0x11UL << ARM_USART_CONTROL_Pos)
//#define 	ARM_USART_SET_SMART_CARD_GUARD_TIME   (0x12UL << ARM_USART_CONTROL_Pos)
//#define 	ARM_USART_SET_SMART_CARD_CLOCK   (0x13UL << ARM_USART_CONTROL_Pos)
//#define 	ARM_USART_CONTROL_SMART_CARD_NACK   (0x14UL << ARM_USART_CONTROL_Pos)
//#define 	ARM_USART_CONTROL_TX   (0x15UL << ARM_USART_CONTROL_Pos)
//#define 	ARM_USART_CONTROL_RX   (0x16UL << ARM_USART_CONTROL_Pos)
//#define 	ARM_USART_CONTROL_BREAK   (0x17UL << ARM_USART_CONTROL_Pos)
//#define 	ARM_USART_ABORT_SEND   (0x18UL << ARM_USART_CONTROL_Pos)
//#define 	ARM_USART_ABORT_RECEIVE   (0x19UL << ARM_USART_CONTROL_Pos)
//#define 	ARM_USART_ABORT_TRANSFER   (0x1AUL << ARM_USART_CONTROL_Pos)
//
//#define 	ARM_USART_DATA_BITS_5   (5UL << ARM_USART_DATA_BITS_Pos)
//#define 	ARM_USART_DATA_BITS_6   (6UL << ARM_USART_DATA_BITS_Pos)
//#define 	ARM_USART_DATA_BITS_7   (7UL << ARM_USART_DATA_BITS_Pos)
//#define 	ARM_USART_DATA_BITS_8   (0UL << ARM_USART_DATA_BITS_Pos)
//#define 	ARM_USART_DATA_BITS_9   (1UL << ARM_USART_DATA_BITS_Pos)
//
//#define 	ARM_USART_PARITY_NONE   (0UL << ARM_USART_PARITY_Pos)
//#define 	ARM_USART_PARITY_EVEN   (1UL << ARM_USART_PARITY_Pos)
//#define 	ARM_USART_PARITY_ODD   (2UL << ARM_USART_PARITY_Pos)
//
//#define 	ARM_USART_STOP_BITS_1   (0UL << ARM_USART_STOP_BITS_Pos)
//#define 	ARM_USART_STOP_BITS_2   (1UL << ARM_USART_STOP_BITS_Pos)
//#define 	ARM_USART_STOP_BITS_1_5   (2UL << ARM_USART_STOP_BITS_Pos)
//#define 	ARM_USART_STOP_BITS_0_5   (3UL << ARM_USART_STOP_BITS_Pos)
//
//
//#define 	ARM_USART_FLOW_CONTROL_NONE   (0UL << ARM_USART_FLOW_CONTROL_Pos)
//#define 	ARM_USART_FLOW_CONTROL_RTS   (1UL << ARM_USART_FLOW_CONTROL_Pos)
//#define 	ARM_USART_FLOW_CONTROL_CTS   (2UL << ARM_USART_FLOW_CONTROL_Pos)
//#define 	ARM_USART_FLOW_CONTROL_RTS_CTS   (3UL << ARM_USART_FLOW_CONTROL_Pos)
//
//
//#define 	ARM_USART_CPOL0   (0UL << ARM_USART_CPOL_Pos)
//#define 	ARM_USART_CPOL1   (1UL << ARM_USART_CPOL_Pos)
//
//#define 	ARM_USART_CPHA0   (0UL << ARM_USART_CPHA_Pos)
//#define 	ARM_USART_CPHA1   (1UL << ARM_USART_CPHA_Pos)
//
//
//
//typedef struct ARM_USART_CAPABILITIES {
//	uint32_t	asynchronous: 1;
//	uint32_t	synchronous_master: 1;
//	uint32_t	synchronous_slave: 1;
//	uint32_t	single_wire: 1;
//	uint32_t	irda: 1;
//	uint32_t	smart_card: 1;
//	uint32_t	smart_card_clock: 1;
//	uint32_t	flow_control_rts: 1;
//	uint32_t	flow_control_cts: 1;
//	uint32_t	event_tx_complete: 1;
//	uint32_t	event_rx_timeout: 1;
//	uint32_t	rts: 1;
//	uint32_t	cts: 1;
//	uint32_t	dtr: 1;
//	uint32_t	dsr: 1;
//	uint32_t	dcd: 1;
//	uint32_t	ri: 1;
//	uint32_t	event_cts: 1;
//	uint32_t	event_dsr: 1;
//	uint32_t	event_dcd: 1;
//	uint32_t	event_ri: 1;
//	uint32_t	reserved: 11;
//
//} ARM_USART_CAPABILITIES;
//
//
//typedef struct ARM_USART_STATUS {
//	uint32_t	tx_busy: 1;
//	uint32_t	rx_busy: 1;
//	uint32_t	tx_underflow: 1;
//	uint32_t	rx_overflow: 1;
//	uint32_t	rx_break: 1;
//	uint32_t	rx_framing_error: 1;
//	uint32_t	rx_parity_error: 1;
//	uint32_t	reserved: 25;
//}ARM_USART_STATUS;
//
//typedef struct ARM_USART_MODEM_STATUS {
//	uint32_t	cts: 1;
//	uint32_t	dsr: 1;
//	uint32_t	dcd: 1;
//	uint32_t	ri: 1;
//	uint32_t	reserved: 28;
//}ARM_USART_MODEM_STATUS;
//
//
//typedef enum ARM_USART_MODEM_CONTROL {
//  ARM_USART_RTS_CLEAR ,
//  ARM_USART_RTS_SET ,
//  ARM_USART_DTR_CLEAR ,
//  ARM_USART_DTR_SET
//} ARM_USART_MODEM_CONTROL;
//
//
//typedef struct ARM_DRIVER_USART {
//	ARM_DRIVER_VERSION(* 	GetVersion )(void);
//	ARM_USART_CAPABILITIES(* 	GetCapabilities )(void);
//	int32_t(* 	Initialize )(ARM_USART_SignalEvent_t cb_event);
//	int32_t(* 	Uninitialize )(void);
//	int32_t(* 	PowerControl )(ARM_POWER_STATE state);
//	int32_t(* 	Send )(const void *data, uint32_t num);
//	int32_t(* 	Receive )(void *data, uint32_t num);
//	int32_t(* 	Transfer )(const void *data_out, void *data_in, uint32_t num);
//	uint32_t(* 	GetTxCount )(void);
//	uint32_t(* 	GetRxCount )(void);
//	int32_t(* 	Control )(uint32_t control, uint32_t arg);
//	ARM_USART_STATUS(* 	GetStatus )(void);
//	int32_t(* 	SetModemControl )(ARM_USART_MODEM_CONTROL control);
//	ARM_USART_MODEM_STATUS(* 	GetModemStatus )(void);
//} ARM_DRIVER_USART;
//
//
//
//ARM_DRIVER_VERSION 	ARM_USART_GetVersion (void);
//ARM_USART_CAPABILITIES 	ARM_USART_GetCapabilities (void);
//int32_t 	ARM_USART_Initialize (ARM_USART_SignalEvent_t cb_event);
//int32_t 	ARM_USART_Uninitialize (void);
//int32_t 	ARM_USART_PowerControl (ARM_POWER_STATE state);
//int32_t 	ARM_USART_Send (const void *data, uint32_t num);
//int32_t 	ARM_USART_Receive (void *data, uint32_t num);
//int32_t 	ARM_USART_Transfer (const void *data_out, void *data_in, uint32_t num);
//uint32_t 	ARM_USART_GetTxCount (void);
//uint32_t 	ARM_USART_GetRxCount (void);
//int32_t 	ARM_USART_Control (uint32_t control, uint32_t arg);
//ARM_USART_STATUS 	ARM_USART_GetStatus (void);
//int32_t 	ARM_USART_SetModemControl (ARM_USART_MODEM_CONTROL control);
//ARM_USART_MODEM_STATUS 	ARM_USART_GetModemStatus (void);
//void 	ARM_USART_SignalEvent (uint32_t event);
//
//
//#endif
