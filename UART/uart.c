/*
 * uart.c
 *  	Created on: 13/7/2019
 *      Author: Amr Halim
 */
#include "uart.h"

#define UART_1_8_FIFO 2
#define UART_2_8_FIFO 4
#define UART_4_8_FIFO 8
#define UART_6_8_FIFO 12
#define UART_7_8_FIFO 14
#define UART_FIFO_MAX 16
#define UART0_MODULE 1
#define UART1_MODULE 2
#define UART2_MODULE 4
#define UART3_MODULE 8
#define UART4_MODULE 16
#define UART5_MODULE 32
#define UART6_MODULE 64
#define UART7_MODULE 128
/*--------------------------------------------------------------------------------------------------------------------------------
[Macro Name]: UART_moduleEnable.
[Description]: This macro is responsible for initiating UART module clock.
[Args]: 
[In]		module:
					this variable selects which UART module to enable.
[Returns]	No return.
--------------------------------------------------------------------------------------------------------------------------------*/
#define UART_moduleEnable(module) (SYSCTL_RCGCUART_R |= module)

volatile static uint8_t *UART2_txBuff;
volatile static uint8_t *UART2_rxBuff;
static uint16_t UART2_txBuffSize;
static uint16_t UART2_rxBuffSize;
volatile static uint8_t *UART2_txSemaphore;
volatile static uint8_t *UART2_rxSemaphore;
static void (*UART2_overrunErrorCallBackFunction)(void);
static void (*UART2_breakErrorCallBackFunction)(void);
static void (*UART2_parityErrorCallBackFunction)(void);
static void (*UART2_framingErrorCallBackFunction)(void);

volatile static uint8_t *UART5_txBuff;
volatile static uint8_t *UART5_rxBuff;
static uint16_t UART5_txBuffSize;
static uint16_t UART5_rxBuffSize;
volatile static uint8_t *UART5_txSemaphore;
volatile static uint8_t *UART5_rxSemaphore;
static void (*UART5_overrunErrorCallBackFunction)(void);
static void (*UART5_breakErrorCallBackFunction)(void);
static void (*UART5_parityErrorCallBackFunction)(void);
static void (*UART5_framingErrorCallBackFunction)(void);

