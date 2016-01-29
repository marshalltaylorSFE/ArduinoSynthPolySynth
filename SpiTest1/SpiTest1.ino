#include <SPI.h>  // include the SPI library:

const int nOEPin = 4;
const int LCLKPin = 2;
const int nRSTPin = 3;
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
  for( int i = 0; i < 11; i++ )
  {
    SPI.transfer(0xFF);
  } 

  for( int i = 0; i < (11-loopCount); i++ )
  {
    SPI.transfer(0xFF);
  }
  
  //SPI.transfer(0xC6);
  //SPI.transfer(0x88);
  //SPI.transfer(0x92);
  //SPI.transfer(0x86);
  //SPI.transfer(0x99);

  
  SPI.transfer(0x80);
  SPI.transfer(0x99);
  SPI.transfer(0x88);
  SPI.transfer(0xC0);
  SPI.transfer(0xC8);
  
  for( int i = 0; i < (maxLoopCount - loopCount); i++ )
  {
    SPI.transfer(0xFF);
  }
  digitalWrite(LCLKPin, 0);
  delay(3);
  digitalWrite(LCLKPin, 1);   
  
  loopCount--;
  if( loopCount < 0 )
  {
	  loopCount = maxLoopCount;
  }
  delay(450);
  
}
