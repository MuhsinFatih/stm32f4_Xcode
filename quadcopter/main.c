
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
	microseconds = 1, milliseconds = 10, seconds = 100
} timeinterval;


// aslında daha iyi bir fikrim var: startAsyncTimer a her sayaç için özel elapsedTime değişkeni yapalım, startAsyncTimer fonksiyonuda bu değişkenlerin
// hepsinin pointer ını depolasın ve yeni timer gelince bütün timerların elapsedTime'ları geçen süre kadar artırılsın. Tabi bu iş için C++ la compile
// etmeyi halledersem çok güzel olur :)
static uint32_t startAsyncStopwatch(uint32_t time, timeinterval interval) {
	if (asyncTimerOn) return ticks; // if timer is already on then abstractly another timer is working. Don't reset that timer
	asyncTimerOn = true;
	ticks = 0;
	enableSysTick();
	return 0; // 0 elapsed
}

#define stopAsyncTimer disableSysTick();

static uint32_t elapsedTime(uint32_t offset, timeinterval interval){
	return (ticks - offset) * interval;
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
	
	
	
	while(true) loop();
	return 0;
}

bool buttonReleased = true;
void loop() {
	
	
	
}














