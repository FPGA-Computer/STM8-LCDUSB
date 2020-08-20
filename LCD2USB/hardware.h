/*
 * hardware.h
 *
 * Created: October 31, 2019, 12:33:01 PM
 *  Author: K. C. Lee
 * Copyright (c) 2019 by K. C. Lee
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.

 If not, see http://www.gnu.org/licenses/gpl-3.0.en.html 
 */ 

#ifndef HARDWARE_H_
#define HARDWARE_H_

#ifndef __CSMC__
#define __CSMC__
#endif
#ifndef STM8S003
#define STM8S003
#endif

#include "stm8s.h"
#include <stdio.h>

// STM8S003F3P6
enum _PA { PA1=0x02, PA2=0x04, PA3=0x08 };
enum _PB { PB4=0x10, PB5=0x20 };
enum _PC { PC3=0x08, PC4=0x10, PC5=0x20, PC6=0x40, PC7=0x80 };
enum _PD { PD1=0x02, PD2=0x04, PD3=0x08, PD4=0x10, PD5=0x20, PD6=0x40 };

#define GPIO(X)					((GPIO_TypeDef *)(X))

#define CPU_CLOCK				16000000UL
#define EE_Addr					0x4000

#define LCD_BIAS_PORT		GPIOA
#define LCD_BIAS_PIN		PA3

enum _OPT2 { AFR0 = 0x01, AFR1=0x02, AFR2=0x04, AFR3=0x08, AFR4=0x10, AFR5=0x20, AFR6=0x40, AFR7 = 0x80 };
#define OPTION_BYTE			AFR0

#define TIM2_PRSC				4
#define TIM2_FREQ				400
#define TIM2_Slow				100
#define TIM2_CNTMAX			4
#define TIM2_RELOAD			(CPU_CLOCK/(1<<TIM2_PRSC)/TIM2_FREQ)

// fake output for PC[5..3]

// Control PC5
#define Clr_PC5()				TIM2->CCMR1 = (0x4<<4)
#define Set_PC5()				TIM2->CCMR1 = (0x5<<4)
#define Clr_PC4()				TIM1->CCMR4 = (0x4<<4)
#define Set_PC4()				TIM1->CCMR4 = (0x5<<4)
#define Clr_PC3()				TIM1->CCMR3 = (0x4<<4)
#define Set_PC3()				TIM1->CCMR3 = (0x5<<4)

// LCD GPIO mapping
#define Clr_LCD_E()			Clr_PC5()
#define Set_LCD_E()			Set_PC5()
#define Clr_LCD_RW()		Clr_PC4()
#define Set_LCD_RW()		Set_PC4()
#define Clr_LCD_RS()		Clr_PC3()
#define Set_LCD_RS()		Set_PC3()

#define LCD_DATA_PORT		GPIOD
#define LCD_DATA				(PD4|PD3|PD2|PD1)
#define LCD_DOFFSET			1
#define KEY_DOFFSET			(LCD_DOFFSET+1)

extern volatile uint8_t TIM2_Update_Flag;

void Init_Hardware(void);
void RESET_CHIP(void);
void Check_OPTION_BYTE(void);
void TIM2_Init(void);

void LCD_Init(void);
void Delay(uint16_t t);
uint8_t Key_Get(void);

// Delays in us.  The delay has been calibrated for 16MHz
// Maximum delay is about 100ms (16-bit)

#define DELAY_TWEAK			0.63
#define US_DELAY(X)			((uint16_t)(X*DELAY_TWEAK))

typedef struct
{
	uint8_t code;
	uint8_t count;
} key_t;

extern key_t key;

void KeyScan(void);

#define KEY_REPEAT_RATE		4
#define KEY_THRESHOLD			2
#define KEY_RELOAD				50
#define KEY_CNT_MAX				(KEY_RELOAD+TIM2_FREQ/TIM2_CNTMAX/2/KEY_REPEAT_RATE)


enum keys { KEY_MENU=0x01, KEY_DEC=0x02, KEY_INC=0x04, KEY_MASK= KEY_MENU|KEY_DEC|KEY_INC, 
						KEY_LONG=0x20, KEY_REPORTED=0x40, KEY_FLAG=0x80 };
						
#endif
