//#ifndef LED_H
//#define LED_H

//void ledInit(void);
//void ledOpen(void);
//void ledClose(void);
////调节灯的亮暗
//void setLedBrightness(uint8_t brightness);

//#endif

#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

void ledInit(void);
void setLedBrightness(uint8_t brightness);
void ledOpen(void);
void ledClose(void);

#endif