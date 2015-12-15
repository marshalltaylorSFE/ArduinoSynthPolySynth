#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "effect_bendvelope.h"

AudioSynthWaveform       waveform1;
AudioEffectBendvelope    bendvelope;      //custom
AudioConnection          patchCord1(waveform1, 0, bendvelope, 0);
AudioConnection          patchCord2(waveform1, 1, i2s1, 0);
AudioConnection          patchCord3(waveform1, 1, i2s1, 1);
AudioOutputI2S           i2s1;
AudioControlSGTL5000     sgtl5000_1;

// -----------------------------------------------------------------------------
void setup() 
{
	//Initialize serial:
	Serial.begin(9600);
	delay(2000);
	Serial.println("Program Started");
	
	AudioMemory(10);
  
	sgtl5000_1.enable();
	sgtl5000_1.volume(1.0);
	sgtl5000_1.unmuteHeadphone();

	
}

void loop()
{
}