/*
 * gpio.c
 *  	Created on: 11/7/2019
 *  	Author: Amr Halim
 */
#include "gpio.h"
void PORTx_ActivateClock(uint8_t port){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= port;     // activate clock for Port x
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
}
void PORTF_UnlockPinPF0(void){
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;   // unlock GPIO Port F
  GPIO_PORTF_CR_R |= PIN0;           // allow changes to PF0
}
void PORTD_UnlockPinPD7(void){
	GPIO_PORTD_LOCK_R = GPIO_LOCK_KEY;   // unlock GPIO Port D
  GPIO_PORTD_CR_R |= PIN7;           // allow changes to PD7
}
void PORTC_UnlockPinPC3_0(uint8_t pinNumber){
	GPIO_PORTC_LOCK_R = GPIO_LOCK_KEY;   // unlock GPIO Port C
  GPIO_PORTC_CR_R |= pinNumber;           // allow changes to PC3-0
}

