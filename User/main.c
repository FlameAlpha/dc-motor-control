/**
  ******************************************************************************
  * @file    main.c
  * @author  Flame
  * @version 3.0
  * @date    07/06/2017
  * @brief   Main program body.

  ******************************************************************************


Includes ------------------------------------------------------------------*/
#include "main.h"

int order=0;
int key_number[8]={0,0,0,0,0,1,5,0};
ideal_speed_struct ideal_speed={0,62.5,0,0};
real_speed_struct real_speed={0,0,0,0};
int Key_Num_Temp=20;

PIDTypedef SpeedPid = {
    SPEED_Kp,
    SPEED_Ti,
    SPEED_Td,
    
    0,
    0,
        
    0,
    0,
    
    0
};

float pow_f(float x)
{
    if(x < 0) return -x;
    return x;
}
void Control(PIDTypedef *pid,float NowValu,float *ExcValu)
{
    if(*ExcValu >  Speed_max) { 
        *ExcValu = Speed_max;
       key_number[5] = (int)(Speed_max*2.4)/100; 
       key_number[6] = (int)(Speed_max*2.4)%100/10; 
       key_number[7] = (int)(Speed_max*2.4)%10;
    }
    if(*ExcValu <  -Speed_max) { 
        *ExcValu = -Speed_max;
       key_number[5] = (int)(Speed_max*2.4)/100; 
       key_number[6] = (int)(Speed_max*2.4)%100/10; 
       key_number[7] = (int)(Speed_max*2.4)%10;
    }
    //计算偏差
    pid->err = *ExcValu - NowValu;
    //if(pid->err > 100) pid->err=0;
    //if(pid->err < -100) pid->err=0;
    //计算积分
    pid->inte += pid->err;
    if(pid->inte > Inte_Count) pid->inte = Inte_Count;
    if(pid->inte < -Inte_Count) pid->inte = -Inte_Count;
    //计算微分
    pid->diff = pid->err - pid->err_last;
    //计算输出
    pid->outvalu = pid->Kp * pid->err + pid->Ti * pid->inte + pid->Td * pid->diff;
    
    //给上次偏差
    pid->err_last = pid->err;
    
    if(pid->outvalu > 1000)
    {
        pid->outvalu = 1000;
    }else if(pid->outvalu < -1000)
    {
        pid->outvalu = -1000;
    }
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{      
	USART_ClearFlag(USART3,USART_FLAG_TC);
	USART3->DR = (u8) ch; 
	while((USART3->SR&0X40)==0);//循环发送,直到发送完毕            
	return ch;
}

int main(void)
{ 
	KeyBoard_Init();
	LCDInit();
	Init_NVIC_PriorityGroupConfig();
	Scan_TIM_NVIC_Config();
	Scan_TIM_config();
	Motor_Config();
	PWM_TIM_Init(999,216);
	EncoderOne_TIM_Config();
	EncoderTwo_TIM_Config();
	USART_Init_Config(115200);
	while(1)
	{		
		Key_Num_Temp=Read_KeyValue();
		Key_Num_deal(&order,Key_Num_Temp,key_number,&ideal_speed);
		display_set_speed(order,DELAY_MS,key_number);
		display_real_speed(&real_speed);
 		speed_control(&ideal_speed);
	}
}



