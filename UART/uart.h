//prevents double inclusion
#ifndef UART_H_
#define UART_H_
#include "tm4c123gh6pm.h"
#include "data_types.h"
#include "gpio.h"
#pragma anon_unions

/*--------------------------------------------------------------------------------------------------------------------------------
[Union Name]: UART_controlRegister_Type.
[Description]: The Union is responsible for maintainnig informations about bits of UART control register.
--------------------------------------------------------------------------------------------------------------------------------*/
typedef	union{
	uint16_t UART_control;
	struct{
		uint16_t :1;
		/* UART SIR (1 enable, 0 Normal use) 
				The IrDA SIR block is enabled, and the UART will transmit and receive data using SIR protocol
			*/
		uint16_t UART_sirEnBit:1;
		/* UART SIR Low-Power Mode 
				0 -> Low-level bits are transmitted as an active High pulse with a
					width of 3/16th of the bit period.
				1 -> The UART operates in SIR Low-Power mode. Low-level bits
					are transmitted with a pulse width which is 3 times the period 
					of the IrLPBaud16 input signal, regardless of the selected bit rate.
		*/
		uint16_t UART_sirLPEnBit:1;
		/* UART ISO 7816 Smart Card Support (1 enable, 0 Normal mode)
				The application must ensure that it sets 8-bit word length (WLEN set to0x3)
				and even parity (PEN set to 1, EPS set to 1, SPS set to 0) in UARTLCRH when using ISO 7816 mode.
				In this mode, the value of the STP2 bit in UARTLCRH is ignored and
				the number of stop bits is forced to 2. Note that the UART does not
				support automatic retransmission on parity errors. If a parity error is
				detected on transmission, all further transmit operations are aborted
				and software must handle retransmission of the affected byte or message.
		*/
		uint16_t UART_smartBit:1;
		uint16_t UART_eotBit:1;
		/* UART High-Speed Enable
				0-> The UART is clocked using the system clock divided by 16.
				1-> The UART is clocked using the system clock divided by 8.
				The state of this bit has no effect on clock generation in ISO 7816 smart card mode (the SMART bit is set).
		*/
		uint16_t UART_hseBit:1;
		uint16_t :1;
		// UART Loop Back Enable (1 tx is conected to rx, 0 Normal mode)
		uint16_t UART_lpeBit:1;
		// UART Transmit Enable (1 enable tx, 0 disable tx)
		uint16_t UART_txeBit:1;
		// UART Receive Enable (1 enable rx, 0 disable rx)
		uint16_t UART_rxeBit:1;
		uint16_t :6;
	};
}UART_controlRegister_Type;
/*--------------------------------------------------------------------------------------------------------------------------------
[Union Name]: UART_lineControl_Type.
[Description]: The Union is responsible for maintainnig informations about some configuration bits of UART line control register.
--------------------------------------------------------------------------------------------------------------------------------*/
typedef	union{
		uint8_t UART_lineControl;
	struct{
		/* UART send break (1 enable break, 0 Normal use)
				A Low level is continually output on the UnTx signal, after
				completing transmission of the current character. For the proper
				execution of the break command, software must set this bit for
				at least two frames (character periods).
		*/
		uint8_t UART_brkBit:1;
		// UART Parity enable (1 enable, 0 Normal use)
		uint8_t UART_penBit:1;
		// UART Even parity select (1 Even parity, 0 Odd parity)
		uint8_t UART_epsBit:1;
		// UART stop bit select (1 two stop bits, 0 one stop bit)
		uint8_t UART_stp2Bit:1;
		uint8_t :1;
		// UART word legth (0x0 5-bits, 0x1 6-bits, 0x2 7-bits, 0x3 8-bits
		uint8_t UART_wlenBit:2;
		// UART sticky parity select ()
		uint8_t UART_spsBit:1;
	};
}UART_lineControlRegister_Type;
/*--------------------------------------------------------------------------------------------------------------------------------
[Structre Name]: UART_registerType.
[Description]: The Structre is responsible for maintainnig informations about UART Baud rate, clock source,
								Baud Clock Source, and an object of UART_controlRegister_Type and UART_lineControlRegister_Type unions.
--------------------------------------------------------------------------------------------------------------------------------*/
typedef struct{
	uint32_t baudRate;
	// clk source value
	uint32_t clkSource;
	UART_controlRegister_Type con;
	UART_lineControlRegister_Type lineCon;
	/* UART Baud Clock Source
			0x0 System clock (based on clock source and divisor factor), 0x5 PIOSC (Percision Internal Oscillator).
	*/
	uint8_t UART_clkConfiguration;
} UART_registerType;

