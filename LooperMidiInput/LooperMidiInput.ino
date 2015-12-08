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

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
	rxLedFlag = 1;
	MidiEvent tempEvent;

	tempEvent.eventType = 0x90;
	tempEvent.channel = channel;
	tempEvent.value = pitch;
	tempEvent.data = velocity;
		
	rxNoteList.pushObject( tempEvent );
	
	//midiA.sendNoteOn(pitch, velocity, channel);
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
	rxLedFlag = 1;
	
	MidiEvent tempEvent;

	tempEvent.eventType = 0x80;
	tempEvent.channel = channel;
	tempEvent.value = pitch;
	tempEvent.data = 0;

	rxNoteList.pushObject( tempEvent );
	
	//midiA.sendNoteOff(pitch, velocity, channel);
	
}


// -----------------------------------------------------------------------------
void setup() 
{
	//Initialize serial:
	Serial.begin(9600);
	delay(2000);
	Serial.println("Program Started");
	
	//Init panel.h stuff
	myLooperPanel.init();
	
	// initialize IntervalTimer
	myTimer.begin(serviceUS, 1);  // serviceMS to run every 0.001 seconds
	
	//Update the panel
	myLooperPanel.update();

	//Connect MIDI handlers
	midiA.setHandleNoteOn(handleNoteOn);  // Put only the name of the function
	midiA.setHandleNoteOff(handleNoteOff);
	
	// Initiate MIDI communications, listen to all channels
	midiA.begin(MIDI_CHANNEL_OMNI);
	//midiA.turnThruOn();
	midiA.turnThruOff();
	
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

	debugTimer.update(usTicks);
	
	//**Copy to make a new timer******************//  
	//  if(msTimerA.flagStatus() == PENDING)
	//  {
	//    digitalWrite( LEDPIN, digitalRead(LEDPIN) ^ 1 );
	//  }
	
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
					//note not found.  record
					noteOnInList.pushObject( tempNote );
					////////This is where we might play the note
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
					noteOnInList.dropObject( tempSeekDepth );

					/////This is where we might turn the note off

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
		Serial.print("\n\nrxNoteList\n");
		rxNoteList.printfMicroLL();
		Serial.print("\n\nnoteOnInList\n");
		noteOnInList.printfMicroLL();
		Serial.print("\n\nnoteOnOutLists\n");

	}
	midiA.read();

	
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

uint32_t tapTempoTimerMath( uint16_t BPMInput )
{
	uint32_t returnVar = 0;
	
	returnVar = 2500000 /( (uint32_t)BPMInput );
	return returnVar;
}