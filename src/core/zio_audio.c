/* Copyright 2018 Neo Natura */

#include "zio.h"

#define BEEP_SAMPLES 14
static const uint16_t _beep_samples[BEEP_SAMPLES] = {
	NOTE_B4, NOTE_B4, 0, 0, NOTE_B4, NOTE_B4,
	NOTE_A4, NOTE_A4, 0, 0, NOTE_A4, NOTE_A4,
	NOTE_D5, NOTE_D5
};

#define NEGATIVE_BEEP_SAMPLES 14
static const uint16_t _negative_beep_samples[NEGATIVE_BEEP_SAMPLES] = {
    NOTE_D5, NOTE_D5,
    NOTE_D4, NOTE_D3, 0, 0, NOTE_D3, NOTE_D3,
    NOTE_F2, NOTE_F2, 0, 0, NOTE_F2, NOTE_F2
};

#define POSITIVE_BEEP_SAMPLES 28
static const uint16_t _positive_beep_samples[POSITIVE_BEEP_SAMPLES] = {
        NOTE_E4, NOTE_E4, NOTE_E4,
        NOTE_C4, NOTE_E4, NOTE_G4, NOTE_G3,
        0, 0,

        NOTE_C4, NOTE_G3, NOTE_E3,
        NOTE_A3, NOTE_B3, NOTE_B3, NOTE_A3,
        NOTE_G3, NOTE_E4, NOTE_G4, NOTE_A4,
        NOTE_F4, NOTE_G4, NOTE_E4, NOTE_C4, NOTE_D4, NOTE_B3,
        0, 0
};

#define INTRO_SAMPLES 38
static const uint16_t _intro_samples[INTRO_SAMPLES] = {
	NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4,
	0, 0,

	NOTE_AS4, NOTE_AS4, NOTE_AS4,
	NOTE_A3, NOTE_A3, NOTE_A3, NOTE_A3, NOTE_A3, NOTE_A3,
	0, 0,

	NOTE_G3, NOTE_G3,NOTE_G3,
	NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4,
	0, 0,

	NOTE_A5, NOTE_A5, NOTE_A5, NOTE_A5, NOTE_A5, NOTE_A5,
	0, 0,
};



int zio_audio_intro(void)
{
	zdev_t *mod;

	mod = zio_mod_get(ZMOD_INTERNAL, ZDEV_AUDIO);

	return (zio_write(mod,
		(uint8_t *)_intro_samples, INTRO_SAMPLES*2));
}

int zio_beep(void)
{
	zdev_t *mod;

	mod = zio_mod_get(ZMOD_INTERNAL, ZDEV_AUDIO);
	if (!mod)
		return (ZERR_INVAL);

	return (zio_write(mod,
		(uint8_t *)_beep_samples, BEEP_SAMPLES*2));
}

int zio_beep_negative(void)
{
	zdev_t *mod;

	mod = zio_mod_get(ZMOD_INTERNAL, ZDEV_AUDIO);
	if (!mod)
		return (ZERR_INVAL);

	return (zio_write(mod,
		(uint8_t *)_negative_beep_samples, NEGATIVE_BEEP_SAMPLES*2));
}

int zio_beep_positive(void)
{
	zdev_t *mod;

	mod = zio_mod_get(ZMOD_INTERNAL, ZDEV_AUDIO);
	if (!mod)
		return (ZERR_INVAL);

	return (zio_write(mod,
		(uint8_t *)_positive_beep_samples, POSITIVE_BEEP_SAMPLES*2));
}

int zio_audio_dtalk(uint8_t *data, size_t data_len)
{
	uint8_t buff[1024];
	uint16_t *ar = (uint16_t *)buff;
	zdev_t *mod;
	int len;

	mod = zio_mod_get(ZMOD_INTERNAL, ZDEV_AUDIO);
	if (!mod)
		return (ZERR_INVAL);

	len = 0;
	memset(buff, 0, sizeof(buff));
	for (len = 0; len < data_len && len < 512; len++) {
		ar[len] = ((unsigned int)data[len] % 42) * ((unsigned int)data[len] / 6) + NOTE_F1;
	}

	return (zio_write(mod, buff, len*2));
}

