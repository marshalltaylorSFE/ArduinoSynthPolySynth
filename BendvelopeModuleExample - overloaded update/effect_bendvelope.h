#ifndef effect_bendvelope_h_
#define effect_bendvelope_h_
#include "AudioStream.h"
#include "utility/dspinst.h"

#define SAMPLES_PER_MSEC (AUDIO_SAMPLE_RATE_EXACT/1000.0)

#include "stdint.h"



class RateParameter
{
public:
  //Constructor
  RateParameter( void );
//Variables
  uint16_t timeScale;
  int8_t powerScale;


};

class LevelParameter
{
public:
  //Constructor
  LevelParameter( void );
  uint8_t getLevel( void );
  //Variables
  uint8_t level;

};

class TimeKeeper32
{
public:
    TimeKeeper32( void );
    void uClear( void );
    uint32_t uGet( void );
    void uIncrement( uint32_t );

private:
    uint32_t timeElapsed;

};


class AudioEffectBendvelope : public AudioStream
{
public:
	AudioEffectBendvelope();
	//Functions
	void noteOn();
	void noteOff();
	void attack( uint8_t, int8_t );
	void decay( uint8_t, int8_t );
	uint16_t getDecay( void );
	void sustain( uint8_t );
	void release( uint8_t, int8_t );
	void setAttackHold( uint8_t );
	//RateParameter getAttack( void );
	//RateParameter getDecay( void );
	//RateParameter getRelease( void );

	using AudioStream::release;
	virtual void update(void);
private:

	audio_block_t *inputQueueArray[1];
	// state
	uint8_t  state;  // idle, delay, attack, hold, decay, sustain, release
	//uint16_t count;  // how much time remains in this state, in 8 sample units
	int32_t  mult;   // attenuation, 0=off, 0x10000=unity gain
	
	uint8_t noteState;
	uint8_t amp;
	uint8_t shadowAmp;
  
	void changeAmp( RateParameter&, uint16_t, uint8_t, uint8_t& );
	void changeAmp( LevelParameter&, uint8_t& );
	void changeAmp( uint8_t, uint8_t& );
	
	//Timekeepers are modified logical counter modules
	TimeKeeper32 mainTimeKeeper;
	TimeKeeper32 shadowTimeKeeper;
	
	RateParameter envAttack;
	//RateParameter envAttackShadow;
	RateParameter envDecay;
	//RateParameter envDecayShadow;
	LevelParameter envSustain;
	RateParameter envRelease;
	//RateParameter envReleaseShadow;
public:
	RateParameter envAttackHold;
 
};

#undef SAMPLES_PER_MSEC
#endif
