//**********************************************************************//
//  BEERWARE LICENSE
//
//  This code is free for any use provided that if you meet the author
//  in person, you buy them a beer.
//
//  This license block is BeerWare itself.
//
//  Written by:  Marshall Taylor
//  Created:  March 21, 2015
//
//**********************************************************************//

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "effect_bendvelope.h"

AudioEffectBendvelope    bendvelope1;      //custom
AudioEffectBendvelope    bendvelope2;      //custom
AudioEffectBendvelope    bendvelope3;      //custom
AudioEffectBendvelope    bendvelope4;      //custom


AudioSynthWaveform       waveform1A;      //xy=191,353
AudioSynthWaveform       waveform1B;      //xy=192,276
AudioSynthWaveform       waveform1C;      //xy=192,314
AudioSynthWaveform       waveform1D;      //xy=193,236
AudioMixer4              mixer1;         //xy=379,294
AudioConnection          patchCord1A(waveform1A, 0, mixer1, 0);
AudioConnection          patchCord1B(waveform1B, 0, mixer1, 1);
AudioConnection          patchCord1C(waveform1C, 0, mixer1, 2);
AudioConnection          patchCord1D(waveform1D, 0, mixer1, 3);


AudioSynthWaveform       waveform2A;      //xy=191,353
AudioSynthWaveform       waveform2B;      //xy=192,276
AudioSynthWaveform       waveform2C;      //xy=192,314
AudioSynthWaveform       waveform2D;      //xy=193,236
AudioMixer4              mixer2;         //xy=379,294
AudioConnection          patchCord2A(waveform2A, 0, mixer2, 0);
AudioConnection          patchCord2B(waveform2B, 0, mixer2, 1);
AudioConnection          patchCord2C(waveform2C, 0, mixer2, 2);
AudioConnection          patchCord2D(waveform2D, 0, mixer2, 3);

AudioSynthWaveform       waveform3A;      //xy=191,353
AudioSynthWaveform       waveform3B;      //xy=192,276
AudioSynthWaveform       waveform3C;      //xy=192,314
AudioSynthWaveform       waveform3D;      //xy=193,236
AudioMixer4              mixer3;         //xy=379,294
AudioConnection          patchCord3A(waveform3A, 0, mixer3, 0);
AudioConnection          patchCord3B(waveform3B, 0, mixer3, 1);
AudioConnection          patchCord3C(waveform3C, 0, mixer3, 2);
AudioConnection          patchCord3D(waveform3D, 0, mixer3, 3);

AudioSynthWaveform       waveform4A;      //xy=191,353
AudioSynthWaveform       waveform4B;      //xy=192,276
AudioSynthWaveform       waveform4C;      //xy=192,314
AudioSynthWaveform       waveform4D;      //xy=193,236
AudioMixer4              mixer4;         //xy=379,294
AudioConnection          patchCord4A(waveform4A, 0, mixer4, 0);
AudioConnection          patchCord4B(waveform4B, 0, mixer4, 1);
AudioConnection          patchCord4C(waveform4C, 0, mixer4, 2);
AudioConnection          patchCord4D(waveform4D, 0, mixer4, 3);

AudioMixer4              mixer5;         //xy=379,294
AudioConnection          patchCord1BendIn(mixer1, 0, bendvelope1, 0);
AudioConnection          patchCord2BendIn(mixer2, 0, bendvelope2, 0);
AudioConnection          patchCord3BendIn(mixer3, 0, bendvelope3, 0);
AudioConnection          patchCord4BendIn(mixer4, 0, bendvelope4, 0);
AudioConnection          patchCord1BendOut(bendvelope1, 0, mixer5, 0);
AudioConnection          patchCord2BendOut(bendvelope2, 0, mixer5, 1);
AudioConnection          patchCord3BendOut(bendvelope3, 0, mixer5, 2);
AudioConnection          patchCord4BendOut(bendvelope4, 0, mixer5, 3);


AudioFilterStateVariable filter1;        //xy=525,301
AudioFilterStateVariable filter3;        //xy=681,292
AudioOutputI2S           i2s1;           //xy=839,296