#ifdef UART_2_
/*--------------------------------------------------------------------------------------------------------------------------------
[Function Name]: UART2_init.
[Description]: This function is responsible for initiating UART; adjasts the Buad rate, configuration, and enables UART.
[Args]:
[In]	config:
					This is a pointer to structure (UART_registerType) that contains control registers of UART as bit fields, so you
					can adjust each bit feild to set the required configuration.
[Returns]	No return.
--------------------------------------------------------------------------------------------------------------------------------*/
void UART2_init(UART_registerType *config);
/*--------------------------------------------------------------------------------------------------------------------------------
[Function Name]: UART2_sendReceiveDataSync.
[Description]: This function is responsible for sending and receiving data bytes Synchronous.
[Args]:
[In]	tx: 
				This is a pointer to array of bytes, string, or a pointer to a single byte that will be sent.
			rx:
				This is a pointer to array of bytes, string, or a pointer to a single byte that will be received.
			txSize:
				This variable contains the size of the transimiting array.
				If the txSize=0 this will mean that a string will be transmitted.
			rxSize:
				This variable contains the size of the receiving array.
				If the rxSize=0 this will mean that a string will be received.
[Returns]	No return.
--------------------------------------------------------------------------------------------------------------------------------*/
void UART2_sendReceiveDataSync(uint8_t *tx, uint16_t *rx, uint16_t txSize, uint16_t rxSize);
/*--------------------------------------------------------------------------------------------------------------------------------
[Function Name]: UART2_sendReceiveDataASync.
[Description]: This function is responsible for sending and receiving data bytes Asynchronous.
[Args]:
[In]	tx: 
				This is a pointer to array of bytes, string, or a pointer to a single byte that will be sent.
			rx:
				This is a pointer to array of bytes, string, or a pointer to a single byte that will be received.
			txSize:
				This variable contains the size of the transimiting array.
				If the txSize=0 this will mean that a string will be transmitted.
			rxSize:
				This variable contains the size of the receiving array.
				If the rxSize=0 this will mean that a string will be received.
			priority:
				This variable contains the priority of the UART interrupt (from 0[highest] to 7[lowest]).
			txSemaphore:
				This is a pointer to a variable that will contain the Acknowledgment of completing the transmission.
			txSemaphore:
				This is a pointer to a variable that will contain the Acknowledgment of completing the Receiption.
[Returns]	No return.
--------------------------------------------------------------------------------------------------------------------------------*/
void UART2_sendReceiveDataASync(uint8_t *tx, uint16_t *rx, uint16_t txSize, uint16_t rxSize, uint8_t priority, uint8_t *txSemaphore, uint8_t *rxSemaphore);
/*--------------------------------------------------------------------------------------------------------------------------------
[Function Name]: UART2_setErrorsCallBackFunction.
[Description]: This function is responsible for enabling interrupt and setting the callback functions for the UART error flags.
[Args]:
[In]	overrunError: 
				This is a pointer to a function that will be called back when overrun error happens.
			parityError:
				This is a pointer to a function that will be called back when parity error happens.
			framingError:
				This is a pointer to a function that will be called back when framing error happens.
			breakError:
				This is a pointer to a function that will be called back when break error happens.
[Returns]	No return.
--------------------------------------------------------------------------------------------------------------------------------*/
void UART2_setErrorsCallBackFunction(void (*overrunError)(void), void (*parityError)(void), void (*framingError)(void), void (*breakError)(void));
#endif
#ifdef UART_5_
void UART5_init(UART_registerType *config);
void UART2_sendReceiveDataSync(uint8_t *tx, uint16_t *rx, uint16_t txSize, uint16_t rxSize);
void UART5_sendReceiveDataASync(uint8_t *tx, uint16_t *rx, uint16_t txSize, uint16_t rxSize, uint8_t priority, uint8_t *txSemaphore, uint8_t *rxSemaphore);
void UART2_setErrorsCallBackFunction(void (*overrunError)(void), void (*parityError)(void), void (*framingError)(void), void (*breakError)(void));
#endif
#endif
