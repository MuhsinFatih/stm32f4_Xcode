
#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <stm32f4xx.h>


#define OUTPUT	GPIO_Mode_OUT
#define INPUT	GPIO_Mode_IN
#define NOPULL	GPIO_PuPd_NOPULL


#define read(idr,pin) (idr & pin)
#define enableFloatingPoint() (*((int*)0xE000ED88))|=0x0F00000;  // Floating Point donanimini aktiflestir.
#define REP(size) for(size_t i=0, length=size; i<length; ++i)
#define REPW(size)  size_t w,length; length=size; while(w<length)

#ifndef BAUDRATE
#define BAUDRATE 230400
#endif

#define pin0		GPIO_Pin_0  /* Pin 0 selected */
#define pin1		GPIO_Pin_1  /* Pin 1 selected */
#define pin2		GPIO_Pin_2  /* Pin 2 selected */
#define pin3		GPIO_Pin_3  /* Pin 3 selected */
#define pin4		GPIO_Pin_4  /* Pin 4 selected */
#define pin5		GPIO_Pin_5  /* Pin 5 selected */
#define pin6		GPIO_Pin_6  /* Pin 6 selected */
#define pin7		GPIO_Pin_7  /* Pin 7 selected */
#define pin8		GPIO_Pin_8  /* Pin 8 selected */
#define pin9		GPIO_Pin_9  /* Pin 9 selected */
#define pin10		GPIO_Pin_10  /* Pin 10 selected */
#define pin11		GPIO_Pin_11  /* Pin 11 selected */
#define pin12		GPIO_Pin_12  /* Pin 12 selected */
#define pin13		GPIO_Pin_13  /* Pin 13 selected */
#define pin14		GPIO_Pin_14  /* Pin 14 selected */
#define pin15		GPIO_Pin_15  /* Pin 15 selected */
#define pinAll		GPIO_Pin_All  /* All pins selected */


#ifdef __cplusplus

#include "def.h"

extern "C" {
#endif
	int main();
#ifdef __cplusplus
}
#endif