AudioConnection          patchCord5(mixer5, 0, filter1, 0);
AudioConnection          patchCord6(filter1, 0, filter3, 0);
AudioConnection          patchCord7(filter3, 0, i2s1, 0);
AudioConnection          patchCord8(filter3, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=493,374
// GUItool: end automatically generated code


//**Timers and stuff**************************//
#include "timerModule32.h"
//Globals
uint32_t MAXTIMER = 60000000;
uint32_t MAXINTERVAL = 2000000;

#include "timeKeeper.h"
//**Panels and stuff**************************//
#include "Panel.h"
//**Seven Segment Display*********************//
#include "Wire.h"

#include "MicroLL.h"

//**Timers and stuff**************************//
IntervalTimer myTimer;

//HOW TO OPERATE
//  Make TimerClass objects for each thing that needs periodic service
//  pass the interval of the period in ticks
//  Set MAXINTERVAL to the max foreseen interval of any TimerClass
//  Set MAXTIMER to overflow number in the header.  MAXTIMER + MAXINTERVAL
//    cannot exceed variable size.

TimerClass32 midiRecordTimer( 1000 );
TimerClass32 panelUpdateTimer(10000);
uint8_t debugLedStates = 1;

TimerClass32 ledToggleTimer( 333000 );
uint8_t ledToggleState = 0;
TimerClass32 ledToggleFastTimer( 100000 );
uint8_t ledToggleFastState = 0;

TimerClass32 envTimer( 2000 );

//TimerClass32 processSMTimer( 50000 );

TimerClass32 debounceTimer(5000);

TimerClass32 debugTimer(2000000);

//tick variable for interrupt driven timer1
uint32_t usTicks = 0;
uint8_t usTicksMutex = 1; //start locked out

//**Panel State Machine***********************//
#include "looperPanel.h"
LooperPanel myLooperPanel;



// MIDI things
#include <MIDI.h>
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, midiA);

MicroLL rxNoteList;
MicroLL noteOnInList;


uint8_t rxLedFlag = 0;

//New voice trackers
uint8_t voicesUsed[4] = {0,0,0,0};
float fineTuneA = 68 * .00261438 + 0.6666666;
float coarseTuneA = 2 * 9 * .0137255 + 0.25;
float fineTuneB = 68 * .00261438 + 0.6666666;
float coarseTuneB = 2 * 27 * .0137255 + 0.25;
float fineTuneC = 64 * .00261438 + 0.6666666;
float coarseTuneC = 2 * 64 * .0137255 + 0.25;
float fineTuneD = 64 * .00261438 + 0.6666666;
float coarseTuneD = 2 * 64 * .0137255 + 0.25;


float last1 = 1;
float last2 = 1;
float last3 = 1;
float last4 = 1;

uint8_t lastAttack = 10;
int8_t lastAttackBend = 0;
uint8_t lastDecay = 10;
int8_t lastDecayBend = 0;
uint8_t lastRelease = 10;
int8_t lastReleaseBend = 0;

#include "note_values.h"

void HandleNoteOn(byte channel, byte pitch, byte velocity)
{
	rxLedFlag = 1;
	MidiEvent tempEvent;

	tempEvent.eventType = 0x90;
	tempEvent.channel = channel;
	tempEvent.value = pitch;
	tempEvent.data = velocity;
	tempEvent.voice = -1;	
	//Hackery
	if( velocity = 0 )
	{
		tempEvent.eventType = 0x80;
	}
	rxNoteList.pushObject( tempEvent );
	
	//midiA.sendNoteOn(pitch, velocity, channel);
}

void HandleNoteOff(byte channel, byte pitch, byte velocity)
{
	rxLedFlag = 1;
	
	MidiEvent tempEvent;

	tempEvent.eventType = 0x80;
	tempEvent.channel = channel;
	tempEvent.value = pitch;
	tempEvent.data = 0;
	tempEvent.voice = -1;	

	rxNoteList.pushObject( tempEvent );
	
	//midiA.sendNoteOff(pitch, velocity, channel);
	
}

