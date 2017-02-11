//
//  usart.cpp
//  quadcopter
//
//  Created by Muhsin Fatih Yorulmaz on 11/02/2017.
//  Copyright © 2017 mekatrotekno. All rights reserved.
//

#include "usart.hpp"


// MARK: usart setup //TODO: fix rx, tx params. currently stm picks one magically for rx and tx
void setup_USART(int rx, int tx) {
	GPIO_InitTypeDef gpioStructure;
	USART_InitTypeDef usartStructure;
	NVIC_InitTypeDef nvicStructure;
	
	int pins[2] = {pow(2,rx),pow(2,tx)};
	// Enable the periph clock for usart1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	// Enable the GPIOA clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	// Setup the gpio pins for Tx and Rx
	gpioStructure.GPIO_Pin = pins[0] | pins[1];
	gpioStructure.GPIO_Mode = GPIO_Mode_AF;
	gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
	gpioStructure.GPIO_OType = GPIO_OType_PP;
	gpioStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_Init(GPIOA, &gpioStructure);
	
	GPIO_PinAFConfig(GPIOA, rx, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, tx, GPIO_AF_USART2);
	
	usartStructure.USART_BaudRate = BAUDRATE;
	usartStructure.USART_WordLength = USART_WordLength_8b;
	usartStructure.USART_StopBits = USART_StopBits_1;
	usartStructure.USART_Parity = USART_Parity_No;
	usartStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usartStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2,&usartStructure);
	
	// enable interrupt for receive event on usart
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	//IRQ: interrupt request
	nvicStructure.NVIC_IRQChannel = USART2_IRQn;
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 0; // 0: highest priority. (lowest=15)
	nvicStructure.NVIC_IRQChannelSubPriority = 0;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);
	
	USART_Cmd(USART2, ENABLE);
	
	
}
void usart_puts(USART_TypeDef *USARTx, volatile char *str) {
	while(*str) {
		//		while(!(USARTx->SR & 0x040)); // get 6'th bit
		// get the TC (transmission complete) flag
		while(!USART_GetFlagStatus(USART2, USART_FLAG_TC));
		USART_SendData(USARTx, *str);
		*str++;
	}
}
//MARK: read usart input
#define MAX_WORDLEN		255
volatile char receivedStr[MAX_WORDLEN + 1];
volatile bool newDataIn = false;
extern void usart_puts(USART_TypeDef *USARTx, volatile char *str);
// Interrupt request handler for all usart2 interrupts
// This interrupt handler will be executed each time a char is received in usart2
void USART2_IRQHandler(){
	// make sure it was usart2 and we didnt screw up things
	if(USART_GetITStatus(USART2, USART_IT_RXNE)){
		static int count = 0;
		char ch = USART2->DR;
		if((ch != '\n') && (count < MAX_WORDLEN)){
			receivedStr[count++] = ch;
		} else {
			receivedStr[count] = '\n';
			count = 0;
			newDataIn = true;
			usart_puts(USART2, receivedStr);
		}
	}
	
}
// return volatile.. http://stackoverflow.com/questions/24515505/assignment-discards-volatile-qualifier-from-pointer-target-type
volatile char* readUsart() {
	newDataIn = false;
	return receivedStr;
}
