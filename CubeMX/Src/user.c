#include "user.h"

uint8_t spiData[3];
uint8_t r;
uint32_t tickCtr;
uint32_t prevTick;
SPI_HandleTypeDef spi5;
uint16_t cData[5];

uint8_t t;

void userInit(){
	//c = 0b1010101010101010;
	//c = 0b1101000100010001;
	r = 0;
	tickCtr = 0;
	prevTick = HAL_GetTick();
	spi5 = getSPI5();

	cData[0] = 0b0000000000000000;
	cData[1] = 0b0000000000000000;
	cData[2] = 0b0000000000000000;
	cData[3] = 0b0000000000000000;
	cData[4] = 0b0000000000000000;

	startLEDs();

}

void userLoop(){

	//Do nothing between ticks
	while(HAL_GetTick() == prevTick);
	tickCtr++;

	if(tickCtr % 10 == 0){
		sendLED();
		r++;
		if(r >= 5){
			r = 0;
		}
	}
	//TODO: Check the systick time - this is 1.5s
	if (tickCtr >= 100000){
		t++;
		if(t == 16){
			t = 0;
		}
		incrCol(t);

		//once a second tick counter resets
		tickCtr = 0;
	}

}

void sendLED(){
	//Get Data
	spiData[1] = ((cData[r] & 0xFF00) >> 8);
	spiData[0] = (cData[r] & 0x00FF);
	spiData[2] = (0b10000000 >> r);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); //Enable Goes Low
	HAL_SPI_Transmit(&spi5, spiData, 3, 10); //Transmit 3 bytes
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); //Enable Goes High
}

void setLED(uint8_t row, uint8_t col, uint8_t val){
	/*
	 * row - value from 0 to 4
	 * col - value from 0 to 15
	 * val - 1 or 0 to set bit
	 */
	cData[row] = (cData[row] & ~(1 << (15-col))) | (val << (15-col));
}

void toggleLED(uint8_t row, uint8_t col){
	/*
	 * row - value from 0 to 4
	 * col - value from 0 to 15
	 */
	cData[row]  ^= 1 << (15-col);
}

void toggleLEDCol(uint8_t col){
	uint8_t i;
	for(i = 0; i<5; i++){
		toggleLED(i, col);
	}
}

void incrCol(uint8_t col){
	uint8_t cl = col-1;
	if (col == 0){
		cl = 15;
	}
	toggleLEDCol(col);
	toggleLEDCol(cl);
}

void startLEDs(){
	t = 0;
	toggleLEDCol(0);
}

void stopLEDs(){
	toggleLEDCol(t);
}