void HandleControlChange(byte channel, byte number, byte value)
{
	Serial.print(number);
	Serial.print(", ");
	Serial.println(value);

	
	
	
	
	
	switch( number )
	{
		case 1:
		//Filter
		filter1.frequency((value * value * 0.9) + 40);  
		filter3.frequency((value * value * 0.9) + 40);  
		break;
		case 21:
		//Process fine tune-- build the multiplier 2/3 to 4/3
		//f(x) = 2/3 * 1/255 * x + 2/3
		fineTuneA = value * .00261438 + 0.6666666;
		waveform1A.frequency((last1) * fineTuneA * coarseTuneA);
		waveform2A.frequency((last2) * fineTuneA * coarseTuneA);
		waveform3A.frequency((last3) * fineTuneA * coarseTuneA);
		waveform4A.frequency((last4) * fineTuneA * coarseTuneA);
 		break;
		case 23:
		//Process fine tune-- build the multiplier 2/3 to 4/3
		//f(x) = 2/3 * 1/255 * x + 2/3
		fineTuneB = value * .00261438 + 0.6666666;
		waveform1B.frequency((last1) * fineTuneB * coarseTuneB);
		waveform2B.frequency((last2) * fineTuneB * coarseTuneB);
		waveform3B.frequency((last3) * fineTuneB * coarseTuneB);
		waveform4B.frequency((last4) * fineTuneB * coarseTuneB);
		break;
		case 25:
		//Process fine tune-- build the multiplier 2/3 to 4/3
		//f(x) = 2/3 * 1/255 * x + 2/3
		fineTuneC = value * .00261438 + 0.6666666;
		waveform1C.frequency((last1) * fineTuneC * coarseTuneC);
		waveform2C.frequency((last2) * fineTuneC * coarseTuneC);
		waveform3C.frequency((last3) * fineTuneC * coarseTuneC);
		waveform4C.frequency((last4) * fineTuneC * coarseTuneC);
		break;
		case 27:
		//Process fine tune-- build the multiplier 2/3 to 4/3
		//f(x) = 2/3 * 1/255 * x + 2/3
		fineTuneD = value * .00261438 + 0.6666666;
		waveform1D.frequency((last1) * fineTuneD * coarseTuneD);
		waveform2D.frequency((last2) * fineTuneD * coarseTuneD);
		waveform3D.frequency((last3) * fineTuneD * coarseTuneD);
		waveform4D.frequency((last4) * fineTuneD * coarseTuneD);
		break;
		case 20:
		//Process coarse tune-- build the multiplier 0.25 to 4
		//f(x) = 7/2 * 1/255 * x + 0.5
		coarseTuneA = 2 * value * .0137255 + 0.25;
		waveform1A.frequency((last1) * fineTuneA * coarseTuneA);
		waveform2A.frequency((last2) * fineTuneA * coarseTuneA);
		waveform3A.frequency((last3) * fineTuneA * coarseTuneA);
		waveform4A.frequency((last4) * fineTuneA * coarseTuneA);
 		break;
		case 22:
		//Process coarse tune-- build the multiplier 0.25 to 4
		//f(x) = 7/2 * 1/255 * x + 0.5
		coarseTuneB = 2 * value * .0137255 + 0.25;
		waveform1B.frequency((last1) * fineTuneB * coarseTuneB);
		waveform2B.frequency((last2) * fineTuneB * coarseTuneB);
		waveform3B.frequency((last3) * fineTuneB * coarseTuneB);
		waveform4B.frequency((last4) * fineTuneB * coarseTuneB);
		break;
		case 24:
		//Process coarse tune-- build the multiplier 0.25 to 4
		//f(x) = 7/2 * 1/255 * x + 0.5
		coarseTuneC = 2 * value * .0137255 + 0.25;
		waveform1C.frequency((last1) * fineTuneC * coarseTuneC);
		waveform2C.frequency((last2) * fineTuneC * coarseTuneC);
		waveform3C.frequency((last3) * fineTuneC * coarseTuneC);
		waveform4C.frequency((last4) * fineTuneC * coarseTuneC);
		break;
		case 26:
		//Process coarse tune-- build the multiplier 0.25 to 4
		//f(x) = 7/2 * 1/255 * x + 0.5
		coarseTuneD = 2 * value * .0137255 + 0.25;
		waveform1D.frequency((last1) * fineTuneD * coarseTuneD);
		waveform2D.frequency((last2) * fineTuneD * coarseTuneD);
		waveform3D.frequency((last3) * fineTuneD * coarseTuneD);
		waveform4D.frequency((last4) * fineTuneD * coarseTuneD);
		break;
		case 32:
		lastAttack = (value << 1 ) + 1;
		Serial.println("Start");
		bendvelope1.attack( lastAttack, lastAttackBend );// 0 to 255 for length, -128 to 127
		bendvelope2.attack( lastAttack, lastAttackBend );// 0 to 255 for length, -128 to 127
		bendvelope3.attack( lastAttack, lastAttackBend );// 0 to 255 for length, -128 to 127
		bendvelope4.attack( lastAttack, lastAttackBend );// 0 to 255 for length, -128 to 127
		Serial.println("End");
		break;
		case 33:
		Serial.println("Start");
		lastAttackBend = (value << 1 ) - 128;
		bendvelope1.attack( lastAttack, lastAttackBend );// 0 to 255 for length, -128 to 127
		bendvelope2.attack( lastAttack, lastAttackBend );// 0 to 255 for length, -128 to 127
		bendvelope3.attack( lastAttack, lastAttackBend );// 0 to 255 for length, -128 to 127
		bendvelope4.attack( lastAttack, lastAttackBend );// 0 to 255 for length, -128 to 127
		Serial.println("End");
		break;
		case 34:
		//
		bendvelope1.setAttackHold( (value << 1 ) + 1 );
		bendvelope2.setAttackHold( (value << 1 ) + 1 );
		bendvelope3.setAttackHold( (value << 1 ) + 1 );
		bendvelope4.setAttackHold( (value << 1 ) + 1 );
		break;
		case 35:
		lastDecay = (value << 1 ) + 1;
		bendvelope1.decay( lastDecay, lastDecayBend );// 0 to 255 for length, -128 to 127
		bendvelope2.decay( lastDecay, lastDecayBend );// 0 to 255 for length, -128 to 127
		bendvelope3.decay( lastDecay, lastDecayBend );// 0 to 255 for length, -128 to 127
		bendvelope4.decay( lastDecay, lastDecayBend );// 0 to 255 for length, -128 to 127
		break;
		case 36:
		lastDecayBend = (value << 1 ) - 128;
		bendvelope1.decay( lastDecay, lastDecayBend );// 0 to 255 for length, -128 to 127
		bendvelope2.decay( lastDecay, lastDecayBend );// 0 to 255 for length, -128 to 127
		bendvelope3.decay( lastDecay, lastDecayBend );// 0 to 255 for length, -128 to 127
		bendvelope4.decay( lastDecay, lastDecayBend );// 0 to 255 for length, -128 to 127
		break;
		case 37:
		//
		bendvelope1.sustain( (value << 1 ) );// 0 to 255 for level
		bendvelope2.sustain( (value << 1 ) );// 0 to 255 for level
		bendvelope3.sustain( (value << 1 ) );// 0 to 255 for level
		bendvelope4.sustain( (value << 1 ) );// 0 to 255 for level
		break;
		case 38:
		lastRelease = (value << 1 ) + 1;
		bendvelope1.release( lastRelease, lastReleaseBend );// 0 to 255 for length, -128 to 127
		bendvelope2.release( lastRelease, lastReleaseBend );// 0 to 255 for length, -128 to 127
		bendvelope3.release( lastRelease, lastReleaseBend );// 0 to 255 for length, -128 to 127
		bendvelope4.release( lastRelease, lastReleaseBend );// 0 to 255 for length, -128 to 127
		break;
		case 39:
		lastReleaseBend = (value << 1 ) - 128;
		bendvelope1.release( lastRelease, lastReleaseBend );// 0 to 255 for length, -128 to 127
		bendvelope2.release( lastRelease, lastReleaseBend );// 0 to 255 for length, -128 to 127
		bendvelope3.release( lastRelease, lastReleaseBend );// 0 to 255 for length, -128 to 127
		bendvelope4.release( lastRelease, lastReleaseBend );// 0 to 255 for length, -128 to 127
		break;
		default:
		break;
	}
}

