/**
  ******************************************************************************
  * @file    lcd1602.c 
  * @author  Flame
  * @version 3.0
  * @date    07/06/2017
  * @brief   This file contains all function which are used for display
  ******************************************************************************/
	
#include "lcd1602.h"
#include "main.h"

uint8_t number[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
uint8_t sign[]={'+','-','_'};
uint8_t ideal_velocity_one[] ="IV1:";
uint8_t actual_velocity_one[]="AV1:";
uint8_t ideal_velocity_two[] ="IV2:";
uint8_t actual_velocity_two[]="AV2:";

void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void delay(int mm)
{
	int i= mm*2000; 
	while(i--); 
}

void LcdWriteCom(unsigned char com)	  //写入命令
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_10);        //	RS=0;
	GPIO_ResetBits(GPIOC,GPIO_Pin_11);    //RW=0;
	GPIO_Write(GPIOD,com);
	delay(10);
	GPIO_SetBits(GPIOC,GPIO_Pin_12);                  // LCDE=1;
	delay(10);
	GPIO_ResetBits(GPIOC,GPIO_Pin_12);
}

void LcdWriteData(unsigned char dat)			//写入数据
{
	GPIO_SetBits(GPIOC,GPIO_Pin_10); 
	GPIO_ResetBits(GPIOC,GPIO_Pin_11);    //RW=0;
	GPIO_Write(GPIOD,dat);
	delay(10);
	GPIO_SetBits(GPIOC,GPIO_Pin_12);                  // LCDE=1;
	delay(10);
	GPIO_ResetBits(GPIOC,GPIO_Pin_12);
}

void LcdSetCursor(uint32_t x, uint32_t y)
{
	uint32_t addr;

	if(y == 0) //由输入的屏幕坐标计算显示 RAM 的地址
	{
		addr = 0x00 + x; //第一行字符地址从 0x00 起始
	}
	else
	addr = 0x40 + x; //第二行字符地址从 0x40 起始
	LcdWriteCom(addr + 0x80); //设置 RAM 地址
}

void displayString(uint32_t x, uint32_t y, uint8_t * string, uint32_t length)
{
	LcdSetCursor(x, y);   

	while(length--)
	{
		LcdWriteData(*string++);
	}
}

void cleanScreen(void)
{
	LcdWriteCom(0x01);  //清屏
}

void displayNumber(uint32_t x, uint32_t y, uint32_t number)
{
	uint8_t buffer[10];
	uint8_t tempBuffer[10];
	uint32_t index = 0;
	uint32_t tempIndex = 0;

	if(number)
	{
	while(number)
	{
		buffer[index++] = number % 10 + '0';
		number /= 10;
	}

	index--;

	while(index)
	{
		tempBuffer[tempIndex++] = buffer[index--];
	}

	tempBuffer[tempIndex++] = buffer[index];

	displayString(x, y, tempBuffer, tempIndex);
	}else
	{
		displayString(x, y,"0", 1);
	}
}

void LcdInit()	 //LCD初始化子程序
{
	LcdWriteCom(0x38);  //设置显示模式
	LcdWriteCom(0x0c);  //开显示不显示光标，光标不闪烁
	LcdWriteCom(0x06);  //写一个指针加1
	LcdWriteCom(0x01);  //清屏
}

void LCDInit(void)
{
	GPIO_Config();
	LcdInit();
	displayString(0,0,ideal_velocity_one,4);
	displayString(8,0,actual_velocity_one,4);
	displayString(0,2,ideal_velocity_two,4); 
	displayString(8,2,actual_velocity_two,4);
}

void display_set_speed(int order,int DELAY_MS,int key_number[8])
{
	int order_if[]={0,0,0,0,0,0,0,0};
		
	if(DELAY_MS%50<25){
		switch(order)
		{
			case 0: order_if[0]=1; displayString(order+4,0,sign+2,1); break;
			case 1: order_if[1]=1; displayString(order+4,0,sign+2,1); break;
			case 2:	order_if[2]=1; displayString(order+4,0,sign+2,1); break;
			case 3:	order_if[3]=1; displayString(order+4,0,sign+2,1); break;
			
			case 4: order_if[4]=1; displayString(order+0,2,sign+2,1); break;
			case 5: order_if[5]=1; displayString(order+0,2,sign+2,1); break;
			case 6: order_if[6]=1; displayString(order+0,2,sign+2,1); break;
			case 7: order_if[7]=1; displayString(order+0,2,sign+2,1); break;
		}
	}//根据按键选择的不同位置进行定位闪烁 闪烁占空比为6/12
	
	//在未选择此位置时显示设定的数值
	if(order_if[0]==0){
		if(key_number[0]<=11) displayString(4,0,sign,1);
		else displayString(4,0,sign+1,1);
	}
	if(order_if[1]==0) displayString(5,0,number+key_number[1],1);	
	if(order_if[2]==0) displayString(6,0,number+key_number[2],1);
	if(order_if[3]==0) displayString(7,0,number+key_number[3],1);	
		
	if(order_if[4]==0){
		if(key_number[4]<=11) displayString(4,2,sign,1);
		else displayString(4,2,sign+1,1);
	}
	if(order_if[5]==0) displayString(5,2,number+key_number[5],1);
	if(order_if[6]==0) displayString(6,2,number+key_number[6],1);
	if(order_if[7]==0) displayString(7,2,number+key_number[7],1);	

}
int positive(int x)
{
    if(x < 0) x=-x;
    return x;
}
void display_real_speed(real_speed_struct *real_speed)
{	
    int Left_Speed , Right_Speed;
    Left_Speed = 2.4*positive(real_speed->Left_speed);
    Right_Speed = 2.4*positive(real_speed->Right_speed);
    
	displayString(12,0,sign + real_speed->Left_Ward,1);
	displayString(13,0,number + Left_Speed/100,1);
	displayString(14,0,number + Left_Speed%100/10,1);
	displayString(15,0,number + Left_Speed%10,1);
	
	displayString(12,2,sign + real_speed->Right_Ward,1);
	displayString(13,2,number + Right_Speed/100,1);
	displayString(14,2,number + Right_Speed%100/10,1);
	displayString(15,2,number + Right_Speed%10,1);
}
