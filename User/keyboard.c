/**
  ******************************************************************************
  * @file    keyboard.c 
  * @author  Flame
  * @version 3.0
  * @date    07/06/2017
  * @brief   This file contains all function which are used for keyboard
  ******************************************************************************/
  
#include "keyboard.h"

void KeyBoard_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOE, &GPIO_InitStructure);  
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
		GPIO_Init(GPIOE, &GPIO_InitStructure);  
		GPIO_SetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3); 
		GPIO_ResetBits(GPIOE, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
}

int Read_KeyValue(void){
	int KeyValue=0;  
	if((GPIO_ReadInputData(GPIOE)&0xff)!=0x0f) 
	{
		int x=200;
		while(x--);
		if((GPIO_ReadInputData(GPIOE)&0xff)!=0x0f) 
		{
			GPIO_SetBits(GPIOE, GPIO_Pin_0);  
			GPIO_ResetBits(GPIOE, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3); 
			switch(GPIO_ReadInputData(GPIOE)&0xff) 
			{
				case 0x11: KeyValue = 1; break; 
				case 0x21: KeyValue = 5; break; 
				case 0x41: KeyValue = 9; break; 
				case 0x81: KeyValue = 13;  break;
			 }  
			GPIO_SetBits(GPIOE, GPIO_Pin_1);  
			GPIO_ResetBits(GPIOE,GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_3); 
			switch(GPIO_ReadInputData(GPIOE)&0xff) 
			{  
				case 0x12: KeyValue = 2; break; 
				case 0x22: KeyValue = 6; break; 
				case 0x42: KeyValue = 0;break; 
				case 0x82: KeyValue = 14;break;
			}  
			GPIO_SetBits(GPIOE, GPIO_Pin_2);  
			GPIO_ResetBits(GPIOE,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3); 
			switch(GPIO_ReadInputData(GPIOE)&0xff) 
			{  
				case 0x14: KeyValue = 3; break; 
				case 0x24: KeyValue = 7; break; 
				case 0x44: KeyValue = 11;break; 
				case 0x84: KeyValue = 15;break;
			}  
			GPIO_SetBits(GPIOE, GPIO_Pin_3);  
			GPIO_ResetBits(GPIOE,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2); 
			switch(GPIO_ReadInputData(GPIOE)&0xff) 
			{  
				case 0x18: KeyValue = 4; break; 
				case 0x28: KeyValue = 8; break; 
				case 0x48: KeyValue = 12;break; 
				case 0x88: KeyValue = 16;break;
			 }  
			GPIO_SetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3); 
			GPIO_ResetBits(GPIOE, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);  
			return KeyValue;
	  } 
} 
return 20; 
}

void Key_Num_deal(int *order,int KEY_NUMBER,int key_number[8],ideal_speed_struct *ideal_speed)
{
	static int over=1;
	static int tab=0; 
	int temp;
	if(KEY_NUMBER==20) over=1;
	else{
		if(over==1){			
			if(KEY_NUMBER==13) {
				if(tab==1) {*order=0; tab=0;}
				else {*order=4; tab=1;}
				over=0;
				return ;
			}		
			if(KEY_NUMBER==14)
			{	
				over=0;
				return ;
			}
			if(KEY_NUMBER==15)
			{ 
				if(tab==1){ 
				for(temp=4;temp<=7;temp++)
				{
					key_number[temp]=0;
				}
					*order=4;
				}
				else{
				for(temp=0;temp<=3;temp++)
				{
					key_number[temp]=0;
				}	
					*order=0;
				}
				over=0;
				return ;
			}
			if(KEY_NUMBER==16) 
			{				
				ideal_speed->Left_speed = (key_number[1] * 100 + key_number[2] * 10 + key_number[3])/2.4;        
				ideal_speed->Right_speed = (key_number[5] * 100 + key_number[6] * 10 + key_number[7])/2.4;
				ideal_speed->Left_Ward=(key_number[0]<=11);				
				ideal_speed->Right_Ward=(key_number[4]<=11);
                if(key_number[4] > 11) ideal_speed->Right_speed = - ideal_speed->Right_speed;    
				over=0;
				return ;			
			}	
				key_number[*order]=KEY_NUMBER; 
				*order=*order+1;
				if(tab==0){ if(*order>=3) *order=3;}					
				else{ if(*order>=7) *order=7;}
				over=0;
				return ;			
		}
	}
}
