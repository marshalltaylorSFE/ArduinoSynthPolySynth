//**********************************************************************//
#ifndef P8INTERFACE_H
#define P8INTERFACE_H

#include "stdint.h"
#include "timeKeeper.h"
#include "P8PanelComponents.h"
#include "P8Panel.h"
#include "flagMessaging.h"

enum PStates
{
	PInit,
	PIdle,

};

class P8Interface : public P8Panel
{
public:
	P8Interface( void );
	void reset( void );
	//State machine stuff  
	void processMachine( void );
	void tickStateMachine( void );

	void timersMIncrement( uint8_t );
	
	//Flags coming in from the system
	
	//Internal - and going out to the system - Flags
	
	//  ..and data.

private:
	//Internal Flags
	//MessagingFlag quantizeHoldOffFlag;
	//  ..and data

	//State machine stuff  
	PStates state;

};



#endif