#include "stm32f10x.h"                  // Device header

//延迟1毫秒
void delayByUs(uint32_t us){
		// 1：先设置初始值
		SysTick->LOAD = 72 * us - 1;
		// 2：设置计数器的值
		SysTick->VAL = 0;
		// 3：使用控制寄存器
		SysTick->CTRL = 0x00000005; //使能，并设置成72MHZ
	
		// 使用while循环盯着count flag寄存器，它变成了1，说明时间到
		while( !(SysTick->CTRL & 0x00010000) ){};
		// 4：关闭计数器
		SysTick->CTRL = 0;  // 完全关闭
}

// 延迟一毫秒
void delayByMs(uint32_t ms){
		if(ms == 0){
				return;
	}
		while(ms--){
				delayByUs(1000);
	}
}

//延迟一秒
void delayByS(uint32_t s){
	if(s == 0){
			return;
	}
	while(s--){
		delayByMs(1000);
	}
}
