//
//  test.hpp
//  quadcopter
//
//  Created by Mehmet emin Kartal on 06/02/2017.
//  Copyright © 2017 mekatrotekno. All rights reserved.
//

#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <stm32f4xx.h>


#define OUTPUT	GPIO_Mode_OUT
#define INPUT	GPIO_Mode_IN
#define NOPULL	GPIO_PuPd_NOPULL

#define true	1
#define false	0

#define read(idr,pin) (idr & pin)
#define enableFloatingPoint() (*((int*)0xE000ED88))|=0x0F00000;  // Floating Point donanimini aktiflestir.
#define REP(size) for(size_t i=0, length=size; i<length; ++i)
#define REPW(size)  size_t w,length; length=size; while(w<length)

#ifndef BAUDRATE
#define BAUDRATE 230400
#endif




#ifdef __cplusplus

#include "def.h"
#include "usart.hpp"

extern "C" {
#endif
	int main();
	void cpp_main();
#ifdef __cplusplus
}
#endif

