//prevents double inclusion
#ifndef GPIO_H_
#define GPIO_H_
#include "tm4c123gh6pm.h"
#include "data_types.h"
#define GPIO_LOCK_KEY 0x4C4F434B
#define PIN0 1
#define PIN1 2
#define PIN2 4
#define PIN3 8
#define PIN4 16
#define PIN5 32
#define PIN6 64
#define PIN7 128
#define PORTA 1
#define PORTB 2
#define PORTC 4
#define PORTD 8
#define PORTE 16
#define PORTF 32
#define PCTL_GPIO 0

#define PORTA_DisableAnakogModeSelect(pinNumber) (GPIO_PORTA_AMSEL_R &= ~pinNumber)
#define PORTA_EnableAnakogModeSelect(pinNumber)(GPIO_PORTA_AMSEL_R |= pinNumber)
#define PORTA_DirectionInput(pinNumber) (GPIO_PORTA_DIR_R &= ~pinNumber)
#define PORTA_DirectionOutput(pinNumber) (GPIO_PORTA_DIR_R |= pinNumber)
#define PORTA_DisableDigital(pinNumber) (GPIO_PORTA_DEN_R &= ~pinNumber)
#define PORTA_EnableDigital(pinNumber) (GPIO_PORTA_DEN_R |= pinNumber)
#define PORTA_DisableAlternateFunctionSelect(pinNumber) (GPIO_PORTA_AFSEL_R &= ~pinNumber)
#define PORTA_EnableAlternateFunctionSelect(pinNumber) (GPIO_PORTA_AFSEL_R |= pinNumber)
#define PORTA_PortControlFuction(functionCode) {\
																									GPIO_PORTA_PCTL_R &= ~functionCode;\
																									GPIO_PORTA_PCTL_R |= functionCode;\
																							 }
#define PORTA_ClearBit(pinNumber)(GPIO_PORTA_DATA_R &= ~pinNumber)
#define PORTA_SetBit(pinNumber)(GPIO_PORTA_DATA_R |= pinNumber)
#define PORTA_Data(data)(GPIO_PORTA_DATA_R = data)			
#define PORTA_ReadData()(GPIO_PORTA_DATA_R)																			
#define PORTA_DisablePullUp(pinNumber) (GPIO_PORTA_PUR_R &= ~pinNumber)
#define PORTA_EnablePullUp(pinNumber) (GPIO_PORTA_PUR_R |= pinNumber)
#define PORTA_DisablePullDown(pinNumber) (GPIO_PORTA_PDR_R &= ~pinNumber)
#define PORTA_EnablePullDown(pinNumber) (GPIO_PORTA_PDR_R |= pinNumber)
							
#define PORTB_DisableAnakogModeSelect(pinNumber) (GPIO_PORTB_AMSEL_R &= ~pinNumber)
#define PORTB_EnableAnakogModeSelect(pinNumber) (GPIO_PORTB_AMSEL_R |= pinNumber)
#define PORTB_DirectionInput(pinNumber) (GPIO_PORTB_DIR_R &= ~pinNumber)
#define PORTB_DirectionOutput(pinNumber) (GPIO_PORTB_DIR_R |= pinNumber)
#define PORTB_DisableDigital(pinNumber) (GPIO_PORTB_DEN_R &= ~pinNumber)
#define PORTB_EnableDigital(pinNumber) (GPIO_PORTB_DEN_R |= pinNumber)
#define PORTB_DisableAlternateFunctionSelect(pinNumber) (GPIO_PORTB_AFSEL_R &= ~pinNumber)
#define PORTB_EnableAlternateFunctionSelect(pinNumber) (GPIO_PORTB_AFSEL_R |= pinNumber)
#define PORTB_PortControlFuction(functionCode) {\
																									GPIO_PORTB_PCTL_R &= ~functionCode;\
																									GPIO_PORTB_PCTL_R |= functionCode;\
																							 }
#define PORTB_ClearBit(pinNumber) (GPIO_PORTB_DATA_R &= ~pinNumber)
#define PORTB_SetBit(pinNumber) (GPIO_PORTB_DATA_R |= pinNumber)
#define PORTB_Data(data) (GPIO_PORTB_DATA_R = data)		
#define PORTB_ReadData() (GPIO_PORTB_DATA_R)																
#define PORTB_DisablePullUp(pinNumber) (GPIO_PORTB_PUR_R &= ~pinNumber)
#define PORTB_EnablePullUp(pinNumber) (GPIO_PORTB_PUR_R |= pinNumber)
#define PORTB_DisablePullDown(pinNumber) (GPIO_PORTB_PDR_R &= ~pinNumber)
#define PORTB_EnablePullDown(pinNumber) (GPIO_PORTB_PDR_R |= pinNumber)
																
