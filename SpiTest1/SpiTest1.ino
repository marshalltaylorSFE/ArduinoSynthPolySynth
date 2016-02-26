#include <SPI.h>  // include the SPI library:

const int nOEPin = 33;
const int LCLKPin = 32;
const int nRSTPin = 26;
const int SPI_CLK = 14;
const int SPI_MOSI = 7;

int8_t loopCount = 0;
int8_t maxLoopCount = 20;
void setup() {
  // set the slaveSelectPin as an output:
  pinMode (nOEPin, OUTPUT);
  pinMode (LCLKPin, OUTPUT);
  pinMode (nRSTPin, OUTPUT);
  
  digitalWrite(nOEPin, 0);
  digitalWrite(LCLKPin, 1);
  digitalWrite(nRSTPin, 1);
  
  // initialize SPI:
  SPI.setMOSI(SPI_MOSI);
  SPI.setSCK(SPI_CLK);
  SPI.begin();

}

void loop()
{
  //Clear the display
  for( int i = 0; i <= 7; i++ )
  {
    SPI.transfer(0x00);
	SPI.transfer(0x01 << i);
	digitalWrite(LCLKPin, 0);
	delay(3);
	digitalWrite(LCLKPin, 1);  
	delay(250);
	
  } 

  for( int i = 0; i <= 7; i++ )
  {
	SPI.transfer(0x01 << i);
    SPI.transfer(0x00);
	digitalWrite(LCLKPin, 0);
	delay(3);
	digitalWrite(LCLKPin, 1);  
	delay(250);
  } 
  delay(1000);
  
}