void UART2_init(UART_registerType *config){
	uint8_t clkDiv;
	uint32_t fraction;
	if(config->con.UART_hseBit){
		clkDiv=8;
	}
	else{
		clkDiv=16;
	}
	//Enable the UART module using the RCGCUART register
	UART_moduleEnable(UART2_MODULE);															
	//Enable the clock to the appropriate GPIO module
	PORTx_ActivateClock(PORTD);	
	PORTD_UnlockPinPD7();
	//Set the GPIO AFSEL bits for the appropriate pins
	PORTD_DisableAnakogModeSelect(PIN6|PIN7);
	PORTD_EnableDigital(PIN6|PIN7);
	PORTD_EnableAlternateFunctionSelect(PIN6|PIN7);		
	//Configure the PMCn fields in the GPIOPCTL register to assign the SSI signals
	PORTD_PortControlFuction(GPIO_PCTL_PD6_U2RX);
	PORTD_PortControlFuction(GPIO_PCTL_PD7_U2TX);	
	//disable UART while comfiguration
	UART2_CTL_R = 1;										
	//setup baud rate registers
	UART2_IBRD_R = config->clkSource/(clkDiv*config->baudRate);
	fraction = (uint64_t)config->clkSource*100/(clkDiv*config->baudRate);
	fraction = (fraction-(UART2_IBRD_R*100))*64 + 50;
	UART2_FBRD_R = (fraction)/100;
	UART2_LCRH_R = config->lineCon.UART_lineControl;
	UART2_CTL_R = config->con.UART_control;
	//select clk source sysclk or PIOSC
	UART2_CC_R = config->UART_clkConfiguration;
	//enable UART
	UART2_CTL_R |= UART_CTL_UARTEN;
}
void UART2_sendReceiveDataSync(uint8_t *tx, uint8_t *rx, uint16_t txSize, uint16_t rxSize){
	uint16_t i=0, j=0;
	UART2_LCRH_R |= UART_LCRH_FEN;
	do{
		//check if there is data to transfer.
		if((tx)&&((i<txSize)||(txSize==0))){
			while(UART2_FR_R&UART_FR_TXFF);
			UART2_DR_R = tx[i++];
		}
		//check if there is data to receive.
		if((rx)&&(j<rxSize)){
			while(UART2_FR_R&UART_FR_RXFE);
			rx[j++] = UART2_DR_R;
		}
	}while(((tx[i]!='\0')&&(tx)&&((i<txSize)||(txSize==0)))||((rx[j]!='\0')&&(rx)&&(j<rxSize)));
}
void UART2_sendReceiveDataASync(uint8_t *tx, uint8_t *rx, uint16_t txSize, uint16_t rxSize, uint8_t priority, uint8_t *txSemaphore, uint8_t *rxSemaphore){
	uint8_t i=0, numberOfBytesToSend=0;
	uint8_t fifoLevels[4]={UART_1_8_FIFO, UART_2_8_FIFO, UART_4_8_FIFO, UART_6_8_FIFO};
	UART2_IFLS_R=0;
	if(tx||rx){
		//if there is a data to send
		if(tx){
			UART2_txSemaphore=txSemaphore;
			//send single byte
			if(txSize==1){
				UART2_DR_R = *tx;
			}
			//for multi byte to send
			else{
				//enable fifo
				UART2_LCRH_R |= UART_LCRH_FEN;
				//if tx buffer size>16 send 14 data bytes
				//if array size=0 -> the array represents a string send data untill you find '\0' or max data sent is 14 bytes(7/8 of fifo)
				if((txSize>UART_FIFO_MAX)||(txSize==0)){
					numberOfBytesToSend=UART_7_8_FIFO;
				}
				//if 0<size<= max fifo size(16) send all bytes once in fifo and don't enable interrupt 
				else{
					numberOfBytesToSend=txSize;
				}
				//send data bytes from array
				do{
					UART2_DR_R = tx[i];
				}while((tx[i++]!='\0')&&(i<numberOfBytesToSend));
			}
			//if tx buffer size>16 or last byte sent is != '\0'
			//enable tx interrupt to interrupt when fifo level is <= 2 data bytes
			if((txSize>UART_FIFO_MAX)||((txSize==0)&&(tx[(i-1)]!='\0'))){
				UART2_txBuff=tx;
				UART2_txBuffSize=txSize;
				UART2_IFLS_R &= ~UART_IFLS_TX1_8;
				UART2_IFLS_R |= UART_IFLS_TX1_8;
			}
			//enable end of transimission interrupt to signal tx semaphore
			//in case of data bytes <= 16
			else{
				UART2_CTL_R |= UART_CTL_EOT;
			}
			//Clear Interrupt flag
			UART2_ICR_R |= UART_ICR_TXIC;
			//Enable UART2 tx interrupt
			UART2_IM_R |= UART_IM_TXIM;
		}
		//if there is a data to receive
		if(rx){
			i=0;
			UART2_rxBuff=rx;
			UART2_rxBuffSize=rxSize;
			UART2_rxSemaphore=rxSemaphore;
			//if one byte is about to received and one or no bytes are transimitted 
			//disable fifo to interrupt when single byte is received
			if((rxSize==1)&&((!tx)||(txSize==1))){
				UART2_LCRH_R &= ~UART_LCRH_FEN;
			}
			else{
				//more than one byte enable fifo
				UART2_LCRH_R |= UART_LCRH_FEN;
				if(rxSize<14){
					//if size = one of fifo level 2, 4, 8, 12 adjust to interrup on that level
					//else adjust interrupt on fifo level 14 and receive that data on receiver timeout interrupt
					//The receive timeout interrupt is asserted when the receive FIFO is not empty, and no further data is received
					//over a 32-bit period when the HSE bit is clear or over a 64-bit period when the HSE bit is set.
					while((rxSize!=fifoLevels[i])&&(i<4)){
						i++;
					}
				}
				//if more than 14 bytes calc. number of interrupt times for each full 14 byte received
				//remaining bytes will be received in receiver timeout interrupt
				else{
					//to select 7/8 of fifo
					i=4;
				}
				if(i==4){
					//Clear Interrupt flag
					UART2_ICR_R |= UART_ICR_RTIC;
					//Enable UART5 rx timeout interrupt
					UART2_IM_R |= UART_IM_RTIM;
				}
			}
			//fifo level select
			UART2_IFLS_R |= i<<3;
			//Clear Interrupt flag
			UART2_ICR_R |= UART_ICR_RXIC;
			//Enable UART2 rx interrupt
			UART2_IM_R |= UART_IM_RXIM;
		}
		//Set UART2 Interrupt priority
		NVIC_PRI8_R &= ~(priority<<13);
		NVIC_PRI8_R |= (priority<<13);
		//Enable UART2 in NVIC
		NVIC_EN1_R |= (1<<1);
	}	
}
void UART2_setErrorsCallBackFunction(void (*overrunError)(void), void (*parityError)(void), void (*framingError)(void), void (*breakError)(void)){
	if(overrunError){
		UART2_IM_R |= UART_IM_OEIM;
		UART2_overrunErrorCallBackFunction = overrunError;
	}
	if(parityError){
		UART2_IM_R |= UART_IM_PEIM;
		UART2_parityErrorCallBackFunction = parityError;
	}
	if(framingError){
		UART2_IM_R |= UART_IM_FEIM;
		UART2_framingErrorCallBackFunction = framingError;
	}
	if(breakError){
		UART2_IM_R |= UART_IM_BEIM;
		UART2_breakErrorCallBackFunction = breakError;
	}
}

