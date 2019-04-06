#include "main.h"
#include "stm32f4xx_hal_conf.h"

extern UART_HandleTypeDef huart1;
extern ADC_HandleTypeDef hadc1;
extern SPI_HandleTypeDef hspi5;

void userInit();
void userLoop();
void sendLED();
void setLED(uint8_t row, uint8_t col, uint8_t val);
void toggleLED(uint8_t row, uint8_t col);
void toggleLEDCol(uint8_t col);
void incrCol(uint8_t col);
void startLEDs();
void stopLEDs();
void initBtns();
void incrBtns();
void resetBtns();

