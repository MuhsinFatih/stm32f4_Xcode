
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

void gpio(uint32_t GPIOx, uint32_t pin, GPIOMode_TypeDef mode, GPIOPuPd_TypeDef PuPd) {
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	// ---------- GPIO  for LEDS -------- //
	// GPIOD Periph clock enable
	RCC_AHB1PeriphClockCmd(GPIOx, ENABLE);
	
	// Configure PD12, PD13, PD14 in output pushpull mode
	GPIO_InitStructure.GPIO_Pin = pin;
	GPIO_InitStructure.GPIO_Mode = mode;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = PuPd;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

}


void deneme(uint32_t gpiox, uint32_t pin, GPIOMode_TypeDef mode, GPIOPuPd_TypeDef pupd) {
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	// ---------- GPIO  for LEDS -------- //
	// GPIOD Periph clock enable
	RCC_AHB1PeriphClockCmd(gpiox, ENABLE);
	
	// Configure PD12, PD13, PD14 in output pushpull mode
	GPIO_InitStructure.GPIO_Pin = pin;
	GPIO_InitStructure.GPIO_Mode = mode;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = pupd;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

}


int main() {
	setSysTick();
//	gpio(RCC_AHB1Periph_GPIOD,
//		 GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15,
//		 GPIO_Mode_OUT,
//		 GPIO_PuPd_NOPULL);
//	gpio(RCC_AHB1Periph_GPIOA,
//		 GPIO_Pin_0,
//		 INPUT,
//		 NOPULL);
	deneme(RCC_AHB1Periph_GPIOD, GPIO_Pin_14, OUTPUT, GPIO_PuPd_NOPULL);
	GPIO_SetBits(GPIOD, GPIO_Pin_14);
	while(true) {
//		loop();
	}
}


void loop() {
	
	if(read(GPIOA->IDR)) { // IDR: input data register
		GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
		delay(200);
	}
	
}