void handleSystemReset(void)
{
	//Flush all notes
	while(rxNoteList.listLength())
	{
		rxNoteList.dropObject( 0 );
	}
	//Clear voice flags
	voicesUsed[0] = 0;
	voicesUsed[1] = 0;
	voicesUsed[2] = 0;
	voicesUsed[3] = 0;
	float last1 = 1;
	float last2 = 1;
	float last3 = 1;
	float last4 = 1;
	//Send note offs to all envelopes
	bendvelope1.noteOff();
	bendvelope2.noteOff();
	bendvelope3.noteOff();
	bendvelope4.noteOff();
}
// -----------------------------------------------------------------------------
void setup() 
{
	//Initialize serial:
	Serial.begin(9600);
	delay(2000);
	Serial.println("Program Started");
	
	bendvelope1.attack( 10, 0 );// 0 to 255 for length, -128 to 127
	bendvelope1.decay( 10, 0 );// 0 to 255 for length, -128 to 127
	bendvelope1.sustain( 150 );// 0 to 255 for level
	bendvelope1.release( 10, 0 );// 0 to 255 for length, -128 to 127
	bendvelope1.setAttackHold( 10 );
	bendvelope2.attack( 10, 0 );// 0 to 255 for length, -128 to 127
	bendvelope2.decay( 10, 0 );// 0 to 255 for length, -128 to 127
	bendvelope2.sustain( 150 );// 0 to 255 for level
	bendvelope2.release( 10, 0 );// 0 to 255 for length, -128 to 127
	bendvelope2.setAttackHold( 10 );
	bendvelope3.attack( 10, 0 );// 0 to 255 for length, -128 to 127
	bendvelope3.decay( 10, 0 );// 0 to 255 for length, -128 to 127
	bendvelope3.sustain( 150 );// 0 to 255 for level
	bendvelope3.release( 10, 0 );// 0 to 255 for length, -128 to 127
	bendvelope3.setAttackHold( 10 );
	bendvelope4.attack( 10, 0 );// 0 to 255 for length, -128 to 127
	bendvelope4.decay( 10, 0 );// 0 to 255 for length, -128 to 127
	bendvelope4.sustain( 150 );// 0 to 255 for level
	bendvelope4.release( 10, 0 );// 0 to 255 for length, -128 to 127
	bendvelope4.setAttackHold( 10 );
	
	//Init panel.h stuff
	myLooperPanel.init();
	
	// initialize IntervalTimer
	myTimer.begin(serviceUS, 1);  // serviceMS to run every 0.001 seconds
	
	//Update the panel
	myLooperPanel.update();

	//Connect MIDI handlers
	midiA.setHandleNoteOn(HandleNoteOn);  // Put only the name of the function
	midiA.setHandleNoteOff(HandleNoteOff);
	midiA.setHandleControlChange(HandleControlChange);
	midiA.setHandleSystemReset(handleSystemReset);
	// Initiate MIDI communications, listen to all channels
	midiA.begin(MIDI_CHANNEL_OMNI);
	//midiA.turnThruOn();
	midiA.turnThruOff();
	AudioMemory(50);
  
	sgtl5000_1.enable();
	sgtl5000_1.volume(1.0);
	sgtl5000_1.unmuteHeadphone();
	
	filter1.frequency(8000);  
	filter3.frequency(8000);
	
	waveform1A.begin(1.0, 100, WAVEFORM_SQUARE);
	waveform1B.begin(1.0, 100, WAVEFORM_SAWTOOTH);
	waveform1C.begin(0.6, 100, WAVEFORM_SAWTOOTH);
	waveform1D.begin(0.6, 100, WAVEFORM_SAWTOOTH);
	
	waveform2A.begin(1.0, 100, WAVEFORM_SQUARE);
	waveform2B.begin(1.0, 100, WAVEFORM_SAWTOOTH);
	waveform2C.begin(0.6, 100, WAVEFORM_SAWTOOTH);
	waveform2D.begin(0.6, 100, WAVEFORM_SAWTOOTH);
	
	waveform3A.begin(1.0, 100, WAVEFORM_SQUARE);
	waveform3B.begin(1.0, 100, WAVEFORM_SAWTOOTH);
	waveform3C.begin(0.6, 100, WAVEFORM_SAWTOOTH);
	waveform3D.begin(0.6, 100, WAVEFORM_SAWTOOTH);
	
	waveform4A.begin(1.0, 100, WAVEFORM_SQUARE);
	waveform4B.begin(1.0, 100, WAVEFORM_SAWTOOTH);
	waveform4C.begin(0.6, 100, WAVEFORM_SAWTOOTH);
	waveform4D.begin(0.6, 100, WAVEFORM_SAWTOOTH);
	
	mixer1.gain(0, 0.0625);
	mixer1.gain(1, 0.0625);
	mixer1.gain(2, 0.0625);
	mixer1.gain(3, 0.0625);

	mixer2.gain(0, 0.0625);
	mixer2.gain(1, 0.0625);
	mixer2.gain(2, 0.0625);
	mixer2.gain(3, 0.0625);

	mixer3.gain(0, 0.0625);
	mixer3.gain(1, 0.0625);
	mixer3.gain(2, 0.0625);
	mixer3.gain(3, 0.0625);

	mixer4.gain(0, 0.0625);
	mixer4.gain(1, 0.0625);
	mixer4.gain(2, 0.0625);
	mixer4.gain(3, 0.0625);

	mixer5.gain(0, 0.25);
	mixer5.gain(1, 0.25);
	mixer5.gain(2, 0.25);
	mixer5.gain(3, 0.25);
	
	
}

