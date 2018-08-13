#ifndef __USER_H__
#define __USER_H__

#include "main.h"

extern void Init_NVIC_PriorityGroupConfig(void);
extern void Scan_TIM_NVIC_Config(void);
extern void Scan_TIM_config(void);
extern void USART_Init_Config(int bound);
extern void PWM_TIM_Init(u16 arr,u16 psc);
extern void Motor_Config(void);
extern void EncoderOne_TIM_Config(void);
extern void EncoderTwo_TIM_Config(void);
extern void SetPwm(int motor);
 
#define Count_Max 8000

#endif
