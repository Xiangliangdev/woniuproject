//#include "stm32f10x.h"                  // Device header
//#include "PWM.h"

//// 左、右电机
//void motorInit(void){
//	// 1：
//	pwmInit();    //产生方波
//	// 用四个引脚控制：两路电机：前进、后退、停止；左：PB11\12  
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//	
//		//设置结构体
//	GPIO_InitTypeDef gpioStruct;
//	//如果这个输出是我们自己干的，就用OUT_PP，如果是硬件干的，就用AF_PP(复用推挽)
//	gpioStruct.GPIO_Mode = GPIO_Mode_Out_PP;
//	gpioStruct.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
//	gpioStruct.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB,&gpioStruct);     
//}

////左前进
//void leftForWard(void){
//	GPIO_SetBits(GPIOB,GPIO_Pin_11);
//	GPIO_ResetBits(GPIOB,GPIO_Pin_12);	
//}

////左后退
//void leftBack(void){
//	GPIO_SetBits(GPIOB,GPIO_Pin_12);
//	GPIO_ResetBits(GPIOB,GPIO_Pin_11);
//}
////左停止
//void leftStop(void){
//	GPIO_ResetBits(GPIOB,GPIO_Pin_11);
//	GPIO_ResetBits(GPIOB,GPIO_Pin_12);	
//}


#include "stm32f10x.h"
#include "motor.h"

// 电机状态
typedef struct {
    uint8_t status;  // 0:停止, 1:正转, 2:反转
    uint8_t speed;   // 转速 0-100
} MotorState;

MotorState motorState = {0, 50};  // 默认停止，速度50%

void motorInit(void){
    // 初始化GPIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitTypeDef gpioStruct;
    gpioStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    gpioStruct.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    gpioStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpioStruct);
    
    // 初始状态：停止
    motorStop();
    motorState.speed = 50;
}

void motorForward(void){
    GPIO_SetBits(GPIOB, GPIO_Pin_11);
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);
    motorState.status = 1;  // 正转
}

void motorBackward(void){
    GPIO_SetBits(GPIOB, GPIO_Pin_12);
    GPIO_ResetBits(GPIOB, GPIO_Pin_11);
    motorState.status = 2;  // 反转
}

void motorStop(void){
    GPIO_ResetBits(GPIOB, GPIO_Pin_11);
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);
    motorState.status = 0;  // 停止
}

void setMotorSpeed(uint8_t speed) {
    if(speed > 100) speed = 100;
    motorState.speed = speed;
    
    // 这里需要调用PWM设置函数来控制电机速度
    // setPWMSpeed(speed);  // 需要你实现PWM速度控制
}

uint8_t getMotorStatus(void) {
    return motorState.status;
}

uint8_t getMotorSpeed(void) {
    return motorState.speed;
}
