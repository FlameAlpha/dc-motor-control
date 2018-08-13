/**
  ******************************************************************************
  * @file    user.c 
  * @author  Flame
  * @version 3.0
  * @date    07/06/2017
  * @brief   This file contains all user's function 
  ******************************************************************************/
  
#include "user.h"

int R_wave_filter(int filter_num)
{
	static int wave[4]={0};
	int m; 
	float count=0;
	for(m=0;m<=2;m++) wave[m]=wave[m+1];
	wave[3]=filter_num;
	for(m=3;m>=0;m--)	count+=wave[m];
    count/=4;
	return count;
}
int L_wave_filter(int filter_num)
{
	static int wave[7]={0,0,0,0,0,0,0};
	int m,n,wave_temp; double count=0;
		
	for(m=0;m<=6;m++) wave[m]=wave[m+1];
	wave[6]=filter_num;
  for(m=6;m>=0;m--)
   for(n=0;n<=m;n++)
    if(wave[6-m]<wave[6-m+n])
		{ 
			wave_temp=wave[6-m]; 
			wave[6-m]=wave[6-m+n]; 
			wave[6-m+n]=wave_temp; 
		} 
	for(m=6;m>=0;m--)	count+=wave[m];
	count/=7;
	wave[2]=count;
	return count;
}
void Init_NVIC_PriorityGroupConfig(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}
void Scan_TIM_NVIC_Config(void)
{
	 NVIC_InitTypeDef NVIC_InitStructure; 
	 NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	 NVIC_Init(&NVIC_InitStructure);
}
void Scan_TIM_config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	TIM_TimeBaseStructure.TIM_Prescaler=7200-1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period=10;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_Trigger,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
}

//TIM4 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数

void PWM_TIM_Init(u16 arr,u16 psc)
{    
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//AFIO复用设置
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE); 
		GPIO_PinRemapConfig(GPIO_Remap_TIM4 , ENABLE); //Timer4部分重映射   
	 
		 //设置该引脚为复用输出功能,输出TIM4 CH1-4的PWM脉冲波GPIOD 12-15
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15; //TIM_CH2 GPIO_Pin_12|GPIO_Pin_13|
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIO
	 
		 //初始化TIM4
		TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
		TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
		
		//初始化TIM4 Channel 1-4 PWM模式	 
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
		
//		TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC1
//		TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC2
		TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC3
		TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC4
		
//		TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR1上的预装载寄存器
//		TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR2上的预装载寄存器
		TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR3上的预装载寄存器
		TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR4上的预装载寄存器
		
		TIM_Cmd(TIM4, ENABLE);  //使能TIM4	
		
//		TIM_SetCompare1(TIM4,500);	//设置占空比为0	
//		TIM_SetCompare2(TIM4,500);	//设置占空比为0
		TIM_SetCompare3(TIM4,0);		//设置占空比为0	
		TIM_SetCompare4(TIM4,0);		//设置占空比为0
}

void Motor_Config(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	 GPIO_Init(GPIOC, &GPIO_InitStructure);
	 GPIO_SetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_8);
	 GPIO_ResetBits(GPIOC,GPIO_Pin_7|GPIO_Pin_9);
}

void USART_Init_Config(int bound)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
		
	  //USART3_TX   GPIOB10
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
		GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB10
		 
		//USART3_RX	  GPIOB11初始化
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
		GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB11  

		//Usart3 NVIC 配置
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级2
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级2
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
		
		 //USART 初始化设置

		USART_InitStructure.USART_BaudRate = bound;//串口波特率
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
		USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

		USART_Init(USART3, &USART_InitStructure); //初始化串口3
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
		USART_Cmd(USART3, ENABLE);                    //使能串口3 
}
void EncoderOne_TIM_Config(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_ICInitTypeDef TIM_ICInitStructure;   	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO , ENABLE);		
		
		GPIO_StructInit(&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;        
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);                           
		
		TIM_DeInit(TIM5);
		TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
		TIM_TimeBaseStructure.TIM_Period = Count_Max;  
		TIM_TimeBaseStructure.TIM_Prescaler = 0; 
		TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;    		
		TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);              
									 
		TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge , TIM_ICPolarity_BothEdge);			
		TIM_ICStructInit(&TIM_ICInitStructure);
		TIM_ICInitStructure.TIM_ICFilter = 6;  
		TIM_ICInit(TIM5, &TIM_ICInitStructure);
		
		TIM_ClearFlag(TIM5, TIM_FLAG_Update);
		TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
		
		TIM_SetCounter(TIM5, 0);
		TIM_Cmd(TIM5, ENABLE);
}