#define PORTC_DisableAnakogModeSelect(pinNumber) (GPIO_PORTC_AMSEL_R &= ~pinNumber)
#define PORTC_EnableAnakogModeSelect(pinNumber) (GPIO_PORTC_AMSEL_R |= pinNumber)
#define PORTC_DirectionInput(pinNumber) (GPIO_PORTC_DIR_R &= ~pinNumber)
#define PORTC_DirectionOutput(pinNumber) (GPIO_PORTC_DIR_R |= pinNumber)
#define PORTC_DisableDigital(pinNumber) (GPIO_PORTC_DEN_R &= ~pinNumber)
#define PORTC_EnableDigital(pinNumber) (GPIO_PORTC_DEN_R |= pinNumber)
#define PORTC_DisableAlternateFunctionSelect(pinNumber) (GPIO_PORTC_AFSEL_R &= ~pinNumber)
#define PORTC_EnableAlternateFunctionSelect(pinNumber) (GPIO_PORTC_AFSEL_R |= pinNumber)
#define PORTC_PortControlFuction(functionCode) {\
																									GPIO_PORTC_PCTL_R &= ~functionCode;\
																									GPIO_PORTC_PCTL_R |= functionCode;\
																							 }
#define PORTC_ClearBit(pinNumber) (GPIO_PORTC_DATA_R &= ~pinNumber)
#define PORTC_SetBit(pinNumber) (GPIO_PORTC_DATA_R |= pinNumber)
#define PORTC_Data(data) (GPIO_PORTC_DATA_R = data)				
#define PORTC_ReadData() (GPIO_PORTC_DATA_R)																
#define PORTC_DisablePullUp(pinNumber) (GPIO_PORTC_PUR_R &= ~pinNumber)
#define PORTC_EnablePullUp(pinNumber) (GPIO_PORTC_PUR_R |= pinNumber)
#define PORTC_DisablePullDown(pinNumber) (GPIO_PORTC_PDR_R &= ~pinNumber)
#define PORTC_EnablePullDown(pinNumber) (GPIO_PORTC_PDR_R |= pinNumber)
																
#define PORTD_DisableAnakogModeSelect(pinNumber) (GPIO_PORTD_AMSEL_R &= ~pinNumber)
#define PORTD_EnableAnakogModeSelect(pinNumber) (GPIO_PORTD_AMSEL_R |= pinNumber)
#define PORTD_DirectionInput(pinNumber) (GPIO_PORTD_DIR_R &= ~pinNumber)
#define PORTD_DirectionOutput(pinNumber) (GPIO_PORTD_DIR_R |= pinNumber)
#define PORTD_DisableDigital(pinNumber) (GPIO_PORTD_DEN_R &= ~pinNumber)
#define PORTD_EnableDigital(pinNumber) (GPIO_PORTD_DEN_R |= pinNumber)
#define PORTD_DisableAlternateFunctionSelect(pinNumber) (GPIO_PORTD_AFSEL_R &= ~pinNumber)
#define PORTD_EnableAlternateFunctionSelect(pinNumber) (GPIO_PORTD_AFSEL_R |= pinNumber)
#define PORTD_PortControlFuction(functionCode) {\
																									GPIO_PORTD_PCTL_R &= ~functionCode;\
																									GPIO_PORTD_PCTL_R |= functionCode;\
																							 }
#define PORTD_ClearBit(pinNumber) (GPIO_PORTD_DATA_R &= ~pinNumber)
#define PORTD_SetBit(pinNumber) (GPIO_PORTD_DATA_R |= pinNumber)
#define PORTD_Data(data) (GPIO_PORTD_DATA_R = data)		
#define PORTD_ReadData() (GPIO_PORTD_DATA_R)																
#define PORTD_DisablePullUp(pinNumber) (GPIO_PORTD_PUR_R &= ~pinNumber)
#define PORTD_EnablePullUp(pinNumber) (GPIO_PORTD_PUR_R |= pinNumber)
#define PORTD_DisablePullDown(pinNumber) (GPIO_PORTD_PDR_R &= ~pinNumber)
#define PORTD_EnablePullDown(pinNumber) (GPIO_PORTD_PDR_R |= pinNumber)
																
