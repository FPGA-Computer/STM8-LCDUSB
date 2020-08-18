/*
 * lcd.h
 *
 * Created: April-08-15, 3:10:36 PM
 *  Author: K. C. Lee
 * Copyright (c) 2017 by K. C. Lee
 
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
#ifndef LCD_H_
#define LCD_H_

#ifndef HARDWARE_H_
#include "hardware.h"
#endif

void LCD_Short_Delay(void);
void LCD_Cls(void);
void LCD_SetMode(void);
void LCD_WR_High_Nibble(uint8_t Code);
void LCD_WR(uint8_t LCD_data);
uint8_t LCD_RD(void);

// Internal functions
void LCD_Wait(void);
void LCD_Data(uint8_t LCD_Data);
void LCD_Cmd(uint8_t Cmd);
void LCD_Contrast(uint8_t Contrast);

void LCD_Moveto(uint8_t X, uint8_t Y);
void LCD_Ch(uint8_t Ch);
void LCD_UserCh(void);
void LCD_Puts(char *str);

// LCD commands
enum _LCD_CMD
{
	_LCD_CLR = 0x01,	
	_LCD_HOME	=	0x02,
	_LCD_ENTRY = 0x06,
	_LCD_ON	=	0x08,_LCD_DISPLAY_ON	= 0x04,_LCD_CSR_ON	= 0x02,_LCD_CSR_BLK	= 0x01,_LCD_CSR_UL = 0x00,
	_LCD_SHIFTLEFT = 0x10, _LCD_SHIFTRIGHT = 0x14,
	// 4-bit interface, 2 lines, 5x7 font
	_LCD_FuncSet = 0x28,	_LCD_FuncSetH	= _LCD_FuncSet & 0xf0,_LCD_Set8bit = 0x30,
	_LCD_CG = 0x40, _LCD_DD = 0x80
};

#define LCD_CUSTOM_CHR
//enum LCD_Char {  };

#define LCD_Addr(X,Y) ((X)|((Y)<<_LCD_YSHIFT))

// LCD constants
#define _LCD_BUSY						0x80

#define	_LCD_MX0						0x0f
#define	_LCD_MX1						0x4f

#define _LCD_Y1         		0x40

#define _LCD_XMASK      		0x0f
#define _LCD_YMASK      		0x01
#define _LCD_YSHIFT     		0x06

#define _LCD_INIT_DELAY1		US_DELAY(4100)
#define _LCD_INIT_DELAY2		US_DELAY(100)

#define _LCD_CMD_DELAY			US_DELAY(1700)
#define _LCD_CD_DELAY				US_DELAY(40)

// compensate for the 1K series resistor on SWIM
#define _LCD_DATABUS_DELAY	5
#define LCD_DEFAULT_CONTRAST	70

#endif
