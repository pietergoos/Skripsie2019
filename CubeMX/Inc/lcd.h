/*
 * lcd.h
 *
 *  Created on: 08 Apr 2019
 *      Author: Pieter Goos
 */

#include "stm32f4xx_hal.h"
#include <math.h>

#ifndef LCD_H_
#define LCD_H_

void initLCD(void);
void lcdPushNibble(uint8_t RS, uint8_t data);
void lcdPushByte(uint8_t RS, uint8_t data);
void updateLCD(void);
void lcdRefresh(uint16_t vol, uint16_t bpm);


#endif /* LCD_H_ */
