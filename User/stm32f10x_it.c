/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "main.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */
extern real_speed_struct real_speed;
extern ideal_speed_struct ideal_speed;
extern int key_number[8];
int DELAY_MS;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}


void EXTI9_5_IRQHandler (void)
{ 
}

void TIM3_IRQHandler()
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
		DELAY_MS++;
		if(DELAY_MS==50000) DELAY_MS=0;
		speed_get(&real_speed,&ideal_speed,DELAY_MS);
		
	}
}
int chartoint(unsigned char a)
{
	switch(a)
	{
		case '0': return 0;
		case '1': return 1;
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;
		case '+': return 11;
		case '-': return 12;
	}
	return 10;
}

void USART3_IRQHandler()
{
	u8 Res;
	static u8 set_char[8]={'0','0','0','0','0','0','0','0'};
	static int n=0;
    double Kr=Speed_max*2.4/50; int Kl=20;
	int m;
	int set_number=0;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是# R L A 结尾)
	{		
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		Res =USART_ReceiveData(USART3);	//读取接收到的数据
		if(Res!='#'){			
			set_char[n]=Res;
			n++;
			if(n>=7) n=7;
		}
		if(Res=='#')
		{	
			switch(set_char[n-1]){
			
            case 'L':							
				for(m=0;m<n-1;m++)
					set_number+=chartoint(set_char[m])*(int)pow(10,n-m-2);
				if(set_number>=99) set_number=99;
				if(set_number<=1) set_number=1;
				set_number-=50;
				if(set_number<0) { set_number=0-set_number;  key_number[0]=12;}
					else { key_number[0]=11;}
					set_number*=Kl;
				key_number[1]=set_number/100;
				key_number[2]=set_number%100/10;
				key_number[3]=set_number%10;
				break;
					
			case 'R':	
				for(m=0;m<n-1;m++)
					set_number+=chartoint(set_char[m])*(int)pow(10,n-m-2);
				if(set_number>=99) set_number=99;
				if(set_number<=1) set_number=1;
				set_number-=50;
				if(set_number<0){set_number=0-set_number;  key_number[4]=12;}
					else{  key_number[4]=11;}
					set_number*=Kr;
				key_number[5]=set_number/100;
				key_number[6]=set_number%100/10;
				key_number[7]=set_number%10;
				break;
			
			case 'A':	
				if(set_char[n-2]=='0')
				{
					key_number[0]=0;
                    key_number[1]=0;
                    key_number[2]=0;
                    key_number[3]=0;
                    key_number[4]=0;
                    key_number[5]=0;
                    key_number[6]=0;
                    key_number[7]=0;
				}
				else
				{
					for(m=0;m<4;m++)
					key_number[m]=9;
                    key_number[4]=0;
                    key_number[5]=2;
                    key_number[6]=5;
                    key_number[7]=0;
                    
				}
				break;			
			}
			n=0;
			ideal_speed.Left_Ward=(key_number[0]<=11);
			ideal_speed.Right_Ward=(key_number[4]<=11);			
			ideal_speed.Left_speed=key_number[1]*100+key_number[2]*10+key_number[3]/2.4;
			ideal_speed.Right_speed=key_number[5]*100+key_number[6]*10+key_number[7]/2.4;
		}
  } 
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
