
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
#include <math.h>

void loop();


void enableSysTick() {
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk; // enable
}

void disableSysTick() {
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk; // disable
}

volatile uint32_t ticks;
void SysTick_Handler() {
	++ticks;
}

static bool asyncTimerOn = false;

// (happens every 1ms)
static void delay(__IO uint32_t milliseconds) {
	uint32_t microseconds = 0;
	if(!asyncTimerOn) ticks = 0; // make sure we don't run out of 32 bits at random times
	else microseconds += ticks;
	enableSysTick(); // start as soon as possible
	microseconds += milliseconds * 10; //clarity
	while (ticks < microseconds);
	disableSysTick();
}

// (happens every 1microseconds)
static void delay_micro(__IO uint32_t microseconds) {
	if(!asyncTimerOn) ticks = 0; // make sure we don't run out of 32 bits at random times
	else microseconds += ticks;
	enableSysTick(); // start as soon as possible
	while (ticks < microseconds);
	disableSysTick();
}


// timer is at microseconds resolution. enum values can be used to multiply with ticks to get human readable results
typedef enum timeinterval{
	microseconds = 1, milliseconds = 1000, seconds = 1000000
} timeinterval;


// aslında daha iyi bir fikrim var: startAsyncTimer a her sayaç için özel elapsedTime değişkeni yapalım, startAsyncTimer fonksiyonuda bu değişkenlerin
// hepsinin pointer ını depolasın ve yeni timer gelince bütün timerların elapsedTime'ları geçen süre kadar artırılsın. Tabi bu iş için C++ la compile
// etmeyi halledersem çok güzel olur :)
static uint32_t startAsyncStopwatch() {
	if (asyncTimerOn) return ticks; // if timer is already on then abstractly another timer is working. Don't reset that timer
	asyncTimerOn = true;
	ticks = 0;
	enableSysTick();
	return 0; // 0 elapsed
}

#define stopAsyncTimer disableSysTick();

static uint32_t elapsedTime(uint32_t offset, timeinterval interval){
	int ret = floor(ticks / interval);
	return ret - offset;
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

void EXTI0_IRQHandler() {
	if (EXTI_GetITStatus(EXTI_Line0)) {
		EXTI_ClearITPendingBit(EXTI_Line0); // Clear the flag
		
		GPIO_ToggleBits(GPIOD, pin14);
//		delay(200);
		
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
	
	
	GPIO_SetBits(GPIOD, pin14);
	
	
	// connect exti0 to gpioA. Since it is exti0 the interrupt will use pin0
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0 /*exti0*/ );
	
	//Configure the exti0 line
	EXTI_InitTypeDef extiStructure;
	
	extiStructure.EXTI_Line = EXTI_Line0;
	extiStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	// set up when interrupt will trigger. Since we set up the button to pull down (gpioA pin0. pupd_down it is),
	// when button is not pressed it will be logic LOW. If we set trigger to rising then it will trigger when the
	// button is pressed, if we set it to falling then it will trigger after releasing the button
	extiStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	extiStructure.EXTI_LineCmd = ENABLE;
	
	EXTI_Init(&extiStructure);
	
	// configure the nvic
	NVIC_InitTypeDef nvicStructure;
	nvicStructure.NVIC_IRQChannel = EXTI0_IRQn;	// interrupt for exti0
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	nvicStructure.NVIC_IRQChannelSubPriority = 0x01;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);
	

	setup_Periph();
	usart_puts(USART2, "hello world!\n");
	startAsyncStopwatch();
	
	while(true) loop();
	return 0;
}

bool buttonReleased = true;
char msg[50];
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
	static uint32_t offset = 0;
	static uint32_t elapsed = 0;
	elapsed = elapsedTime(offset, seconds);
	if(elapsed != 0){
		sprintf(msg, "%i offset %i elapsed\n", offset, elapsed);
		offset++;
	}
	usart_puts(USART2, msg);
	
	
}














