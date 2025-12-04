//#ifndef MOTOR_H
//#define MOTOR_H

//// 左、右电机
//void motorInit(void);

////左前进
//void leftForWard(void);

////左后退
//void leftBack(void);

////左停止
//void leftStop(void);

////右前进
//void rightForWard(void);

////右后退
//void rightBack(void);

////右停止
//void rightStop(void);

//#endif


#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"

void motorInit(void);
void motorForward(void);
void motorBackward(void);
void motorStop(void);
void setMotorSpeed(uint8_t speed);

#endif