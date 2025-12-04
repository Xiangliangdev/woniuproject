#ifndef DHT1_H
#define DHT1_H

//初始化
void dht1Init(void);
//STM32要给DTH1发送数据，所以要设置成输出
void setOutPutPP(void);
//STM32接收DTH1发送的数据，所以要设置成输入
void setIpu(void);

//1：主机发送信号
void sendRequest(void);
//2：接收信号:DHT1传过来的数据
uint8_t getRespone(void);
// 3：接收一个字节的数据：
uint8_t getByte(void);

#endif
