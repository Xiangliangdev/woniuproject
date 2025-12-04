#ifndef TIM_H
#define TIM_H

extern int16_t num ;

// TIM3 设置为主定时器
void tim3Init(void);

// TIM2是从定时器
void tim2Init(void);

void TIM2_IRQHandler(void);








#endif
