#include "user.h"

uint8_t spiData[3];
uint8_t r;
//uint16_t c;
uint32_t tickCtr;
uint32_t prevTick;
SPI_HandleTypeDef spi5;
uint16_t cData[5];

void userInit(){
	//c = 0b1010101010101010;
	//c = 0b1101000100010001;
	r = 0;
	tickCtr = 0;
	prevTick = HAL_GetTick();
	spi5 = getSPI5();

	cData[0] = 0b1010110100100010;
	cData[1] = 0b1010100100100101;
	cData[2] = 0b1110110100100101;
	cData[3] = 0b1010100100100101;
	cData[4] = 0b1010110110110010;

}

void userLoop(){

	while(HAL_GetTick() == prevTick);
	tickCtr++;

	if(tickCtr % 10 == 0){
		sendLED();
		r++;
		if(r >= 5){
			r = 0;
		}
	}

	if (tickCtr >= 100000){
		//once a second
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




