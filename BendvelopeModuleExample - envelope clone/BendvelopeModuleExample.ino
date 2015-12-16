#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "effect_bendvelope.h"

AudioOutputI2S           i2s1;
AudioControlSGTL5000     sgtl5000_1;

AudioSynthWaveform       waveform1;
AudioEffectBendvelope    bendvelope;      //custom
//AudioConnection          patchCord1(waveform1, 0, i2s1, 0);
AudioConnection          patchCord1(waveform1, 0, bendvelope, 0);
AudioConnection          patchCord2(bendvelope, 0, i2s1, 0);
AudioConnection          patchCord3(bendvelope, 0, i2s1, 1);
// -----------------------------------------------------------------------------
void setup() 
{
	//Initialize serial:
	Serial.begin(9600);
	delay(2000);
	Serial.println("Program Started");
	
	AudioMemory(10);
  
	sgtl5000_1.enable();
	sgtl5000_1.volume(0.4);
	sgtl5000_1.unmuteHeadphone();

	waveform1.begin(1.0, 100, WAVEFORM_SAWTOOTH);
	waveform1.frequency(100);
	
	//Setting envelope
	//bendvelope.attack(uint8_t, int8_t);// 0 to 255 for length, -128 to 127
	//decay(uint8_t, int8_t);// 0 to 255 for length, -128 to 127
	//sustain(uint8_t);// 0 to 255 for level
	//release(uint8_t, int8_t);// 0 to 255 for length, -128 to 127

}

void loop()
{
	//Trigger
	bendvelope.noteOn();
	delay(1000);
	{
		//Serial.print("\n\nrxNoteList\n");
		//rxNoteList.printfMicroLL();
		//Serial.print("\n\nnoteOnInList\n");
		//noteOnInList.printfMicroLL();
		//Serial.print("\n\nnoteOnOutLists\n");
		Serial.print("all=");
		Serial.print(AudioProcessorUsage());
		Serial.print(",");
		Serial.print(AudioProcessorUsageMax());
		Serial.print("    ");
		Serial.print("Memory: ");
		Serial.print(AudioMemoryUsage());
		Serial.print(",");
		Serial.print(AudioMemoryUsageMax());
		Serial.print(",   FreeRam: ");
		Serial.print(FreeRam());
		Serial.print("\n");
	}
	bendvelope.noteOff();
	delay(1000);
}