// Example
// 4x Thermocouple Shield for Arduino Uno

//MAX6675 SPI pin definitions
#define csTC0  7      //chip select pins for MAX6675
#define csTC1  8 
#define csTC2  9 
#define csTC3  10 

#define nTC 4 // number of thermocouples

#define tCONV 220    // conversion time is 0.22s per data sheet
#define nMEAS 8      // number of measurements to average
   
#include <xMAX6675.h>      

#define MOSI  11    //master out slave in (not connected)
#define MISO  12    //master in slave out
#define SCK   13    //serial clock


#ifdef MONITOR_BAUDRATE // given in Makefile 
#define SERIAL_SPEED MONITOR_BAUDRATE
#else
#define SERIAL_SPEED 9600
#endif

xMAX6675 tc[nTC]; // instantiate Thermocouple objects
double tempTC[nTC][nMEAS], tempTCAvg[nTC]; 

unsigned char MeasIdx;

unsigned long StartTime, MeasTime;
unsigned long time_s;

// average function
double Avg (double Vals[]) {

 unsigned char n;
 double Sum = 0.0;
// sum values   
 for (n = 0; n < nMEAS; n++) Sum += Vals[n];
 return (Sum/(double)nMEAS);
}

void setup() {

 unsigned char tcn;

 Serial.begin(SERIAL_SPEED);       //initialize Serial
 Serial.println("Thermocouple Shield Example");

 tc[0].attach(MOSI,MISO,SCK,csTC0);    //attach Thermocouples
 tc[1].attach(MOSI,MISO,SCK,csTC1);    
 tc[2].attach(MOSI,MISO,SCK,csTC2);    
 tc[3].attach(MOSI,MISO,SCK,csTC3);    

// do first nMEAS measurements to fill array
// measure 
 for (MeasIdx = 0; MeasIdx < nMEAS;MeasIdx++) {
   for (tcn = 0; tcn < nTC; tcn++) tempTC[tcn][MeasIdx] = tc[tcn].readC(); // measure thermocouple temperature
   delay(tCONV);
 }
 MeasIdx=0;
 MeasTime = millis();
 StartTime = MeasTime;

 time_s = 0;
}

void loop() {

 unsigned char tcn;


// calculate averages
 for (tcn=0; tcn<nTC;tcn++) tempTCAvg[tcn] = Avg(tempTC[tcn]);

// output
 if ((StartTime+1000*time_s) <= millis()) {

  Serial.print(time_s);Serial.print(";");

  for (tcn=0; tcn<nTC;tcn++) {
   Serial.print(tempTCAvg[tcn]);   //read the TC and print the temperature
   Serial.print(";");
  }
  Serial.println();
  time_s++;
 } 

// take next measurement
  
 for (tcn = 0; tcn < nTC; tcn++) {
  tempTC[tcn][MeasIdx] = tc[tcn].readC(); // measure thermocouple temperature
 }

 MeasIdx=(MeasIdx==nMEAS-1)?0:MeasIdx+1;
 MeasTime = millis();
 delay(tCONV);  
}


