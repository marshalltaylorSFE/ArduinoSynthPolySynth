#ifndef effect_bendvelope_h_
#define effect_bendvelope_h_
#include "AudioStream.h"
#include "utility/dspinst.h"

#define SAMPLES_PER_MSEC (AUDIO_SAMPLE_RATE_EXACT/1000.0)

class AudioEffectBendvelope : public AudioStream
{
public:
	AudioEffectBendvelope() : AudioStream(1, inputQueueArray) {
		state = 0;
		delay(0.0);  // default values...
		attack(1.5);
		hold(0.5);
		decay(15.0);
		sustain(0.667);
		release(30.0);
	}
	void noteOn();
	void noteOff();
	void delay(float milliseconds) {
		delay_count = milliseconds2count(milliseconds);
	}
	void attack(float milliseconds) {
		attack_count = milliseconds2count(milliseconds);
	}
	void hold(float milliseconds) {
		hold_count = milliseconds2count(milliseconds);
	}
	void decay(float milliseconds) {
		decay_count = milliseconds2count(milliseconds);
	}
	void sustain(float level) {
		if (level < 0.0) level = 0;
		else if (level > 1.0) level = 1.0;
		sustain_mult = level * 65536.0;
	}
	void release(float milliseconds) {
		release_count = milliseconds2count(milliseconds);
	}
	using AudioStream::release;
	virtual void update(void);
private:
	uint16_t milliseconds2count(float milliseconds) {
		if (milliseconds < 0.0) milliseconds = 0.0;
		uint32_t c = ((uint32_t)(milliseconds*SAMPLES_PER_MSEC)+7)>>3;
		if (c > 1103) return 1103; // allow up to 200 ms
		return c;
	}
	audio_block_t *inputQueueArray[1];
	// state
	uint8_t  state;  // idle, delay, attack, hold, decay, sustain, release
	uint16_t count;  // how much time remains in this state, in 8 sample units
	int32_t  mult;   // attenuation, 0=off, 0x10000=unity gain
	int32_t  inc;    // amount to change mult on each sample
	
	// settings
	uint16_t delay_count;
	uint16_t attack_count;
	uint16_t hold_count;
	uint16_t decay_count;
	int32_t  sustain_mult;
	uint16_t release_count;

};

#undef SAMPLES_PER_MSEC
#endif
