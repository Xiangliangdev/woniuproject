#include "stm32f10x.h"                  // Device header
#include "Delay.h"


GPIO_InitTypeDef gpioStruct;
//初始化:PB11引脚
void dht1Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	gpioStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioStruct.GPIO_Pin = GPIO_Pin_11;
	gpioStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpioStruct);
}
	
//STM32要给DTH1发送数据，所以要设置成输出
void setOutPutPP(void){
	gpioStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&gpioStruct);
	// 把PB11设置成高电平
	GPIO_SetBits(GPIOB,GPIO_Pin_11);
}
//STM32接收DTH1发送的数据，所以要设置成输入
void setIpu(void){
	gpioStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&gpioStruct);
}

//1：主机发送信号
void sendRequest(void){
	// 1：设置成输出模式
	setOutPutPP();
	// 2：拉低电平至少18ms
	GPIO_ResetBits(GPIOB,GPIO_Pin_11);
	delayByMs(20);
	// 3：电平接高至少20-40us
	GPIO_SetBits(GPIOB,GPIO_Pin_11);
	delayByUs(40);
}
//2：接收信号:DHT1传过来的数据
uint8_t getRespone(void){
	// 1：设置成输入模式
	setIpu();
	//2：读取引脚的数据，是0才走流程：
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11) == 0){
		// 等待响应信号结束，耗掉相应的这段时间
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11) == 0);
		return SUCCESS;
	}
	return ERROR;
}

// 3：接收一个字节的数据：
uint8_t getByte(void){
	// 正好是一个字节数据
	uint8_t data = 0x00;
	
	// 一个字节有8bit，所以要循环8次
	for(int i = 0;i<8;i++){
		//耗掉，先高后低电平的时间
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11) == 1); //先高电平
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11) == 0); //后低电平
		delayByUs(35);
		
		// 35us之后，如果读到的还是高电平，那么传过来的就是1，反之是0；
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11) == 1){
			// 这个时候，传过来的数据是：1
			//data
			data |= (0x80 >> i);			
	}else{
		//反之，得到的是0
	}
}
	return data;
}
		