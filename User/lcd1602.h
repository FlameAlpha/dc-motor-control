#ifndef _LCD_H
#define _LCD_H

#include "stm32f10x.h"

void LCDInit(void);
void displayNumber(uint32_t x, uint32_t y, uint32_t number);
void displayString(uint32_t x, uint32_t y, uint8_t * string, uint32_t length);
void displayPass(uint32_t x, uint32_t y, uint32_t length);
void cleanScreen(void);
void display_set_speed(int order,int tab_num,int key_number[8]);
#endif
