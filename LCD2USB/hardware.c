/*
 * hardware.c
 *
 * Created: August 30, 2019, 6:35:08 PM
 *  Author: K. C. Lee
 * Copyright (c) 2016 by K. C. Lee
 
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

#include "hardware.h"
#include "usb.h"
#include "lcd.h"

volatile uint8_t TIM2_Update_Flag;
key_t key;

void Init_Hardware(void)
{	
	Check_OPTION_BYTE();
	CLK->CKDIVR = 0; // HSIDIV = 0; CPUDIV = 0
	USB_disconnect();
	TIM2_Init();
	USB_Init();
	Delay(US_DELAY(90000));
	LCD_Init();
	enableInterrupts();
	USB_connect();
}

#define IWDG_KEY_ENABLE 0xcc

void RESET_CHIP(void)
{
	IWDG->KR = IWDG_KEY_ENABLE; // Reset the CPU: Enable the watchdog and wait until it expires
	while(1);    // Wait until reset occurs from IWDG
}

void FLASH_Data_lock(uint8_t lock)
{
	if(lock)
		FLASH->IAPSR = 0;
	else
	{
		/* Warning: keys are reversed on data memory !!! */
    FLASH->DUKR = FLASH_RASS_KEY2;
    FLASH->DUKR = FLASH_RASS_KEY1;		
	}
}

void FLASH_Wait(void)
{
	while(!(uint8_t)(FLASH->IAPSR & (FLASH_IAPSR_EOP | FLASH_IAPSR_WR_PG_DIS)))
		/* wait*/;
}

void Check_OPTION_BYTE(void)
{
	uint8_t option_byte;

	if ((OPT->OPT2 != (uint8_t)(~OPT->NOPT2))||((OPT->OPT2 & AFR0) == 0)) 
	{ // check AFR0
		option_byte = OPT->OPT2|AFR0; 		// set AFR0 = 1 // PORT C[7..5] Alternate Function
		
		FLASH_Data_lock(0);
		FLASH->CR2 |= FLASH_CR2_OPT;
		FLASH->NCR2 &= (uint8_t)(~FLASH_NCR2_NOPT);

		OPT->OPT2 = option_byte;
		FLASH_Wait();

		OPT->NOPT2 = (uint8_t)(~option_byte);
		FLASH_Wait();
		
		// Disable write access to option bytes
		FLASH->CR2 &= (uint8_t)(~FLASH_CR2_OPT);
		FLASH->NCR2 |= FLASH_NCR2_NOPT;

		FLASH_Data_lock(1);
    RESET_CHIP();
	}
	
	if ((OPT->OPT3 != (uint8_t)(~OPT->NOPT3))||((OPT->OPT3 & HSITRIM) == 0)) 
	{ // check HSITRIM
		option_byte = OPT->OPT3|HSITRIM;			// 4 bit on-the-fly trimming
		
		FLASH_Data_lock(0);
		FLASH->CR2 |= FLASH_CR2_OPT;
		FLASH->NCR2 &= (uint8_t)(~FLASH_NCR2_NOPT);

		OPT->OPT3 = option_byte;
		FLASH_Wait();

		OPT->NOPT3 = (uint8_t)(~option_byte);
		FLASH_Wait();
		
		// Disable write access to option bytes
		FLASH->CR2 &= (uint8_t)(~FLASH_CR2_OPT);
		FLASH->NCR2 |= FLASH_NCR2_NOPT;
		FLASH_Data_lock(1);
    RESET_CHIP();
	}
}

void TIM2_Init(void)
{
	// TIM2_TimeBaseInit(TIM2_PRESCALER_16, 2500); // 16 MHz / 16 / 2500 = 400 Hz	
	TIM2->PSCR = TIM2_PRSC;
	TIM2->ARRH = TIM2_RELOAD >>8;
	TIM2->ARRL = TIM2_RELOAD & 0xff;
	
	//	TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
	TIM2->IER = TIM2_IER_UIE;

	// TIM2 enable
	TIM2->CR1 = TIM2_CR1_CEN;
}

// 400Hz IRQ handler
@far @interrupt void TIM2_Update_IRQ(void)
 {
	TIM2_Update_Flag = 1;
	TIM2->SR1 = (uint8_t)~TIM2_SR1_UIF;
 }

void LCD_Init(void)
{
	// PWM - LCD contrast control - charge pump
	// CH3: PWM mode 1, preload enable
	TIM2->CCMR3 = (0x06<<4)|TIM2_CCMR_OCxPE;
	TIM2->CCER2 |= TIM2_CCER2_CC3E;				// CH3 output enable
	
	// PC5: TIM2_CH1: fake output port by using PWM mode
	TIM2->CCER1 |= TIM2_CCER1_CC1E;				// CH1 output enable
	
	// PC3: TIM1_CH4: fake output port by using PWM mode
	// PC2: TIM1_CH3: fake output port by using PWM mode
	TIM1->BKR |= TIM1_BKR_MOE;						// Main output enable
	TIM1->CCER2 |= TIM1_CCER2_CC3E|TIM1_CCER2_CC4E;	// CH3, CH4 output enable
	
	LCD_DATA_PORT->CR1 |= LCD_DATA;				// Set push-pull/pull up
	
	LCD_Contrast(LCD_DEFAULT_CONTRAST);
	LCD_SetMode();
}

// Delay in us
void Delay(uint16_t t)
{
	for(;t;t--)
		LCD_Short_Delay();
}

void KeyScan(void)
{
	uint8_t new_code = (uint8_t)((~(LCD_DATA_PORT->IDR)>>KEY_DOFFSET)&KEY_MASK);

	if(new_code==(key.code & KEY_MASK))
	{
		if(new_code)
		{
			if(key.count == KEY_CNT_MAX)
			{	
				if(!(new_code&KEY_MENU))						// no auto-repeat for menu key
					key.count = (uint8_t)KEY_RELOAD;
				
				key.code |= KEY_LONG|KEY_FLAG;
			}
			else
				key.count++;
		}
	}
	else
	{
		if(!new_code&&key.code&&(key.count>KEY_THRESHOLD)&&!(key.code&(KEY_REPORTED|KEY_LONG)))
			key.code|=KEY_FLAG;
		else
		{
			key.code = new_code;
			key.count = 0;
		}
	}
}

uint8_t Key_Get(void)
{ 
  if(key.code & KEY_FLAG)
	{
		key.code = (uint8_t)((key.code & ~KEY_FLAG)|KEY_REPORTED);
		return(uint8_t)(key.code & (KEY_MASK|KEY_LONG));
	}
	return(0);
}

