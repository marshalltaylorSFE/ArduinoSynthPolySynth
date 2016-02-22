#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveformSine   sine3;          //xy=176.88888549804688,208
AudioSynthWaveformSine   sine1;          //xy=177.88888549804688,112
AudioSynthWaveformSine   sine2;          //xy=177.88888549804688,160
AudioSynthWaveformSine   sine4;          //xy=177.88888549804688,258
AudioOutputI2SQuad       i2s_quad1;      //xy=454.888916015625,147
AudioConnection          patchCord1(sine3, 0, i2s_quad1, 2);
AudioConnection          patchCord2(sine1, 0, i2s_quad1, 0);
AudioConnection          patchCord3(sine2, 0, i2s_quad1, 1);
AudioConnection          patchCord4(sine4, 0, i2s_quad1, 3);
AudioControlSGTL5000     sgtl5000_1;     //xy=429.888916015625,226
AudioControlSGTL5000     sgtl5000_2;     //xy=429.888916015625,271
// GUItool: end automatically generated code

const int nOEPin = 33;
const int LCLKPin = 32;
const int nRSTPin = 26;
const int SPI_CLK = 14;
const int SPI_MOSI = 7;

int8_t loopCount = 0;
int8_t maxLoopCount = 20;

uint8_t testingLEDs = 1;

void setup() {
//**** LED Section ****//
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

  //**** Audio Section ****//
  AudioMemory(35);
  
  sgtl5000_1.setAddress(LOW);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);

  sgtl5000_2.setAddress(HIGH);
  sgtl5000_2.enable();
  sgtl5000_2.volume(0.5);

  sine1.amplitude(1);
  sine1.frequency(440);
  
  sine2.amplitude(1);
  sine2.frequency(440);

  sine3.amplitude(1);
  sine3.frequency(440);
  
  sine4.amplitude(1);
  sine4.frequency(440);

}

void loop()
{
	//LED stuff
	if( testingLEDs )
	{
		for( int n = 0; n <= 1; n++ )
		{
			for( int i = 0; i <= 7; i++ )
			{
				SPI.transfer(0x01);
				SPI.transfer(0x01);
				SPI.transfer(0x01);
				SPI.transfer(0x01);
				SPI.transfer(0x01);
				SPI.transfer(0x01);
				SPI.transfer(0x00);
				SPI.transfer(0x01 << i);
				digitalWrite(LCLKPin, 0);
				delay(1);
				digitalWrite(LCLKPin, 1);  
				delay(n*200+50);
				
			} 
			
			for( int i = 0; i <= 7; i++ )
			{
				SPI.transfer(0x01);
				SPI.transfer(0x01);
				SPI.transfer(0x01);
				SPI.transfer(0x01);
				SPI.transfer(0x01);
				SPI.transfer(0x01);
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
					SPI.transfer(0x00);
					SPI.transfer(0x00);
					SPI.transfer(0x00);
					SPI.transfer(0x00);
					SPI.transfer(0x00);
					SPI.transfer(0x00);
					SPI.transfer(0xFE);
					SPI.transfer(0xFF);
					digitalWrite(LCLKPin, 0);
					delay(1);
					digitalWrite(LCLKPin, 1);  
				}
				for(int i = 0; i < 3; i++)
				{
					SPI.transfer(0x00);
					SPI.transfer(0x00);
					SPI.transfer(0x00);
					SPI.transfer(0x00);
					SPI.transfer(0x00);
					SPI.transfer(0x00);
					SPI.transfer(0x78);
					SPI.transfer(0x3F);
					digitalWrite(LCLKPin, 0);
					delay(1);
					digitalWrite(LCLKPin, 1);  
				}
				for(int i = 0; i < 6; i++)
				{
					SPI.transfer(0x00);
					SPI.transfer(0x00);
					SPI.transfer(0x00);
					SPI.transfer(0x00);
					SPI.transfer(0x00);
					SPI.transfer(0x00);
					SPI.transfer(0x30);
					SPI.transfer(0x0F);
					digitalWrite(LCLKPin, 0);
					delay(1);
					digitalWrite(LCLKPin, 1);  
				}
				for(int i = 0; i < 12; i++)
				{
					SPI.transfer(0x00);
					SPI.transfer(0x00);
					SPI.transfer(0x00);
					SPI.transfer(0x00);
					SPI.transfer(0x00);
					SPI.transfer(0x00);
					SPI.transfer(0x10);
					SPI.transfer(0x03);
					digitalWrite(LCLKPin, 0);
					delay(1);
					digitalWrite(LCLKPin, 1);  
				}
		}
		
		testingLEDs = 0;
	}
	SPI.transfer(0x00);//MSB
	SPI.transfer(0x00);
	SPI.transfer(0x00);
	SPI.transfer(0x00);
	SPI.transfer(0x00);
	SPI.transfer(0x00);
	SPI.transfer(0xAA);
	SPI.transfer(0x55);
	digitalWrite(LCLKPin, 0);
	delay(1);
	digitalWrite(LCLKPin, 1);  	 
	sine1.frequency(220);
	sine2.frequency(311.127);
	sine3.frequency(220);
	sine4.frequency(440);	
	delay(1000);

	SPI.transfer(0x00);//MSB
	SPI.transfer(0x00);
	SPI.transfer(0x00);
	SPI.transfer(0x00);
	SPI.transfer(0x00);
	SPI.transfer(0x00);
	SPI.transfer(0x55);
	SPI.transfer(0xAA);
	digitalWrite(LCLKPin, 0);
	delay(1);
	digitalWrite(LCLKPin, 1);  	 
	sine1.frequency(311.127);
	sine2.frequency(220);
	sine3.frequency(440);
	sine4.frequency(220);	
	delay(1000);
}
