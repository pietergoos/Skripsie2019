#include "main.h"
#include "stm32f4xx_hal_conf.h"
#include "usb_host.h"
#include "fatfs.h"

extern UART_HandleTypeDef huart1;
extern ADC_HandleTypeDef hadc1;
extern SPI_HandleTypeDef hspi5;
extern I2C_HandleTypeDef hi2c1;
extern I2S_HandleTypeDef hi2s3;
extern uint8_t volString;
extern uint8_t bpmString;
extern ApplicationTypeDef Appli_state;
extern FATFS USBHFatFS;

#define USBH_USR_FS_INIT    ((uint8_t)0x00)
#define USBH_USR_AUDIO      ((uint8_t)0x01)

typedef struct
{
  uint32_t   ChunkID;       /* 0 */
  uint32_t   FileSize;      /* 4 */
  uint32_t   FileFormat;    /* 8 */
  uint32_t   SubChunk1ID;   /* 12 */
  uint32_t   SubChunk1Size; /* 16*/
  uint16_t   AudioFormat;   /* 20 */
  uint16_t   NbrChannels;   /* 22 */
  uint32_t   SampleRate;    /* 24 */

  uint32_t   ByteRate;      /* 28 */
  uint16_t   BlockAlign;    /* 32 */
  uint16_t   BitPerSample;  /* 34 */
  uint32_t   SubChunk2ID;   /* 36 */
  uint32_t   SubChunk2Size; /* 40 */

}WAVE_FormatTypeDef;

typedef enum
{
  BUFFER_OFFSET_NONE = 0,
  BUFFER_OFFSET_HALF,
  BUFFER_OFFSET_FULL,
}BUFFER_StateTypeDef;

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

void fillDMAArr();
void primeCol(uint8_t col);
uint8_t initFiles();

