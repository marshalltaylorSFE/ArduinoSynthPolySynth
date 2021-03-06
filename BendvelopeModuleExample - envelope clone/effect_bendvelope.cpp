#include "effect_bendvelope.h"

#define STATE_IDLE	0
#define STATE_DELAY	1
#define STATE_ATTACK	2
#define STATE_HOLD	3
#define STATE_DECAY	4
#define STATE_SUSTAIN	5
#define STATE_RELEASE	6

void AudioEffectBendvelope::noteOn(void)
{
	__disable_irq();
	mult = 0;
	count = delay_count;
	if (count > 0) {
		state = STATE_DELAY;
		inc = 0;
	} else {
		state = STATE_ATTACK;
		count = attack_count;
		inc = (0x10000 / count) >> 3;
	}
	__enable_irq();
}

void AudioEffectBendvelope::noteOff(void)
{
	__disable_irq();
	state = STATE_RELEASE;
	count = release_count;
	mult = sustain_mult;
	inc = (-mult / ((int32_t)count << 3));
	__enable_irq();
}

void AudioEffectBendvelope::update(void)
{
	audio_block_t *block;
	uint32_t *p, *end;
	uint32_t sample12, sample34, sample56, sample78, tmp1, tmp2;

	block = receiveWritable();
	if (!block) return;
	if (state == STATE_IDLE) {
		release(block);
		return;
	}
	p = (uint32_t *)(block->data);
	end = p + AUDIO_BLOCK_SAMPLES/2;

	while (p < end) {
		// we only care about the state when completing a region
		if (count == 0) {
			if (state == STATE_ATTACK) {
				count = hold_count;
				if (count > 0) {
					state = STATE_HOLD;
					mult = 0x10000;
					inc = 0;
				} else {
					count = decay_count;
					state = STATE_DECAY;
					inc = ((sustain_mult - 0x10000) / ((int32_t)count << 3));
				}
				continue;
			} else if (state == STATE_HOLD) {
				state = STATE_DECAY;
				count = decay_count;
				inc = ((sustain_mult - 0x10000) / (int32_t)count) >> 3;
				continue;
			} else if (state == STATE_DECAY) {
				state = STATE_SUSTAIN;
				count = 0xFFFF;
				mult = sustain_mult;
				inc = 0;
			} else if (state == STATE_SUSTAIN) {
				count = 0xFFFF;
			} else if (state == STATE_RELEASE) {
				state = STATE_IDLE;
				while (p < end) {
					*p++ = 0;
					*p++ = 0;
					*p++ = 0;
					*p++ = 0;
				}
				break;
			} else if (state == STATE_DELAY) {
				state = STATE_ATTACK;
				count = attack_count;
				inc = (0x10000 / count) >> 3;
				continue;
			}
		}
		// process 8 samples, using only mult and inc
		sample12 = *p++;
		sample34 = *p++;
		sample56 = *p++;
		sample78 = *p++;
		p -= 4;
		mult += inc;
		tmp1 = signed_multiply_32x16b(mult, sample12);
		mult += inc;
		tmp2 = signed_multiply_32x16t(mult, sample12);
		sample12 = pack_16b_16b(tmp2, tmp1);
		mult += inc;
		tmp1 = signed_multiply_32x16b(mult, sample34);
		mult += inc;
		tmp2 = signed_multiply_32x16t(mult, sample34);
		sample34 = pack_16b_16b(tmp2, tmp1);
		mult += inc;
		tmp1 = signed_multiply_32x16b(mult, sample56);
		mult += inc;
		tmp2 = signed_multiply_32x16t(mult, sample56);
		sample56 = pack_16b_16b(tmp2, tmp1);
		mult += inc;
		tmp1 = signed_multiply_32x16b(mult, sample78);
		mult += inc;
		tmp2 = signed_multiply_32x16t(mult, sample78);
		sample78 = pack_16b_16b(tmp2, tmp1);
		*p++ = sample12;
		*p++ = sample34;
		*p++ = sample56;
		*p++ = sample78;
		count--;
	}
	transmit(block);
	release(block);
}

