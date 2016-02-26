#include "Panel.h"

//This is where PanelComponents are joined to form the custom panel
#define tapButtonPin 4


Panel::Panel( void )
{
	flasherState = 0;
	fastFlasherState = 0;
}

void Panel::init( void )
{
	button1.init(1);
	
 	flasherState = 0;
	fastFlasherState = 0;
}

void Panel::update( void )
{
	button1.update();

}

void Panel::toggleFlasherState( void )
{
	flasherState ^= 0x01;
}

void Panel::toggleFastFlasherState( void )
{
	fastFlasherState ^= 0x01;
}