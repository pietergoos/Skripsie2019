#include <avr/interrupt.h>
#include <util/delay.h>

#define LatchPin  10
#define DataPin   11
#define ClockPin  13

int r = 1;
int c = 1;

void setup() {
  // put your setup code here, to run once:
  pinMode(ClockPin,OUTPUT);
  pinMode(LatchPin,OUTPUT);
  pinMode(DataPin,OUTPUT);
  
  setupSPI();
  timer0_setup();
  sei();
}

void loop() {
  _delay_ms(500);
  c++;
  if (c == 17) {
    c = 1;
  }
}

ISR(TIMER2_COMPA_vect){
  PORTB &= ~0x04;
  // c8-15 ; c0-7 ; r0-r5
  spi_transfer(c & 0xFF00);
  spi_transfer(c & 0x00FF);
  spi_transfer((1 << r));
  PORTB |= 0x04;  
}

void timer0_setup()
{
  TCCR2A = 0x02;    //Use Timer0 in CTC(Clear Timer On Compare Match) mode
  TCCR2B = 0x07;    //Use the system clock(16MHz) with a prescaler of 1024
  OCR2A = 10;       //Set the match value (10 gives a delay of 640uS)
  TIMSK2 = 0x02;    //Enables the CTC A interrupt
}

byte spi_transfer(byte data)
{
  SPDR = data;        // Start the transmission
  while (!(SPSR & (1<<SPIF)));    // Wait the end of the transmission
  return SPDR;        // return the received byte, we don't need that
}

void setupSPI()
{
  SPCR |= ( (1<<SPE) | (1<<MSTR) ); // enable SPI as master
  SPCR &= ~( (1<<SPR1) | (1<<SPR0) ); // clear prescaler bits
  SPSR |= (1<<SPI2X); // set prescaler bits
}
