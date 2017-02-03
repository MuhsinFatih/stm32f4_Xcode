
#define OUTPUT	GPIO_Mode_OUT
#define INPUT	GPIO_Mode_IN
#define NOPULL	GPIO_PuPd_NOPULL

#define true	1
#define false	0

#define read(idr) idr & 0x01



#include <stdio.h>
#include <stm32f4xx.h>




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


int main() {
	setSysTick();

	// enable GPIOx clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	gpio(GPIOD,
		 GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15,
		 OUTPUT,
		 NOPULL);
	gpio(GPIOA,
		 GPIO_Pin_0,
		 INPUT,
		 NOPULL);
//	GPIO_SetBits(GPIOD, GPIO_Pin_14);
	while(true) {
		loop();
	}
}


void loop() {
	
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)) {
		GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
	}
	delay(200);
	
}