#define PORTE_DisableAnakogModeSelect(pinNumber) (GPIO_PORTE_AMSEL_R &= ~pinNumber)
#define PORTE_EnableAnakogModeSelect(pinNumber) (GPIO_PORTE_AMSEL_R |= pinNumber)
#define PORTE_DirectionInput(pinNumber) (GPIO_PORTE_DIR_R &= ~pinNumber)
#define PORTE_DirectionOutput(pinNumber) (GPIO_PORTE_DIR_R |= pinNumber)
#define PORTE_DisableDigital(pinNumber) (GPIO_PORTE_DEN_R &= ~pinNumber)
#define PORTE_EnableDigital(pinNumber) (GPIO_PORTE_DEN_R |= pinNumber)
#define PORTE_DisableAlternateFunctionSelect(pinNumber) (GPIO_PORTE_AFSEL_R &= ~pinNumber)
#define PORTE_EnableAlternateFunctionSelect(pinNumber) (GPIO_PORTE_AFSEL_R |= pinNumber)
#define PORTE_PortControlFuction(functionCode) {\
																									GPIO_PORTE_PCTL_R &= ~functionCode;\
																									GPIO_PORTE_PCTL_R |= functionCode;\
																							 }
#define PORTE_ClearBit(pinNumber) (GPIO_PORTE_DATA_R &= ~pinNumber)
#define PORTE_SetBit(pinNumber) (GPIO_PORTE_DATA_R |= pinNumber)
#define PORTE_Data(data) (GPIO_PORTE_DATA_R = data)							
#define PORTE_ReadData() (GPIO_PORTE_DATA_R)																
#define PORTE_DisablePullUp(pinNumber) (GPIO_PORTE_PUR_R &= ~pinNumber)
#define PORTE_EnablePullUp(pinNumber) (GPIO_PORTE_PUR_R |= pinNumber)
#define PORTE_DisablePullDown(pinNumber) (GPIO_PORTE_PDR_R &= ~pinNumber)
#define PORTE_EnablePullDown(pinNumber) (GPIO_PORTE_PDR_R |= pinNumber)
																
#define PORTF_DisableAnakogModeSelect(pinNumber) (GPIO_PORTF_AMSEL_R &= ~pinNumber)
#define PORTF_EnableAnakogModeSelect(pinNumber) (GPIO_PORTF_AMSEL_R |= pinNumber)
#define PORTF_DirectionInput(pinNumber) (GPIO_PORTF_DIR_R &= ~pinNumber)
#define PORTF_DirectionOutput(pinNumber) (GPIO_PORTF_DIR_R |= pinNumber)
#define PORTF_DisableDigital(pinNumber) (GPIO_PORTF_DEN_R &= ~pinNumber)
#define PORTF_EnableDigital(pinNumber) (GPIO_PORTF_DEN_R |= pinNumber)
#define PORTF_DisableAlternateFunctionSelect(pinNumber) (GPIO_PORTF_AFSEL_R &= ~pinNumber)
#define PORTF_EnableAlternateFunctionSelect(pinNumber) (GPIO_PORTF_AFSEL_R |= pinNumber)
#define PORTF_PortControlFuction(functionCode) {\
																									GPIO_PORTF_PCTL_R &= ~functionCode;\
																									GPIO_PORTF_PCTL_R |= functionCode;\
																							 }
#define PORTF_ClearBit(pinNumber) (GPIO_PORTF_DATA_R &= ~pinNumber)
#define PORTF_SetBit(pinNumber) (GPIO_PORTF_DATA_R |= pinNumber)
#define PORTF_Data(data) (GPIO_PORTF_DATA_R = data)					
#define PORTF_ReadData() (GPIO_PORTF_DATA_R)																
#define PORTF_DisablePullUp(pinNumber) (GPIO_PORTF_PUR_R &= ~pinNumber)
#define PORTF_EnablePullUp(pinNumber) (GPIO_PORTF_PUR_R |= pinNumber)
#define PORTF_DisablePullDown(pinNumber) (GPIO_PORTF_PDR_R &= ~pinNumber)
#define PORTF_EnablePullDown(pinNumber) (GPIO_PORTF_PDR_R |= pinNumber)

void PORTx_ActivateClock(uint8_t port);
void PORTF_UnlockPinPF0(void);
void PORTD_UnlockPinPD7(void);
void PORTC_UnlockPinPC3_0(uint8_t pinNumber);
#endif
