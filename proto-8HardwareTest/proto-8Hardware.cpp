#include <SPI.h>
#include "proto-8Hardware.h"

// -----------------------------------------------------------------------------
LEDShiftRegister::LEDShiftRegister()
{
}

void LEDShiftRegister::begin()
{
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

void LEDShiftRegister::send( uint8_t LEDNum, uint8_t LEDState )
{
	//Store datea
	store( LEDNum, LEDState );
	//Send packet
	send();

}

void LEDShiftRegister::send()
{
	//Send packet
	SPI.transfer(LEDData[7]);//MSB
	SPI.transfer(LEDData[6]);
	SPI.transfer(LEDData[5]);
	SPI.transfer(LEDData[4]);
	SPI.transfer(LEDData[3]);
	SPI.transfer(LEDData[2]);
	SPI.transfer(LEDData[1]);
	SPI.transfer(LEDData[0]);
	//Fwap the clock
	digitalWrite(LCLKPin, 0);
	delay(1);
	digitalWrite(LCLKPin, 1);  	 

}

void LEDShiftRegister::store( uint8_t LEDNum, uint8_t LEDState )
{
	//Sanitize!
	if( LEDNum == 0 )
	{
		return;
	}
	
	LEDNum--;
	int16_t byteSeekVar = LEDNum;
	uint8_t bitSeekVar = 0;
	for( int i = 0; i < 8; i++ )
	{
		byteSeekVar -= 8;
		if( byteSeekVar < 0 )
		{
			//Time to leave
			//if LEDNum was large, i will be large
			//First, get bit position in byte
			bitSeekVar = LEDNum - ( 8 * i );
			if( LEDState == 0 )
			{
				LEDData[i] &= ~( 1 << bitSeekVar );
			}
			else
			{
				LEDData[i] |= ( 1 << bitSeekVar );
			}
			break;
		}
	}
}

void LEDShiftRegister::clear()
{
	for( int i = 0; i < 8; i++ )
	{
		LEDData[i] = 0;
	}
}
// -----------------------------------------------------------------------------
AnalogMuxTree::AnalogMuxTree()
{
}

void AnalogMuxTree::begin()
{
	// set the pins
	pinMode(MUXAPin, OUTPUT);
	pinMode(MUXBPin, OUTPUT);
	pinMode(MUXCPin, OUTPUT);
	pinMode(MUXDPin, OUTPUT);

	pinMode(AnENPin, OUTPUT);

	pinMode(A10Pin, INPUT);
	pinMode(A11Pin, INPUT);
	pinMode(A12Pin, INPUT);
	pinMode(A13Pin, INPUT);
	
	digitalWrite(MUXAPin, 0);
	digitalWrite(MUXBPin, 0);
	digitalWrite(MUXCPin, 0);
	digitalWrite(MUXDPin, 0);

	digitalWrite(AnENPin, 0);
	
}

void AnalogMuxTree::scan()
{
	for( int i = 0; i < 16; i++ )
	{
		//Set the address
		digitalWrite(MUXAPin, (i & 0x01) >> 0);
		digitalWrite(MUXBPin, (i & 0x02) >> 1);
		digitalWrite(MUXCPin, (i & 0x04) >> 2);
		digitalWrite(MUXDPin, (i & 0x08) >> 3);
		//Read the pins
		delay(1);
		KnobData[i] = analogRead(A10Pin);
		KnobData[i+16] = analogRead(A11Pin);
		KnobData[i+32] = analogRead(A12Pin);
		KnobData[i+48] = analogRead(A13Pin);
	}
}

uint16_t AnalogMuxTree::fetch( uint8_t knobNumber )
{
	return KnobData[knobNumber -1];
}
