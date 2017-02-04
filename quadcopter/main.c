
#define OUTPUT	GPIO_Mode_OUT
#define INPUT	GPIO_Mode_IN
#define NOPULL	GPIO_PuPd_NOPULL

#define true	1
#define false	0

#define read(idr,pin) (idr & pin)



#include <stdio.h>
#include <stm32f4xx.h>
#include <stdbool.h>
#include "def.h"


void loop();


void enableSysTick() {
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk; // enable
}

void disableSysTick() {
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk; // disable
}

volatile uint32_t msTicks;
void SysTick_Handler() {
	++msTicks;
}

// (happens every 1ms)
static void delay(__IO uint32_t milliseconds) {
	msTicks = 0;
	enableSysTick();
	milliseconds *= 10;
	while (msTicks < milliseconds);
	disableSysTick();
}

// (happens every 1microseconds)
static void delay_micro(__IO uint32_t dlyTicks) {
	msTicks = 0;
	enableSysTick();
	while (msTicks < dlyTicks);
	disableSysTick();
}

// microsecond resolution
void setSysTick() {
	if (SysTick_Config(SystemCoreClock / 1000000)) {
		// capture error
		while(1);
	}
}

void gpio(GPIO_TypeDef* GPIOx, uint32_t pin, GPIOMode_TypeDef mode, GPIOPuPd_TypeDef PuPd) {
	GPIO_InitTypeDef initStructure;
	
	initStructure.GPIO_Pin = pin;
	initStructure.GPIO_Mode = mode;
	initStructure.GPIO_OType = GPIO_OType_PP;
	initStructure.GPIO_Speed = GPIO_Speed_100MHz;
	initStructure.GPIO_PuPd = PuPd;
	GPIO_Init(GPIOx, &initStructure);
}

void setup_Periph() {
	GPIO_InitTypeDef gpioStructure;
	USART_InitTypeDef usartStructure;
	NVIC_InitTypeDef nvicStructure;
	
	
	// Enable the periph clock for usart1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	// Enable the GPIOA clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	// Setup the gpio pins for Tx and Rx
	gpioStructure.GPIO_Pin = pin2 | pin3;
	gpioStructure.GPIO_Mode = GPIO_Mode_AF;
	gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
	gpioStructure.GPIO_OType = GPIO_OType_PP;
	gpioStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_Init(GPIOA, &gpioStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	
	usartStructure.USART_BaudRate = 230400;
	usartStructure.USART_WordLength = USART_WordLength_8b;
	usartStructure.USART_StopBits = USART_StopBits_1;
	usartStructure.USART_Parity = USART_Parity_No;
	usartStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usartStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2,&usartStructure);
	
	// enable interrupt for receive event on usart
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
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

int main() {
	setSysTick();
	
	// enable GPIOx clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	gpio(GPIOD, pin12 | pin13 | pin14 | pin15,
		 OUTPUT, NOPULL);
	gpio(GPIOA, pin0,
		 INPUT, GPIO_PuPd_DOWN);

	
	setup_Periph();
	usart_puts(USART2, "hello world!\n");
	
	
	
	
	while(true) loop();
	return 0;
}

bool buttonReleased = true;
void loop() {
	if(!buttonReleased && !read(GPIOA->IDR, pin0)){
		buttonReleased = true;
		delay(200);
	}
	
	if(buttonReleased && read(GPIOA->IDR, pin0)) {
		GPIO_ToggleBits(GPIOD, pin14);
		buttonReleased = false;
		delay(200);
	}
	
	
}














