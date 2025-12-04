#include "stm32f10x.h"                  // Device header

// TIM3 设置为主定时器
void tim3Init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_InternalClockConfig(TIM3);
	
	//设置成主模式，作为输出来源
	TIM_SelectMasterSlaveMode(TIM3,TIM_MasterSlaveMode_Enable);
	TIM_SelectOutputTrigger(TIM3,TIM_TRGOSource_Update);      //TIM3的更新事件，作为输出

	TIM_TimeBaseInitTypeDef timeStruct;
	timeStruct.TIM_ClockDivision = TIM_CKD_DIV1;      //	主频进来是72M,不用再分频了
	timeStruct.TIM_CounterMode = TIM_CounterMode_Up;   //	向上计数 
	timeStruct.TIM_Period = 10000 - 1;								//	ARR设置 
	timeStruct.TIM_Prescaler = 7200 - 1;							//	分频器	
	timeStruct.TIM_RepetitionCounter = 0x00;					//	重复计数(高级定时器才用的)
	TIM_TimeBaseInit(TIM3,&timeStruct);
	
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	
	TIM_Cmd(TIM3,ENABLE);
}



// TIM2是从定时器
void tim2Init(void){
	// 1：开时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	// 2：设置时钟的来源;给定时器2配置一个内部72M的时钟，这句可以不写，默认就是72M
	//TIM_InternalClockConfig(TIM2);
	
	// 设置成外部时钟
	// 2-2:	给定时器配置外部时钟
	// 参数1：定时器外设
	// 参数2：是否要先分频一次（这里不要）
	// 参数3：ETR信号的极性（两个选项：反转，不反转）
	// 参数4：数字滤波器（0x00 到 0x0F 之间的值 ，代表的是采样率）
	//TIM_ETRClockMode2Config( TIM2 , TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x0F);
	
	// 2-3: TIM2配置成从模式，
	TIM_SelectSlaveMode(TIM2,TIM_SlaveMode_External1);
	TIM_SelectInputTrigger(TIM2,TIM_TS_ITR2);
	
	// 3：设置时基单元
	TIM_TimeBaseInitTypeDef time_base;
	time_base.TIM_ClockDivision = TIM_CKD_DIV1;      // 主频进来是72M，不用再分频了
	time_base.TIM_CounterMode = TIM_CounterMode_Up;  // 向上计数
	time_base.TIM_Period = 5 - 1;                // ARR设置
	time_base.TIM_Prescaler = 1 - 1;              // 分频器
	time_base.TIM_RepetitionCounter = 0x00;          // 重复计数（高级定时器才用）
	TIM_TimeBaseInit(TIM2,&time_base);
	
	// 清除定时器的更新事件标志位
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	
	// 4：配置中断
	// 定时器中断 - 更新中断
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	// 5：NVIC
	NVIC_InitTypeDef nvic_init;
	nvic_init.NVIC_IRQChannel = TIM2_IRQn;
	nvic_init.NVIC_IRQChannelCmd = ENABLE;
	nvic_init.NVIC_IRQChannelPreemptionPriority = 1;
	nvic_init.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&nvic_init);
	
	// 6：启动定时器
	TIM_Cmd(TIM2,ENABLE);
	
}

// 7：中断函数
uint16_t num = 0;

void TIM2_IRQHandler(void){
	// 1：先判断中断的来源
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET){
		num++;
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}