void loop()
{
//**Copy to make a new timer******************//  
//   msTimerA.update(usTicks);

	midiRecordTimer.update(usTicks);
	ledToggleTimer.update(usTicks);
	ledToggleFastTimer.update(usTicks);
	panelUpdateTimer.update(usTicks);
	debounceTimer.update(usTicks);
	envTimer.update(usTicks);
	
	debugTimer.update(usTicks);

	//**Copy to make a new timer******************//  
	//  if(msTimerA.flagStatus() == PENDING)
	//  {
	//    digitalWrite( LEDPIN, digitalRead(LEDPIN) ^ 1 );
	//  }
	
	//**Bendvelope timer**************************//  
	if(envTimer.flagStatus() == PENDING)
	{
		bendvelope1.tick(2000);
		bendvelope2.tick(2000);
		bendvelope3.tick(2000);
		bendvelope4.tick(2000);
		//Serial.println(usTicks);
	}
	//**Debounce timer****************************//  
	if(debounceTimer.flagStatus() == PENDING)
	{
		myLooperPanel.timersMIncrement(5);
	
	}
		
	//**Process the panel and state machine***********//  
	if(panelUpdateTimer.flagStatus() == PENDING)
	{
		//Provide inputs
		if( rxLedFlag == 1 )
		{
			rxLedFlag = 0;
			myLooperPanel.setRxLed();
		}

		//Tick the machine
		myLooperPanel.processMachine();
		
		//Deal with outputs
	}
	
	if(midiRecordTimer.flagStatus() == PENDING)
	{
		//
		listIdemNumber_t unservicedNoteCount = rxNoteList.listLength();
		//Get a note for this round
		MidiEvent tempNote;
		if( unservicedNoteCount > 0 )
		{
			tempNote = *rxNoteList.readObject( unservicedNoteCount - 1 );
			if( tempNote.eventType == 0x90 )//We got a new note-on
			{
				//Search for the note on.  If found, do nothing, else write
				if( noteOnInList.seekObjectbyNoteValue( tempNote ) == -1 )
				{
					//note not found.
					////////This is where we might play the note
					//Check for available voice by finding the next zero

					int nextAvailable = -1;
					int seek;
					uint8_t currentAmps[4];
					currentAmps[0] = bendvelope1.amp;
					currentAmps[1] = bendvelope2.amp;
					currentAmps[2] = bendvelope3.amp;
					currentAmps[3] = bendvelope4.amp;
					
					//Seek the next available with the lowest current amp.
					for( seek = 0; seek < 4; seek++ )
					{
						if(voicesUsed[seek] == 0)
						{
							//the voice is free
							if(nextAvailable == -1)
							{
								//no previous note to compare
								nextAvailable = seek;
							}
							else
							{
								//compare, choose the quieter voice
								if( currentAmps[seek] < currentAmps[nextAvailable] )
								{
									nextAvailable = seek;
								}
								
							}
						}

					}
					if( nextAvailable != -1 )//One is free
					{
						//Assign the note to this voice
						tempNote.voice = nextAvailable;
						voicesUsed[nextAvailable] = 1;
						//mixer5.gain(tempNote.voice, 0.25);
						//select voice
						float tempFrequencyA = note_frequency[tempNote.value] * fineTuneA * coarseTuneA;
						float tempFrequencyB = note_frequency[tempNote.value] * fineTuneB * coarseTuneB;
						float tempFrequencyC = note_frequency[tempNote.value] * fineTuneC * coarseTuneC;
						float tempFrequencyD = note_frequency[tempNote.value] * fineTuneD * coarseTuneD;

						switch( tempNote.voice )
						{
							case 0:
							waveform1A.frequency(tempFrequencyA);
							waveform1B.frequency(tempFrequencyB);
							waveform1C.frequency(tempFrequencyC);
							waveform1D.frequency(tempFrequencyD);
							last1 = note_frequency[tempNote.value];
							bendvelope1.noteOn();
							break;
							case 1:
							waveform2A.frequency(tempFrequencyA);
							waveform2B.frequency(tempFrequencyB);
							waveform2C.frequency(tempFrequencyC);
							waveform2D.frequency(tempFrequencyD);
							last2 = note_frequency[tempNote.value];
							bendvelope2.noteOn();
							break;
							case 2:
							waveform3A.frequency(tempFrequencyA);
							waveform3B.frequency(tempFrequencyB);
							waveform3C.frequency(tempFrequencyC);
							waveform3D.frequency(tempFrequencyD);
							last3 = note_frequency[tempNote.value];
							bendvelope3.noteOn();
							break;
							case 3:
							waveform4A.frequency(tempFrequencyA);
							waveform4B.frequency(tempFrequencyB);
							waveform4C.frequency(tempFrequencyC);
							waveform4D.frequency(tempFrequencyD);
							last4 = note_frequency[tempNote.value];
							bendvelope4.noteOn();
							break;
							default:
							break;
						}
						Serial.print("Used voice: ");
						Serial.println(tempNote.voice);
						
					}
					else
					{
						//No new voices
					}
					//record
					noteOnInList.pushObject( tempNote );
					
					rxNoteList.dropObject( unservicedNoteCount - 1 );
				}
				else
				{
					//Was found
					//do nothing
				}
			}
			else if( tempNote.eventType == 0x80 )
			{
				//Congratulations! It's a note off!
				//Search for the note on.  If found, do nothing, else write
				int8_t tempSeekDepth = noteOnInList.seekObjectbyNoteValue( tempNote );
				if( tempSeekDepth == -1 )
				{
					//not found.
					//Do nothing
					
				}
				else
				{
					//Was found.  Time for note off actions
					Serial.print("Dropping ");
					Serial.println( tempSeekDepth );
					//Find the note and learn its voice
					
					/////This is where we might turn the note off -- repurpose tempNote
					tempNote = *noteOnInList.readObject( tempSeekDepth );
					voicesUsed[tempNote.voice] = 0;
					//mixer5.gain(tempNote.voice, 0);
					switch( tempNote.voice )
						{
							case 0:
							bendvelope1.noteOff();
							break;
							case 1:
							bendvelope2.noteOff();
							break;
							case 2:
							bendvelope3.noteOff();
							break;
							case 3:
							bendvelope4.noteOff();
							break;
							default:
							break;
						}
					Serial.print("Voice silenced: ");
					Serial.println(tempNote.voice);					
					
					noteOnInList.dropObject( tempSeekDepth );

					rxNoteList.dropObject( unservicedNoteCount - 1 );
				}				
			}
			else
			{
				//Destroy the crappy data!
				rxNoteList.dropObject( unservicedNoteCount );
			}
		}
		//else we no new data!
		else
		{
			
		}
	}
	
	
	//**Fast LED toggling of the panel class***********//  
	if(ledToggleFastTimer.flagStatus() == PENDING)
	{
		myLooperPanel.toggleFastFlasherState();
		
	}

	//**LED toggling of the panel class***********//  
	if(ledToggleTimer.flagStatus() == PENDING)
	{
		myLooperPanel.toggleFlasherState();
		
	}
	//**Debug timer*******************************//  
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
	}
	midiA.read();

	
}

//uint32_t FreeRam(){ // for Teensy 3.0
//    uint32_t stackTop;
//    uint32_t heapTop;
//
//    // current position of the stack.
//    stackTop = (uint32_t) &stackTop;
//
//    // current position of heap.
//    void* hTop = malloc(1);
//    heapTop = (uint32_t) hTop;
//    free(hTop);
//
//    // The difference is the free, available ram.
//    return stackTop - heapTop;
//}

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

uint32_t tapTempoTimerMath( uint16_t BPMInput )
{
	uint32_t returnVar = 0;
	
	returnVar = 2500000 /( (uint32_t)BPMInput );
	return returnVar;
}