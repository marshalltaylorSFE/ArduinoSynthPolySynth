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
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));

}

void loop()
{
	for( int n = 0; n <= 1; n++ )
	{
		for( int i = 0; i <= 7; i++ )
		{
			SPI.transfer(0x00);
			SPI.transfer(0x01 << i);
			digitalWrite(LCLKPin, 0);
			delay(1);
			digitalWrite(LCLKPin, 1);  
			delay(n*200+50);
			
		} 
		
		for( int i = 0; i <= 7; i++ )
		{
			SPI.transfer(0x01 << i);
			SPI.transfer(0x00);
			digitalWrite(LCLKPin, 0);
			delay(1);
			digitalWrite(LCLKPin, 1);  
			delay(n*200+50);
		} 
		delay(n*400+200);
	}
	//delay(1000);
	for(uint32_t n = 0; n < 100; n++)
	{
			for(int i = 0; i < 2; i++)
			{
				SPI.transfer(0xFE);
				SPI.transfer(0xFF);
				digitalWrite(LCLKPin, 0);
				delay(1);
				digitalWrite(LCLKPin, 1);  
			}
			for(int i = 0; i < 3; i++)
			{
				SPI.transfer(0x78);
				SPI.transfer(0x3F);
				digitalWrite(LCLKPin, 0);
				delay(1);
				digitalWrite(LCLKPin, 1);  
			}
			for(int i = 0; i < 6; i++)
			{
				SPI.transfer(0x30);
				SPI.transfer(0x0F);
				digitalWrite(LCLKPin, 0);
				delay(1);
				digitalWrite(LCLKPin, 1);  
			}
			for(int i = 0; i < 12; i++)
			{
				SPI.transfer(0x10);
				SPI.transfer(0x03);
				digitalWrite(LCLKPin, 0);
				delay(1);
				digitalWrite(LCLKPin, 1);  
			}
	}
	SPI.transfer(0xAA);//MSB
	SPI.transfer(0x55);
	digitalWrite(LCLKPin, 0);
	delay(1);
	digitalWrite(LCLKPin, 1);  	
	while(1);
}
