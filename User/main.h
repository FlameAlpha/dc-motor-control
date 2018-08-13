#ifndef __MAIN_H__
#define __MAIN_H__

#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "delay.h"
#include "lcd1602.h"
#include "keyboard.h"
#include "user.h"
#include "stdio.h"	
#include "math.h"

typedef struct 
{
	float Left_speed;
	float Right_speed;
	int Left_Ward;
	int Right_Ward;
}ideal_speed_struct;	

typedef struct 
{
	float Left_speed;
	float Right_speed;
	int Left_Ward;
	int Right_Ward;
}real_speed_struct;	

typedef struct{
    float Kp;
    float Ti;
    float Td;
    
    float inte;
    float diff;
        
    float err;
    float err_last;
    
    float outvalu;
}PIDTypedef;

#define SPEED_Kp 1.1f
#define SPEED_Ti SPEED_Kp*0.2
#define SPEED_Td SPEED_Kp*1.25
#define Speed_max 250
#define Inte_Count 6000.0f

extern PIDTypedef SpeedPid;


extern void Key_Num_deal(int *order,int KEY_NUMBER,int key_number[8],ideal_speed_struct *ideal_speed);
extern void speed_control(ideal_speed_struct *ideal_speed);
extern void speed_get(real_speed_struct *real_speed,ideal_speed_struct *ideal_speed,int DELAY_MS);
extern void display_real_speed(real_speed_struct *real_speed);
extern void Control(PIDTypedef *pid,float NowValu,float *ExcValu);

extern int DELAY_MS;

#endif

