#include "xMAX6675.h"
#include <SPI.h>

// SPI clock frequency dependend on processor clock frequency, max clock 4 MHz
#if F_CPU > 16000000UL
  #define SPI_CLOCK_DIV SPI_CLOCK_DIV8
#elif F_CPU > 8000000UL
  #define SPI_CLOCK_DIV SPI_CLOCK_DIV4
#else
  #define SPI_CLOCK_DIV SPI_CLOCK_DIV2
#endif 

bool SpiInitialized = false;

void xMAX6675::attach(int MOSI, int MISO, int SCK, int SSn) {
 pSSn = SSn;
 pinMode(pSSn, OUTPUT);
 digitalWrite(pSSn, HIGH);             //Start de-selected

 if (SpiInitialized == false) {
    //SPI pin setup
  pinMode(MOSI, OUTPUT);
  pinMode(MISO, INPUT);
  pinMode(SCK, OUTPUT);

    //SPI configuration
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);
  SPI.setClockDivider(SPI_CLOCK_DIV);   //Set SPI clock freq
  SPI.begin();                          //initialize SPI
  SpiInitialized = true;
 }
}

double xMAX6675::readC(void) {
 unsigned int tcData;
     
    //read the temperature
 digitalWrite(pSSn, LOW);
 tcData = SPI.transfer(0x00) << 8;
 tcData |= SPI.transfer(0x00);
 digitalWrite(pSSn, HIGH);

 if (tcData & 0x0004) {                
  return NAN;
 } else {
  return (tcData >> 3) / 4.0;         //calculate deg C
 }
}
