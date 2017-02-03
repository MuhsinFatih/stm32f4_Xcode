
#define OUTPUT	GPIO_Mode_OUT
#define INPUT	GPIO_Mode_IN
#define NOPULL	GPIO_PuPd_NOPULL

#define true	1
#define false	0

#define read(idr,pin) (idr & pin)



#include <stdio.h>
#include <stm32f4xx.h>
#include <stdbool.h>



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
	return 0;
}

bool buttonReleased = true;
void loop() {
	if(!buttonReleased && !read(GPIOA->IDR, GPIO_Pin_0)){
		buttonReleased = true;
		delay(200);
	}
	
	if(buttonReleased && read(GPIOA->IDR, GPIO_Pin_0)) {
		GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
		buttonReleased = false;
		delay(200);
	}
	
	
}














