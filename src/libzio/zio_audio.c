/* Copyright 2018 Neo Natura */

#include "zio.h"

#ifdef HAVE_LIBESPEAK 
#include <espeak/speak_lib.h>
#endif

#define BEEP_SAMPLES 21
static const uint16_t _beep_samples[BEEP_SAMPLES] = {
        NOTE_C4, NOTE_E4, NOTE_G4, NOTE_G3,
        NOTE_C4, NOTE_G3, NOTE_E3,
        NOTE_A3, NOTE_B3, NOTE_B3, NOTE_A3,
        NOTE_G3, NOTE_E4, NOTE_G4, NOTE_A4,
        NOTE_F4, NOTE_G4, NOTE_E4, NOTE_C4, NOTE_D4, NOTE_B3
};

#define NEGATIVE_BEEP_SAMPLES 6
static const uint16_t _negative_beep_samples[NEGATIVE_BEEP_SAMPLES] = {
    NOTE_D5, NOTE_D5,
    NOTE_D4, NOTE_D3, 
		NOTE_D3, NOTE_F2
};

#define POSITIVE_BEEP_SAMPLES 14
static const uint16_t _positive_beep_samples[POSITIVE_BEEP_SAMPLES] = {
	NOTE_B4, NOTE_B4, 0, 0, NOTE_B4, NOTE_B4,
	NOTE_A4, NOTE_A4, 0, 0, NOTE_A4, NOTE_A4,
	NOTE_D5, NOTE_D5
};

//#define INTRO_SAMPLES 38
#define INTRO_SAMPLES 9
static const uint16_t _intro_samples[INTRO_SAMPLES] = {
	NOTE_A4, //NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, 0, 0,

	NOTE_AS4, //NOTE_AS4, NOTE_AS4,
	NOTE_A3, //NOTE_A3, NOTE_A3, 
	NOTE_A3, //NOTE_A3, NOTE_A3, 0, 0,

	NOTE_G3, //NOTE_G3,NOTE_G3,
	NOTE_G4, //NOTE_G4, NOTE_G4,
	NOTE_G4, //NOTE_G4, NOTE_G4, 0, 0,

	NOTE_A5, //NOTE_A5, NOTE_A5,
	NOTE_A5 //NOTE_A5, NOTE_A5, 0, 0,
};

int zio_audio_intro(void)
{
	zdev_t *mod;

	mod = zio_mod_get(ZMOD_INTERNAL, ZDEV_AUDIO);

	return (zio_write16_r(mod,
		(uint8_t *)_intro_samples, INTRO_SAMPLES*2, ZIO_AUDIO_64MS));
}

int zio_beep(void)
{
	zdev_t *mod;

	mod = zio_mod_get(ZMOD_INTERNAL, ZDEV_AUDIO);
	if (!mod)
		return (ZERR_INVAL);

	return (zio_write16_r(mod,
		(uint8_t *)_beep_samples, BEEP_SAMPLES*2, ZIO_AUDIO_32MS));
}

int zio_beep_negative(void)
{
	zdev_t *mod;

	mod = zio_mod_get(ZMOD_INTERNAL, ZDEV_AUDIO);
	if (!mod)
		return (ZERR_INVAL);

	return (zio_write16_r(mod,
		(uint8_t *)_negative_beep_samples, NEGATIVE_BEEP_SAMPLES*2, ZIO_AUDIO_64MS));
}

int zio_beep_positive(void)
{
	zdev_t *mod;

	mod = zio_mod_get(ZMOD_INTERNAL, ZDEV_AUDIO);
	if (!mod)
		return (ZERR_INVAL);

	return (zio_write16_r(mod,
		(uint8_t *)_positive_beep_samples, POSITIVE_BEEP_SAMPLES*2, ZIO_AUDIO_64MS));
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

	return (zio_write16_r(mod, buff, len*2, ZIO_AUDIO_64MS));
}

#ifdef HAVE_LIBESPEAK 
static double espeak_buffer_srate;
static unsigned short _notes[4096];

static int zio_espeak_synth(short *data, int samples, espeak_EVENT *event)
{
	zdev_t *dev = (zdev_t *)event->user_data;
	double of;
	int idx;
	int err;

	memset(_notes, 0, sizeof(_notes));

	of = 0;
	idx = 0;
	while (of < samples) {
		_notes[idx] = (uint16_t)data[(int)of];

		idx++;
		if (idx >= 4096)
			break;

		of += espeak_buffer_srate;
	}

	if (idx > 0) {
		err = zio_write(dev, (uint8_t *)_notes, idx * sizeof(unsigned short));
		if (err)
			return (err);
	}
//zio_write(dev, (uint8_t *)data, samples*2);

  return (0);
}

static void zio_audio_etalk_init(void)
{
	double srate;


	srate = espeak_Initialize(AUDIO_OUTPUT_RETRIEVAL, 0, NULL, 0);
	espeak_buffer_srate = srate/5000; /* 4.41 */

  espeak_SetParameter(espeakRATE, 165, 0);
  espeak_SetParameter(espeakVOLUME, 100, 0);
  espeak_SetParameter(espeakPITCH, 80, 0);
  espeak_SetParameter(espeakWORDGAP, 11, 0);
  espeak_SetParameter(espeakRANGE, 60, 0);
  espeak_SetParameter(espeakPUNCTUATION, espeakPUNCT_NONE, 0);

  espeak_SetSynthCallback(zio_espeak_synth);
}

static void zio_audio_etalk_term(void)
{
	espeak_Terminate();
}

int zio_audio_etalk(zdev_t *dev, uint8_t *data, size_t data_len)
{
	static int init;
	zdev_t *mod;
  unsigned int id;
	int synth_flags;

	if (!init) {
		zio_audio_etalk_init();
		init = 1;
	}

	mod = zio_mod_get(ZMOD_INTERNAL, ZDEV_AUDIO);
	if (!mod)
		return (ZERR_OPNOTSUPP);

  synth_flags = espeakCHARS_AUTO;
	espeak_Synth(data, strlen(data)+1, 0, 
			POS_CHARACTER, 0, synth_flags, &id, mod);

//  espeak_Synchronize();

	return (0);
}
#else
int zio_audio_etalk(zdev_t *dev, uint8_t *data, size_t data_len)
{
	return (ZERR_OPNOTSUPP);
}
#endif

