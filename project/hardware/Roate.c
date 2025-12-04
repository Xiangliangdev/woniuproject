#include "stm32f10x.h"                  // Device header
#include "Roate.h"

//初始化:GPIO
void roateInit(void){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		GPIO_InitTypeDef gpioStruct;
		gpioStruct.GPIO_Mode = GPIO_Mode_IPU;    //上拉输入模式，默认高电平（没有驱动能力）
		//gpioStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0;
		gpioStruct.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10;
		gpioStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB,&gpioStruct);

		// 2：AFIO(引脚复用）
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);   // 开时钟
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource11);   //PA1->EXTI1
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource10);		//PAO->EXTI1
		// 3：配置EXTI
		EXTI_InitTypeDef extiStruct;
		extiStruct.EXTI_Line = EXTI_Line11 | EXTI_Line10;   //线路配置（参考9.2.5)
		extiStruct.EXTI_LineCmd = ENABLE;
		extiStruct.EXTI_Mode = EXTI_Mode_Interrupt;
		extiStruct.EXTI_Trigger = EXTI_Trigger_Falling;
		EXTI_Init(&extiStruct);
	
		// 4：NVIC
		NVIC_InitTypeDef nvicStruct;
		// 配置优先级 - 使用分组2
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		nvicStruct.NVIC_IRQChannelPreemptionPriority = 1;
		nvicStruct.NVIC_IRQChannelSubPriority = 1;
		// 配置PA1
		nvicStruct.NVIC_IRQChannel = EXTI15_10_IRQn;     //配置通道
		nvicStruct.NVIC_IRQChannelCmd = ENABLE;      //使能
		NVIC_Init(&nvicStruct);
		
}

// 5：写中断函数：
// 系统产生中断时，会自动调用中断函数
void EXTI15_10_IRQHandler(){
	// 1：判断中断的来源
	if(EXTI_GetFlagStatus(EXTI_Line11) == SET){
		// 如果A相是0时，B相还是1，那么是顺时针
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11) == 0 && GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10) == 1){
		
		num++;
		}
		// 2：完了以后，要清除中断的标志位
		EXTI_ClearITPendingBit(EXTI_Line11);
		
	}

	// 1：判断中断的来源
	if(EXTI_GetFlagStatus(EXTI_Line10) == SET){
		// 如果B相是0时，A相还是1，那么是逆时针
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11) == 1 && GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10) == 0){
		
		num--;
		}
		// 2：完了以后，要清除中断的标志位
		EXTI_ClearITPendingBit(EXTI_Line10);
		
	}
}
