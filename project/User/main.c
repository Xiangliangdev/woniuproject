#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Led.h"
#include "Button.h"
#include "DHT1.h"
#include "Roate.h"
#include "TIM.h"
#include "PWM.h"
#include "Motor.h"


//int main(void){
//			
//		OLED_Init();
//		OLED_Init();
//		motorInit();
//	
//	
//	
//		leftForWard();
//		rightForWard();
//		while(1){
//		

//			
//				
//		}
//		
//}
#include "stm32f10x.h"
#include "led.h"
#include "motor.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

// 简单的延时函数
void delay_ms(uint32_t ms) {
    for(uint32_t i = 0; i < ms * 1000; i++) {
        for(uint32_t j = 0; j < 72; j++);  // 72MHz时钟下的简单延时
    }
}

// 测试LED功能
void testLED(void) {
    // 测试LED亮度渐变
    for(uint8_t i = 0; i <= 100; i += 20) {
        setLedBrightness(i);
        delay_ms(500);
    }
    
    // 开关测试
    ledOpen();
    delay_ms(1000);
    ledClose();
    delay_ms(1000);
}

// 测试电机功能
void testMotor(void) {
    // 正转测试
    motorForward();
    delay_ms(2000);
    
    // 停止
    motorStop();
    delay_ms(1000);
    
    // 反转测试
    motorBackward();
    delay_ms(2000);
    
    // 停止
    motorStop();
    delay_ms(1000);
}

int main(void) {
    // 系统初始化
    SystemInit();
    
    // 初始化各模块
    ledInit();      // 初始化LED（PWM调光）
    motorInit();    // 初始化电机控制
    
    // 初始状态：LED关闭，电机停止
    ledClose();
    motorStop();
    
    while(1) {
        // 测试序列1：LED亮度测试
        testLED();
        
        // 测试序列2：电机控制测试  
        testMotor();
        
        // 组合测试：LED和电机同时工作
        ledOpen();
        motorForward();
        delay_ms(3000);
        
        // 设置50%亮度，电机反转
        setLedBrightness(50);
        motorBackward();
        delay_ms(3000);
        
        // 全部停止
        ledClose();
        motorStop();
        delay_ms(2000);
    }
}