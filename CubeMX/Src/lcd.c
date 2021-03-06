/*
 * lcd.c
 *
 *  Created on: 08 Apr 2019
 *      Author: Pieter Goos
 *      Adapted from L Visagie
 */

#include "lcd.h"

uint32_t lcdState;
uint32_t lcdDelay;
uint32_t lcdStartTick;

uint32_t* B_odr_ptr;
uint32_t* D_odr_ptr;

uint8_t volString[8];
uint8_t bpmString[8];

uint32_t ShortDelay(uint16_t cycles);


void initLCD(void){
	lcdState = 1;
	lcdDelay = 20;

	lcdStartTick = HAL_GetTick();

	B_odr_ptr = (uint32_t*) 0x40020414;
	D_odr_ptr = (uint32_t*) 0x40020C14;
}

void lcdPushNibble(uint8_t RS, uint8_t data){
	uint32_t odr = *B_odr_ptr;
	uint32_t odr_LowE, odr_HighE;

	//E and R/W LOW + RS = RS
	odr_LowE = odr & 0x4FFF;
	odr_LowE |= ((uint32_t)RS << 15);

	//E High; R/W LOW; RS=RS
	odr_HighE = odr_LowE + 0x2000;

	//Set and Delay lowE
	*B_odr_ptr = odr_LowE;
	ShortDelay(1);

	//Set and Delay highE
	*B_odr_ptr = odr_HighE;
	ShortDelay(1);

	//Data portion
	odr = *D_odr_ptr;
	odr &= 0xF0FF;
	odr += ((data & 0xF) << 8);

	//Set Data
	*D_odr_ptr = odr;
	ShortDelay(1);

	//lowE again
	*B_odr_ptr = odr_LowE;
	ShortDelay(1);
}

void lcdPushByte(uint8_t RS, uint8_t data){
	uint8_t temp;

	//High Nibble
	temp = data >> 4;
	lcdPushNibble(RS, temp);
	ShortDelay(50);

	//Low Nibble
	temp = data & 0x0F;
	lcdPushNibble(RS, temp);
	ShortDelay(50);
}

void updateLCD(void){
	uint32_t newTick;
	uint8_t chari;

	if (lcdState == 0){
		return;
	} else if(lcdState > 0){
		newTick = HAL_GetTick();
		if((newTick - lcdStartTick) > lcdDelay){
			switch(lcdState){
				//Initialization Sequence
				case 1:
					lcdPushNibble(0, 3);
					lcdDelay = 5;
					lcdStartTick = newTick;
					lcdState = 2;
					break;
				case 2:
					lcdPushNibble(0, 3);
					lcdDelay = 1;
					lcdStartTick = newTick;
					lcdState = 3;
					break;
				case 3:
					lcdPushNibble(0, 3);
					lcdDelay = 1;
					lcdStartTick = newTick;
					lcdState = 4;
					break;
				case 4:
					// 4 bit mode
					lcdPushNibble(0, 2);
					lcdDelay = 1;
					lcdStartTick = newTick;
					lcdState = 5;
					break;
				case 5:
					//4 bit mode, 2 lines
					lcdPushNibble(0, 2);
					lcdPushNibble(0, 8);
					lcdDelay = 1;
					lcdStartTick = newTick;
					lcdState = 6;
					break;
				case 6:
					//Disp on, cursor off
					lcdPushNibble(0, 0);
					lcdPushNibble(0, 0xE);
					lcdDelay = 1;
					lcdStartTick = newTick;
					lcdState = 7;
					break;
				case 7:
					//Clear Disp
					lcdPushNibble(0, 0);
					lcdPushNibble(0, 1);
					lcdDelay = 2;
					lcdStartTick = newTick;
					lcdState = 8;
					break;
				case 8:
					//Cursor Incr
					lcdPushNibble(0, 0);
					lcdPushNibble(0, 6);
					lcdDelay = 1;
					lcdStartTick = newTick;
					lcdState = 9;
					break;
				case 9:
					//Home
					lcdPushNibble(0, 8);
					lcdPushNibble(0, 0);
					lcdDelay = 1;
					lcdStartTick = newTick;
					lcdState = 10;
					break;
				case 10:
					lcdDelay = 100;
					lcdState = 20;
					break;
				//TEXT REFRESH
				case 20:
					//Home
					lcdPushNibble(0, 8);
					lcdPushNibble(0, 0);
					lcdDelay = 3;
					lcdStartTick = newTick;
					lcdState = 21;
					break;
				case 21:
					//Actual Text Stuff
					for(chari = 0; chari < 8; chari++){
						lcdPushByte(1, bpmString[chari]);
					}

					lcdPushByte(0, 192); //64+128 (DD RAM adr set)

					for(chari = 0; chari < 8; chari++){
						lcdPushByte(1, volString[chari]);
					}

					lcdDelay = 0;
					lcdState = 0;
					break;
			}
		}
	}
}

void lcdRefresh(uint16_t vol, uint16_t bpm){

	bpmString[0] = bpm/100;
	bpmString[1] = (bpm - bpmString[0]*100) / 10;
	bpmString[2] = (bpm - bpmString[0] * 100 - bpmString[1] * 10);

	bpmString[0] += 48;
	bpmString[1] += 48;
	bpmString[2] += 48;
	bpmString[3] = ' ';
	bpmString[4] = 'B';
	bpmString[5] = 'P';
	bpmString[6] = 'M';
	bpmString[7] = ' ';

	if(bpmString[1] == 48 && bpmString[0] == 48){
		bpmString[1] = ' ';
	}
	if(bpmString[0] == 48){
		bpmString[0] = ' ';
	}


	if(vol == 0){
		volString[0] = 'V';
		volString[1] = 'o';
		volString[2] = 'l';
		volString[3] = ':';
		volString[4] = 'M';
		volString[5] = 'u';
		volString[6] = 't';
		volString[7] = 'e';
	} else {
		volString[4] = vol/100;
		volString[5] = (vol - volString[4]*100) / 10;
		volString[6] = (vol - volString[4] * 100 - volString[5] * 10);

		volString[0] = 'V';
		volString[1] = 'o';
		volString[2] = 'l';
		volString[3] = ':';
		volString[4] += 48;
		volString[5] += 48;
		volString[6] += 48;
		volString[7] = '%';

		if(volString[5] == 48 && volString[4] == 48){
			volString[5] = ' ';
		}
		if(volString[4] == 48){
			volString[4] = ' ';
		}

	}

	if(lcdState == 0){
		lcdState = 20;
		lcdDelay = 1;
	}
}


#pragma GCC push_options
#pragma GCC optimize ("O0")

uint32_t ShortDelay(uint16_t cycles)
{
	int16_t i = 0;
	uint32_t val = 0;
	for (i = 0; i < cycles; i++)
	{
		val += sinf(i);
	}
	return val;
}

#pragma GCC pop_options