void EncoderTwo_TIM_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOE,&GPIO_InitStructure);

	TIM_DeInit(TIM1);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = Count_Max;
	TIM_TimeBaseStructure.TIM_Prescaler = 0; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12,TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);
	TIM_ICStructInit(&TIM_ICInitStructure);  
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM1, &TIM_ICInitStructure);
	TIM_ClearFlag(TIM1, TIM_FLAG_Update); 

	TIM_SetCounter(TIM1, 0);
	TIM_Cmd(TIM1, ENABLE);
}
void speed_control(ideal_speed_struct *ideal_speed)
{
	static int Left_ward=0;
//	static int Right_ward=0;
	if(ideal_speed->Left_Ward!=Left_ward){
		GPIO_WriteBit(GPIOC, GPIO_Pin_6,(BitAction)((1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_6)))); 
		GPIO_WriteBit(GPIOC, GPIO_Pin_7,(BitAction)((1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_7)))); 
		Left_ward=ideal_speed->Left_Ward;
	}
	
//	if(ideal_speed->Right_Ward!=Right_ward){
//		GPIO_WriteBit(GPIOC, GPIO_Pin_8,(BitAction)((1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_8)))); 
//		GPIO_WriteBit(GPIOC, GPIO_Pin_9,(BitAction)((1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_9))));
//		Right_ward=ideal_speed->Right_Ward;
//	}
	
	TIM_SetCompare3(TIM4,ideal_speed->Left_speed);	
//	TIM_SetCompare4(TIM4,999-ideal_speed->Right_speed);	
}


void SetPwm(int motor)
{
	if(motor > 0)
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_8);
		GPIO_ResetBits(GPIOC,GPIO_Pin_9);
	}else{
		motor = - motor;
		GPIO_SetBits(GPIOC,GPIO_Pin_9);
		GPIO_ResetBits(GPIOC,GPIO_Pin_8);
	}
	
	TIM_SetCompare4(TIM4,motor);
}

void speed_get(real_speed_struct *real_speed,ideal_speed_struct *ideal_speed,int DELAY_MS)
{
	if(DELAY_MS%250==0){
        
  		real_speed->Left_speed = TIM1->CNT;
		real_speed->Right_speed = R_wave_filter(TIM5->CNT);
        if(real_speed->Right_speed > 250 ) real_speed->Right_speed=0;
        if(real_speed->Right_speed < -250 ) real_speed->Right_speed=0;
        if(real_speed->Left_speed > Count_Max/2 ) real_speed->Left_speed = real_speed->Left_speed - Count_Max;
        if(real_speed->Right_speed > Count_Max/2 ) real_speed->Right_speed = real_speed->Right_speed - Count_Max;
        
		TIM1->CNT=0;
		TIM5->CNT=0;
		
		Control(&SpeedPid,real_speed->Right_speed,&(ideal_speed->Right_speed));
		SetPwm(SpeedPid.outvalu);
	}
	if(real_speed->Left_speed > 0) real_speed->Left_Ward = 0;
	else real_speed->Left_Ward = 1;
	if(real_speed->Right_speed > 0) real_speed->Right_Ward = 0; 
	else real_speed->Right_Ward = 1;
	
	if(DELAY_MS%250==0) printf("%dL%dR#",(int)(real_speed->Left_speed+400),(int)(real_speed->Right_speed+400));
}
