
#define OUTPUT	GPIO_Mode_OUT
#define INPUT	GPIO_Mode_IN
#define NOPULL	GPIO_PuPd_NOPULL

#define true	1
#define false	0

#define read(idr) idr & 0x01



#include <stdio.h>
#include <stm32f4xx.h>




void loop();

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

void enableSysTick() {
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk; // enable
}

void disableSysTick() {
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk; // disable
}

void gpio(uint32_t GPIOx, uint32_t pin, uint32_t mode, uint32_t PuPd) {
	GPIO_InitTypeDef initStructure;
	
	// enable GPIOx clock
	RCC_AHB1PeriphClockCmd(GPIOx, ENABLE);
	
	initStructure.GPIO_Pin = pin;
	initStructure.GPIO_Mode = mode;
	initStructure.GPIO_OType = GPIO_OType_PP;
	initStructure.GPIO_Speed = GPIO_Speed_100MHz;
	initStructure.GPIO_PuPd = PuPd;
	GPIO_Init(GPIOx, &initStructure);
}





int main() {
	setSysTick();
	gpio(GPIOD,
		 GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15,
		 OUTPUT,
		 NOPULL);
	gpio(GPIOA,
		 GPIO_Pin_0,
		 INPUT,
		 NOPULL);
	
	while(true) {
		loop();
	}
}


void loop() {
	
	if(read(GPIOA->IDR)) { // IDR: input data register
		GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
		delay(200);
	}
	
}
