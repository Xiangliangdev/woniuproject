#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void buttonInit(void){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		GPIO_InitTypeDef gpioStruct;
		gpioStruct.GPIO_Mode = GPIO_Mode_IPU;    //上拉输入模式，默认高电平（没有驱动能力）
		gpioStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
		gpioStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB,&gpioStruct);
}
	
//按下去是0，没有按是1
uint8_t getButton01Number(void){
	return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0);
		}

uint8_t getButton02Number(void){
		return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1);
		}
	