void UART2_Handler(void){
	static uint16_t i=UART_7_8_FIFO, j=0;
	//for transmit isr
	if(UART2_MIS_R&UART_MIS_TXMIS){
		//if tx interrupt is generated because of tx buffer size>16
		if(!(UART2_CTL_R&UART_CTL_EOT)){
			do{
				//send bytes till find '\0' or i=tx buffer size or send maximum of 14 bytes at a time
				UART2_DR_R = UART2_txBuff[i];
			}while((UART2_txBuff[i++]!='\0')&&((i<UART2_txBuffSize)||(UART2_txBuffSize==0))&&(i%UART_7_8_FIFO));
			//ack tx interrupt
			UART2_ICR_R |= UART_ICR_TXIC;
			//if last byte sent is '\0' or total data sent=tx buffer size
			//enable end of transimission to signal tx semaphore
			if((UART2_txBuff[(i-1)]=='\0')||(i==UART2_txBuffSize)){
				UART2_CTL_R |= UART_CTL_EOT;
			}
		}
		//end of transimission to signal tx semaphore and disable interrupt
		else{
			//clear tx array index for the next array to be sent
			i=0;
			//disable UART5 end of transimission
			UART2_CTL_R &= ~UART_CTL_EOT;
			//disable UART5 tx interrupt
			UART2_IM_R &= ~UART_IM_TXIM;
			*UART2_txSemaphore=1;
		}
	}

	//for receive isr
	if((UART2_MIS_R&UART_MIS_RXMIS)||(UART2_MIS_R&UART_MIS_RTMIS)){
		if(UART2_MIS_R&UART_MIS_RXMIS){
			//receive 14 bytes max or untill '\0' is received
			do{
				UART2_rxBuff[j]=UART2_DR_R;
			}while((UART2_rxBuff[j++]!='\0')&&(j<UART2_rxBuffSize)&&(j%UART_7_8_FIFO));
			//Ack rx Interrupt flag
			UART2_ICR_R |= UART_ICR_RXIC;
		}
		//for receiver timeout isr
		//last data bytes are received here (data bytes <14)
		else if(UART2_MIS_R&UART_MIS_RTMIS){
			do{
				UART2_rxBuff[j]=UART2_DR_R;
			}while((++j<UART2_rxBuffSize)&&(!(UART2_FR_R&UART_FR_RXFE)));
			//Ack received timeout flag
			UART2_ICR_R |= UART_ICR_RTIC;
			*UART2_rxSemaphore=1;
		}
		if((UART2_rxBuff[(j-1)]=='\0')||(j==UART2_rxBuffSize)||(*UART2_rxSemaphore)){
			//clear array index for the next receive operation
			j=0;
			//disable rx interrupt
			UART2_IM_R &= ~UART_IM_RXIM;
			//disable received timeout interrupt
			UART2_IM_R &= ~UART_IM_RTIM;
			*UART2_rxSemaphore=1;
		}
	}
  //for overrun error isr
	if(UART2_MIS_R&UART_MIS_OEMIS){
		(*UART2_overrunErrorCallBackFunction)();
		UART2_IM_R &= ~UART_IM_OEIM;
	}
  //for break error isr
	if(UART2_MIS_R&UART_MIS_BEMIS){
		(*UART2_breakErrorCallBackFunction)();
		UART2_IM_R &= ~UART_IM_BEIM;
	}
  //for parity error isr
	if(UART2_MIS_R&UART_MIS_PEMIS){
		(*UART2_parityErrorCallBackFunction)();
		UART2_IM_R &= ~UART_IM_PEIM;
	}
  //for framing error isr
	if(UART2_MIS_R&UART_MIS_FEMIS){
		(*UART2_framingErrorCallBackFunction)();
		UART2_IM_R &= ~UART_IM_FEIM;
	}
}






