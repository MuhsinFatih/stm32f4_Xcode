//
//  usart.hpp
//  quadcopter
//
//  Created by Muhsin Fatih Yorulmaz on 11/02/2017.
//  Copyright © 2017 mekatrotekno. All rights reserved.
//

#ifndef usart_hpp
#define usart_hpp

#include <stdio.h>
#include <stm32f4xx.h>
#include <stdbool.h>
#include "def.h"
#include <math.h>

class usart {
	USART_TypeDef device;
	public:
	usart(int rx, int tx);
	int	 printf(const char * __restrict, ...);
};


#endif /* usart_hpp */
