#include "user.h"

uint8_t spiData[3];
uint16_t c;
uint8_t r;
uint32_t tickCtr;
uint32_t prevTick;

void userInit(){
	c = 0b1010101010101010;
	r = 2;
	tickCtr = 0;
	prevTick = HAL_GetTick();
}

void userLoop(){

	while(HAL_GetTick() == prevTick);
	tickCtr++;

	if (tickCtr >= 100000){
		c = ~c;
		sendLED();
		//once a second
		r++;
		if(r == 6){
			r = 1;
		}

		tickCtr = 0;
	}

}

void sendLED(){
	//TODO: Add multiple Row printing in one go, not row by row (for loop?)
	spiData[0] = ~((c & 0xFF00) >> 8);
	spiData[1] = ~(c & 0x00FF);
	spiData[2] = (1 << (r-1));

	SPI_HandleTypeDef spi5 = getSPI5();

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&spi5, spiData, 3, 10);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
}