void UART5_init(UART_registerType *config){
	uint8_t clkDiv;
	uint32_t fraction;
	if(config->con.UART_hseBit){
		clkDiv=8;
	}
	else{
		clkDiv=16;
	}
	//Enable the UART module using the RCGCUART register
	UART_moduleEnable(UART5_MODULE);															
	//Enable the clock to the appropriate GPIO module
	PORTx_ActivateClock(PORTE);																	
	//Set the GPIO AFSEL bits for the appropriate pins
	PORTE_DisableAnakogModeSelect(PIN4|PIN5);
	PORTE_EnableDigital(PIN4|PIN5);
	PORTE_EnableAlternateFunctionSelect(PIN4|PIN5);		
	//Configure the PMCn fields in the GPIOPCTL register to assign the SSI signals
	PORTE_PortControlFuction(GPIO_PCTL_PE4_U5RX);
	PORTE_PortControlFuction(GPIO_PCTL_PE5_U5TX);	
	//disable UART while comfiguration
	UART5_CTL_R = 1;										
	//setup baud rate registers
	UART5_IBRD_R = config->clkSource/(clkDiv*config->baudRate);
	fraction = (uint64_t)config->clkSource*100/(clkDiv*config->baudRate);
	fraction = (fraction-(UART5_IBRD_R*100))*64 + 50;
	UART5_FBRD_R = (fraction)/100;
	UART5_LCRH_R = config->lineCon.UART_lineControl;
	UART5_CTL_R = config->con.UART_control;
	//select clk source sysclk or PIOSC
	UART5_CC_R = config->UART_clkConfiguration;
	//enable UART
	UART5_CTL_R |= UART_CTL_UARTEN;
}
void UART5_sendReceiveDataSync(uint8_t *tx, uint8_t *rx, uint16_t txSize, uint16_t rxSize){
	uint16_t i=0, j=0;
	UART2_LCRH_R |= UART_LCRH_FEN;
	while(((tx)&&(i<txSize))||((rx)&&(j<rxSize))){
		//check if there is data to transfer.
		if(tx&&i<txSize){
			if(!(UART5_FR_R&UART_FR_TXFF)){
				UART5_DR_R = tx[i++];
			}
		}
		//check if there is data to receive.
		if(rx&&j<rxSize){
			if(!(UART5_FR_R&UART_FR_RXFE)){
				rx[j++] = UART5_DR_R;
			}
		}
	}
}



