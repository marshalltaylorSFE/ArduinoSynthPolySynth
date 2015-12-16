#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "effect_bendvelope.h"

//**Timers and stuff**************************//
#include "timerModule32.h"
//Globals
uint32_t MAXTIMER = 60000000;
uint32_t MAXINTERVAL = 2000000;

uint8_t noteToggle = 0;

//**Timers and stuff**************************//
IntervalTimer myTimer;

//tick variable for interrupt driven timer1
uint32_t usTicks = 0;
uint8_t usTicksMutex = 1; //start locked out

//HOW TO OPERATE
//  Make TimerClass objects for each thing that needs periodic service
//  pass the interval of the period in ticks
//  Set MAXINTERVAL to the max foreseen interval of any TimerClass
//  Set MAXTIMER to overflow number in the header.  MAXTIMER + MAXINTERVAL
//    cannot exceed variable size.

TimerClass32 envTimer( 1000 );
TimerClass32 debugTimer( 200000 );

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
	
	// initialize IntervalTimer
	myTimer.begin(serviceUS, 1);  // serviceMS to run every 0.000001 seconds
	
	AudioMemory(10);
  
	sgtl5000_1.enable();
	sgtl5000_1.volume(0.4);
	sgtl5000_1.unmuteHeadphone();

	waveform1.begin(1.0, 80, WAVEFORM_SAWTOOTH);
	waveform1.frequency(80);
	
	//Setting envelope
	bendvelope.attack( 20, 100 );// 0 to 255 for length, -128 to 127
	bendvelope.decay( 100, 100 );// 0 to 255 for length, -128 to 127
	bendvelope.sustain( 150 );// 0 to 255 for level
	bendvelope.release( 30, 100 );// 0 to 255 for length, -128 to 127
	bendvelope.setAttackHold( 10 );
	Serial.println("Settings set");	
	
}

void loop()
{
	envTimer.update(usTicks);
	debugTimer.update(usTicks);
	
	if(envTimer.flagStatus() == PENDING)
	{
		bendvelope.tick(1000);
	
	}
	if(debugTimer.flagStatus() == PENDING)
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
		AudioProcessorUsageMaxReset();	

		if(noteToggle == 1)
		{
			bendvelope.noteOn();
			noteToggle = 0;
		}
		else
		{
			bendvelope.noteOff();
			noteToggle = 1;
		}
		
		
	
	}

}

void serviceUS(void)
{
  uint32_t returnVar = 0;
  if(usTicks >= ( MAXTIMER + MAXINTERVAL ))
  {
    returnVar = usTicks - MAXTIMER;

  }
  else
  {
    returnVar = usTicks + 1;
  }
  usTicks = returnVar;
  usTicksMutex = 0;  //unlock
}
