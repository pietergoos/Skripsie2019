#include "user.h"

#define ONESEC 67000

uint8_t spiData[3];
uint8_t r;
uint32_t tickCtr;
uint32_t prevTick;
uint16_t cData[5];
uint8_t cBtns[5][16];
uint8_t txBuff[50];

uint32_t adc[2], adcDMA[2];

uint8_t t;
uint8_t tm;

void userInit(){
	r = 0;
	tickCtr = 0;
	prevTick = HAL_GetTick();

	cData[0] = 0b0000000000000000;
	cData[1] = 0b0000000000000000;
	cData[2] = 0b0000000000000000;
	cData[3] = 0b0000000000000000;
	cData[4] = 0b0000000000000000;

	initBtns();
	startLEDs();

	HAL_UART_Transmit_DMA(&huart1, txBuff, 17);

	HAL_ADC_Start_DMA(&hadc1, adcDMA, 2);

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

	//Resets on high (PE11) /\ Clk on PE12
	if (tickCtr % (ONESEC/160) == 0){

		incrBtns();

	}

	if (tickCtr >= ONESEC){
		t++;
		if(t == 16){
			t = 0;
		}
		incrCol(t);

		sprintf(txBuff, "A: %4u B: %4u\r\n", adc[0], adc[1]);
		HAL_UART_Transmit_DMA(&huart1, txBuff, 17);

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
	HAL_SPI_Transmit(&hspi5, spiData, 3, 10); //Transmit 3 bytes
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

void initBtns(){
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET);
	tm = 0;
	resetBtns();
}

void incrBtns(){
	tm++;
	if(tm == 16){
		tm = 0;
	}
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET);

	GPIO_TypeDef* letter[] = {GPIOE, GPIOE, GPIOE, GPIOE, GPIOC};
	uint16_t number[] = {GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_4};
	uint8_t i;

	for (i = 0; i<5; i++){
		if(cBtns[i][tm] != HAL_GPIO_ReadPin(letter[i], number[i])){
			//Change
			if(cBtns[i][tm] == 0){
				//Update the LED
				//setLED(0, tm, ~cBtns[0][tm]);
				toggleLED(i, tm);
			}
			//Change the value of the array
			cBtns[i][tm] = ~cBtns[i][tm];
		}
	}
}

void resetBtns(){
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	adc[0] = adcDMA[0];
	adc[1] = adcDMA[1];
}

void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart)
{

}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

}