void UART5_sendReceiveDataASync(uint8_t *tx, uint8_t *rx, uint16_t txSize, uint16_t rxSize, uint8_t priority, uint8_t *txSemaphore, uint8_t *rxSemaphore){
	uint8_t i=0, numberOfBytesToSend=0;
	uint8_t fifoLevels[4]={UART_1_8_FIFO, UART_2_8_FIFO, UART_4_8_FIFO, UART_6_8_FIFO};
	UART5_IFLS_R=0;
	if(tx||rx){
		//if there is a data to send
		if(tx){
			UART5_txSemaphore=txSemaphore;
			//send single byte
			if(txSize==1){
				UART5_DR_R = *tx;
			}
			//for multi byte to send
			else{
				//enable fifo
				UART5_LCRH_R |= UART_LCRH_FEN;
				//if tx buffer size>16 send 14 data bytes
				//if array size=0 -> the array represents a string send data untill you find '\0' or max data sent is 14 bytes(7/8 of fifo)
				if((txSize>UART_FIFO_MAX)||(txSize==0)){
					numberOfBytesToSend=UART_7_8_FIFO;
				}
				//if 0<size<= max fifo size(16) send all bytes once in fifo and don't enable interrupt 
				else{
					numberOfBytesToSend=txSize;
				}
				//send data bytes from array
				do{
					UART5_DR_R = tx[i];
				}while((tx[i++]!='\0')&&(i<numberOfBytesToSend));
			}
			//if tx buffer size>16 or last byte sent is != '\0'
			//enable tx interrupt to interrupt when fifo level is <= 2 data bytes
			if((txSize>UART_FIFO_MAX)||((txSize==0)&&(tx[(i-1)]!='\0'))){
				UART5_txBuff=tx;
				UART5_txBuffSize=txSize;
				UART5_IFLS_R &= ~UART_IFLS_TX1_8;
				UART5_IFLS_R |= UART_IFLS_TX1_8;
			}
			//enable end of transimission interrupt to signal tx semaphore
			//in case of data bytes <= 16
			else{
				UART5_CTL_R |= UART_CTL_EOT;
			}
			//Clear Interrupt flag
			UART5_ICR_R |= UART_ICR_TXIC;
			//Enable UART5 tx interrupt
			UART5_IM_R |= UART_IM_TXIM;
		}
		//if there is a data to receive
		if(rx){
			i=0;
			UART5_rxBuff=rx;
			UART5_rxBuffSize=rxSize;
			UART5_rxSemaphore=rxSemaphore;
			//if one byte is about to received and one or no bytes are transimitted 
			//disable fifo to interrupt when single byte is received
			if((rxSize==1)&&((!tx)||(txSize==1))){
				UART5_LCRH_R &= ~UART_LCRH_FEN;
			}
			else{
				//more than one byte enable fifo
				UART5_LCRH_R |= UART_LCRH_FEN;
				if(rxSize<14){
					//if size = one of fifo level 2, 4, 8, 12 adjust to interrup on that level
					//else adjust interrupt on fifo level 14 and receive that data on receiver timeout interrupt
					//The receive timeout interrupt is asserted when the receive FIFO is not empty, and no further data is received
					//over a 32-bit period when the HSE bit is clear or over a 64-bit period when the HSE bit is set.
					while((rxSize!=fifoLevels[i])&&(i<4)){
						i++;
					}
				}
				//if more than 14 bytes calc. number of interrupt times for each full 14 byte received
				//remaining bytes will be received in receiver timeout interrupt
				else{
					//to select 7/8 of fifo
					i=4;
				}
				if(i==4){
					//Clear Interrupt flag
					UART5_ICR_R |= UART_ICR_RTIC;
					//Enable UART5 rx timeout interrupt
					UART5_IM_R |= UART_IM_RTIM;
				}
			}
			//fifo level select
			UART5_IFLS_R |= i<<3;
			//Clear Interrupt flag
			UART5_ICR_R |= UART_ICR_RXIC;
			//Enable UART5 rx interrupt
			UART5_IM_R |= UART_IM_RXIM;
		}
		//Set UART5 Interrupt priority
		NVIC_PRI15_R &= ~(priority<<13);
		NVIC_PRI15_R |= (priority<<13);
		//Enable UART5 in NVIC
		NVIC_EN1_R |= (1<<29);	
	}
}



void UART5_setErrorsCallBackFunction(void (*overrunError)(void), void (*parityError)(void), void (*framingError)(void), void (*breakError)(void)){
	if(overrunError){
		UART5_IM_R |= UART_IM_OEIM;
		UART5_overrunErrorCallBackFunction = overrunError;
	}
	if(parityError){
		UART5_IM_R |= UART_IM_PEIM;
		UART5_parityErrorCallBackFunction = parityError;
	}
	if(framingError){
		UART5_IM_R |= UART_IM_FEIM;
		UART5_framingErrorCallBackFunction = framingError;
	}
	if(breakError){
		UART5_IM_R |= UART_IM_BEIM;
		UART5_breakErrorCallBackFunction = breakError;
	}
}
void UART5_Handler(void){
	static uint16_t i=UART_7_8_FIFO, j=0;
	//for transmit isr
	if(UART5_MIS_R&UART_MIS_TXMIS){
		//if tx interrupt is generated because of tx buffer size>16
		if(!(UART5_CTL_R&UART_CTL_EOT)){
			do{
				//send bytes till find '\0' or i=tx buffer size or send maximum of 14 bytes at a time
				UART5_DR_R = UART5_txBuff[i];
			}while((UART5_txBuff[i++]!='\0')&&((i<UART5_txBuffSize)||(UART5_txBuffSize==0))&&(i%UART_7_8_FIFO));
			//ack tx interrupt
			UART5_ICR_R |= UART_ICR_TXIC;
			//if last byte sent is '\0' or total data sent=tx buffer size
			//enable end of transimission to signal tx semaphore
			if((UART5_txBuff[(i-1)]=='\0')||(i==UART5_txBuffSize)){
				UART5_CTL_R |= UART_CTL_EOT;
			}
		}
		//end of transimission to signal tx semaphore and disable interrupt
		else{
			//clear tx array index for the next array to be sent
			i=0;
			//disable UART5 end of transimission
			UART5_CTL_R &= ~UART_CTL_EOT;
			//disable UART5 tx interrupt
			UART5_IM_R &= ~UART_IM_TXIM;
			*UART5_txSemaphore=1;
		}
	}
	//for receive isr
	if((UART5_MIS_R&UART_MIS_RXMIS)||(UART5_MIS_R&UART_MIS_RTMIS)){
		if(UART5_MIS_R&UART_MIS_RXMIS){
			//receive 14 bytes max or untill '\0' is received
			do{
				UART5_rxBuff[j]=UART5_DR_R;
			}while((UART5_rxBuff[j++]!='\0')&&(j<UART5_rxBuffSize)&&(j%UART_7_8_FIFO));
			//Ack rx Interrupt flag
			UART5_ICR_R |= UART_ICR_RXIC;
		}
		//for receiver timeout isr
		//last data bytes are received here (data bytes <14)
		else if(UART5_MIS_R&UART_MIS_RTMIS){
			do{
				UART5_rxBuff[j]=UART5_DR_R;
			}while((++j<UART5_rxBuffSize)&&(!(UART5_FR_R&UART_FR_RXFE)));
			//Ack received timeout flag
			UART5_ICR_R |= UART_ICR_RTIC;
			*UART5_rxSemaphore=1;
		}
		if((UART5_rxBuff[(j-1)]=='\0')||(j==UART5_rxBuffSize)||(*UART5_rxSemaphore)){
			//clear array index for the next receive operation
			j=0;
			//disable rx interrupt
			UART5_IM_R &= ~UART_IM_RXIM;
			//disable received timeout interrupt
			UART5_IM_R &= ~UART_IM_RTIM;
			*UART5_rxSemaphore=1;
		}
	}
  //for overrun error isr
	if(UART5_MIS_R&UART_MIS_OEMIS){
		(*UART5_overrunErrorCallBackFunction)();
		UART5_IM_R &= ~UART_IM_OEIM;
	}
  //for break error isr
	if(UART5_MIS_R&UART_MIS_BEMIS){
		(*UART5_breakErrorCallBackFunction)();
		UART5_IM_R &= ~UART_IM_BEIM;
	}
  //for parity error isr
	if(UART5_MIS_R&UART_MIS_PEMIS){
		(*UART5_parityErrorCallBackFunction)();
		UART5_IM_R &= ~UART_IM_PEIM;
	}
  //for framing error isr
	if(UART5_MIS_R&UART_MIS_FEMIS){
		(*UART5_framingErrorCallBackFunction)();
		UART5_IM_R &= ~UART_IM_FEIM;
	}
}
	
