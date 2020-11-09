
/*
 *  Copyright 2020 Brian Burrell
 *
 *  This file is part of libhtm.
 *  (https://github.com/neonatura/zio)
 *
 *  libhtm is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Shionbot is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with libhtm.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "htm.h"

#if 0
#define MIN_WEIGHT 0
#define MID_WEIGHT 0x80
#define MAX_WEIGHT 0xFF
#endif

const char *htm_chord_hex(chord_t *hash)
{
	static char ret_buf[4096];
	int i;

	memset(ret_buf, 0, sizeof(ret_buf));
	for (i = 0; i < CHORD_SIZE; i++)
		sprintf(ret_buf+strlen(ret_buf), "%-2.2x", (unsigned int)hash->vec[i]);

	return ((const char *)ret_buf);
}

#if 0
uint64_t htm_chord_csum(uint8_t *data, size_t data_len)
{
	return (crc64(0, data, data_len));
}
#endif

#if 0
double htm_chord_weight(chord_t *hash)
{
	double weight;
	int i;

	for (i = 0; i < CHORD_SIZE; i++) {
		weight += hash->weight[i];
	}
	weight /= CHORD_SIZE;

	return (weight);
}
#endif

#if 0
void htm_chord_weight_apply(chord_t *hash)
{
	htime diff;
	htime now;
	int i;

	now = htime_now();
	diff = (now - hash->stamp) * 10; /* 1ms */

	if (diff != 0) {
		for (i = 0; i < CHORD_SIZE; i++) {
			if (hash->weight[i] <= diff) {
				hash->vec[i] = 0;
				hash->weight[i] = 0;
			} else {
				hash->weight[i] = hash->weight[i] - diff;
			}
		}

		hash->stamp = now;
	}

}

void htm_chord_weight_incr(chord_t *hash, int idx)
{
	hash->weight[idx] = MIN(255, (int)hash->weight[idx] + 32);
}

void htm_chord_weight_decr(chord_t *hash, int idx)
{
	hash->weight[idx] = MAX(0, (int)hash->weight[idx] - 32);
}
#endif

#if 0
void htm_chord_hash_init(chord_t *hash, chord_t *in_hash)
{
	int i;

	if (!in_hash)
		return;

	for (i = 0; i < CHORD_SIZE; i++) {
		if (in_hash->vec[i] != 0) {
			hash->vec[i] = 0xFF;
//			hash->weight[i] = MIN(255, (int)hash->weight[i] + 16);
		}
	}
}

void htm_chord_hash_pool(chord_t *hash, chord_t *in_hash)
{
	int i;

	for (i = 0; i < CHORD_SIZE; i++) {
		if (in_hash->vec[i] == 0 || /* unset */
				in_hash->vec[i] == 0xFF) /* unclear */
			continue;
		if (hash->vec[i] != 0xFF)
			hash->vec[i] |= in_hash->vec[i]; /* OR bits */
		else
			hash->vec[i] = in_hash->vec[i]; /* OR bits */
		hash->weight[i] = MIN(255, (int)hash->weight[i] + 1);
		//hash->weight[i] = MIN(255, (int)hash->weight[i] + 16);
	}

//	htm_chord_inhibit(hash);

}

void htm_chord_set(chord_t *hash, chord_t *in_hash)
{
	int i;

	if (!in_hash)
		return;

	for (i = 0; i < CHORD_SIZE; i++) {
		if (in_hash->vec[i] == 0)
			continue;

		/* clear all bits except for input */
		hash->vec[i] = in_hash->vec[i];
		//hash->weight[i] = MIN(255, (int)hash->weight[i] + 16);
		hash->weight[i] = MIN(255, (int)hash->weight[i] + 1);
	}

}
#endif

#if 0
int htm_chord_match(chord_t *ch, chord_t *hash)
{
	int match;
	int i;

	match = 0;
	for (i = 0; i < CHORD_SIZE; i++) {
		if (hash->vec[i] == 0)
			continue;
		if (ch->vec[i] & hash->vec[i]) {
			match++;
			if (match == 5)
				return (TRUE);
		}
	}

	return (FALSE);
}
#endif

static double hyper_sigmoid(double x)
{
  double a = (1 - exp(-2 * x));
  double b = (1 + exp(-2 * x));
  return (a / b);
}

static double sigmoid(double x)
{
	return (1 / (1 + exp(-x)));
}

double htm_chord_compare(chord_t *ch, chord_t *hash)
{
	double match;
	int bit1, bit2;
	int bit;
	int i;

	match = 0;
	for (i = 0; i < CHORD_SIZE; i++) {
		for (bit = 0; bit < 8; bit++) {
			bit1 = (ch->vec[i] & (1 << bit));
			bit2 = (hash->vec[i] & (1 << bit));
//			if ( !((ch->vec[i] & (1 << bit))) && !((hash->vec[i] & (1 << bit))) )

			if (bit1 && bit2)
				match += 0.125; /* 1/8 */
			else if (bit1 || bit2)
				match -= 0.125; /* 1/8 */
		}
	}

//fprintf(stderr, "DEBUG: htm_chord_match: match = %f\n", match); 
	return (hyper_sigmoid(match));
}

int htm_chord_match(chord_t *ch, chord_t *hash)
{
	if (!ch || !hash)
		return (0);
	return (htm_chord_compare(ch, hash) >= 0.75); /* 6/8 */
}

uint64_t htm_chord_compact(chord_t *hash)
{
	uint64_t ret_csum;
	uint8_t *csum_raw;
	int idx;
	int bit;
	int i;
	int j;

	ret_csum = 0;
	csum_raw = (uint8_t *)&ret_csum;

	idx = 0;
	if (hash) {
		for (i = (CHORD_SIZE-1); i >= 0; i--) {
			if (hash->vec[i] == 0 || /* unset */
					hash->vec[i] == 0xFF) /* uncertain */
				continue;



			
			bit = (i * 8);
			int t_bit = -1;
			for (j = 7; j >= 0; j--) {
				if (hash->vec[i] & (1 << j)) {
					bit += j; 
					break;
				}
			}
			if (j == -1)
				continue;

			csum_raw[idx] = bit;
			idx++;
			if (idx == 8)
				break;
		}
	}

	return (ret_csum);
}

void htm_chord_expand(chord_t *hash, uint64_t csum)
{
	uint8_t *val = (uint8_t *)&csum;
	int pos;
	int bit;
	int i;

	for (i = 0; i < 8; i++) {
		bit = val[i] % 8;
		pos = val[i] / 8;
		hash->vec[pos] |= (1 << bit);
	}

}

#if 0
int htm_chord_spike(chord_t *hash)
{
	int idx;

	for (idx = 0; idx < CHORD_SIZE; idx++) {
		if (hash->weight[idx] >= 0x80)
			return (TRUE);
	}

	return (FALSE);
}

void htm_chord_inhibit(chord_t *hash)
{
	int idx;

	for (idx = 0; idx < CHORD_SIZE; idx++) {
		hash->weight[idx] /= 2;
	}

}
#endif

#if 0
/** Compact the hash into a number from 0 to 1.0 */
double htm_chord_value(chord_t *hash)
{
	double val;
	int i;

	val = 0;
	for (i = 0; i < CHORD_SIZE; i++) {
		val *= 0.0039;
		val = (hash->vec[i] / 256);
	}

	return (val);
}
#endif

#if 0
static void *_memxor(void *dest, const void *src, size_t n)
{
  char const *s = (const char *)src;
  char *d = (char *)dest;

  for (; n > 0; n--)
    *d++ ^= *s++;

  return dest;
}

void htm_chord_digest(chord_t *hash, uint8_t *data, size_t data_len)
{
	char buf[CHORD_SIZE];
	size_t of;

	for (of = 0; of < data_len; of += CHORD_SIZE) {
		_memxor((uint8_t *)hash->vec, data + of, MIN(data_len - of, CHORD_SIZE));
	}

}
#endif

#if 0
int htm_chord_digest_bit(uint8_t ch)
{
	uint32_t ret_val;
	int nr;
	int k;

	nr = 0;
	ret_val = 0;
	for (k = 7; k >= 0; k--) {
		if (ch & (1 << k)) {// & ch) {
			ret_val |= (1 << k);
			nr++;
			if (nr >= 3)
				break;
		}
	}

	return (ret_val);
}
#endif

static uint8_t select_bits(uint8_t val)
{
  uint8_t ret_val;
  int tot;
  int i;

	tot = 0;
	ret_val = 0;
	if (val) {
		for (i = 7; i >= 0; i--) {
			if (val & (1 << i)) {
				ret_val |= (1 << i);

				tot++;
				if (tot >= 3)
					break;
			}
		}
	}

  return (ret_val);
}

uint8_t htm_chord_select(uint8_t val)
{
  uint8_t ret_val;
  int tot;
  int i;

	tot = 0;
	ret_val = 0;
	if (val) {
		for (i = 7; i >= 0; i--) {
			if (val & (1 << i)) {
				ret_val |= (1 << i);

				tot++;
				if (tot >= 3)
					break;
			}
		}
	}

  return (ret_val);
}

int htm_chord_digest(chord_t *hash, uint32_t *seed_p, uint8_t *data, size_t data_len)
{
	unsigned char buf[CHORD_SIZE];
	uint32_t col_idx;
	uint32_t bit_seed;
	uint32_t bit;
	int is_bit;
	int idx;
	int of;

	is_bit = 0;
	col_idx = 0;
	bit_seed = 1;
	for (of = 0; of < data_len; of += CHORD_SIZE) {
		memset(buf, 0, sizeof(buf));
		memcpy(buf, data + of, MIN(CHORD_SIZE, data_len - of));

		/* determine activated columns. */
		for (idx = 0; idx < CHORD_SIZE; idx++) {
			col_idx = (of + idx) % CHORD_SIZE;

			if (buf[idx] == 0) {
//				hash->bias[col_idx] = MAX(0, hash->bias[col_idx]-1);
				continue;
			}

			bit = select_bits(buf[idx]);
			{
				uint32_t ival = (uint32_t)buf[idx];
			}
			if (bit) is_bit = 1;
			hash->vec[col_idx] |= bit;
//			hash->bias[col_idx] = MIN(255, hash->bias[col_idx]+1);
		}
	}

	return (is_bit);
}

#if 0
/* set in_hash onto hash */
void htm_chord_set(chord_t *hash, chord_t *in_hash)
{
	int i;

	if (!in_hash)
		return;

	for (i = 0; i < CHORD_SIZE; i++) {
		if (in_hash->vec[i] == 0xFF) /* unclear */
			continue;
		if (in_hash->vec[i] == 0) { /* unset */
//			hash->bias[i] = MAX(0, hash->bias[i] - 1);
			continue;
		}


		hash->vec[i] |= in_hash->vec[i];
#if 0
		if (hash->vec[i] != 0) {
			/* clear all bits except for input */
			hash->vec[i] = in_hash->vec[i];
		} else {
			/* prediction mode -- hunt for matches. */
			hash->vec[i] = 0xFF;
		}
#endif

		/* add activity weight */
		htm_chord_weight_incr(hash, i);
//		hash->bias[i] = MIN(255, hash->bias[i] + 1);
	}

}
void htm_chord_reset(chord_t *hash, chord_t *in_hash)
{
	int i;

	if (!in_hash)
		return;

	for (i = 0; i < CHORD_SIZE; i++) {
		if (in_hash->vec[i] == 0) { /* unset */
			continue;
		}

		if (hash->vec[i] != 0) {
			/* prediction mode -- hunt for matches. */
			hash->vec[i] = 0xFF;
		} else {
			hash->vec[i] |= in_hash->vec[i];
		}

		/* add activity weight */
		htm_chord_weight_incr(hash, i);
	}

}
/* absorb in_hash into hash */
void htm_chord_pool(chord_t *hash, chord_t *in_hash)
{
	int i;

	for (i = 0; i < CHORD_SIZE; i++) {
		if (in_hash->vec[i] == 0 || /* unset */
				in_hash->vec[i] == 0xFF) /* unclear */
			continue;

#if 0
		/* only apply from 'active' columns. */
		if (in_hash->weight[i] < MID_WEIGHT)
			break;
#endif

		hash->vec[i] |= in_hash->vec[i]; /* OR bits */
#if 0
		htm_chord_weight_incr(hash, i);
#endif
	}

}
void htm_chord_feedback(chord_t *hash, chord_t *in_hash)
{
  int i;
  
  for (i = 0; i < CHORD_SIZE; i++) {
    if (in_hash->vec[i] == 0) {
      htm_chord_weight_decr(hash, i);
    } else {
      htm_chord_weight_incr(hash, i);
    }
  }

}
#endif

static void *_memxor(void *dest, const void *src, size_t n)
{
  char const *s = (const char *)src;
  char *d = (char *)dest;

  for (; n > 0; n--)
    *d++ ^= *s++;

  return dest;
}
void htm_chord_xor(chord_t *hash, chord_t *in_hash)
{
	int i;

	if (!in_hash)
		return;

	_memxor(hash->vec, in_hash->vec, CHORD_SIZE);
}

void htm_chord_merge(chord_t *hash, chord_t *in_hash)
{
	int i;

	for (i = 0; i < CHORD_SIZE; i++) {
		hash->vec[i] |= in_hash->vec[i]; /* add flags set */
	}

}

void htm_chord_unset(chord_t *hash, chord_t *in_hash)
{
	int i;

	for (i = 0; i < CHORD_SIZE; i++) {
		hash->vec[i] &= ~in_hash->vec[i]; /* remove flags set */
	}

}

void htm_chord_reduce(chord_t *hash, double deg)
{
	uint8_t val;
	int i;

	deg = MAX(0.0, deg);
	deg = MIN(1.0, deg);
	val = (uint8_t)((double)256 * deg);

	for (i = 0; i < CHORD_SIZE; i++) {
		if (hash->vec[i] & val)
			hash->vec[i] &= ~val;
	}

}

#if 0
#define MAX_INSET_CORD 10240
uint64_t htm_chord_inset(uint64_t seed, double x, double y, double z)
{
	chord_t hash;
	double ar[4];

	ar[0] = (x / MAX_INSET_CORD);
	ar[1] = (y / MAX_INSET_CORD);
	ar[2] = (z / MAX_INSET_CORD);
	ar[3] = 0;

	memset(&hash, 0, sizeof(hash));
	htm_chord_split_set(&hash, 4, ar, MIN_CHORD_SPLIT_SIZE);
	return (htm_chord_compact(&hash));
}
#endif

void htm_chord_split_get(chord_t *hash, int nr, double *val_p, double max_val)
{
	uint64_t val;
	uint64_t span;
	int bit_idx;
	int idx;
	int of;
	int i;

	span = CHORD_SIZE / MAX(3, nr);
	max_val = (double)UINT64_MAX / span;

	idx = 0;
	for (of = 0; of < CHORD_SIZE; of += span) {
		val = 1;

		for (i = 0; i < span && i < 16; i++) {
			bit_idx = (of + i) % CHORD_SIZE;
			val = val << 8;
			val += select_bits(hash->vec[bit_idx]);
		}
		val_p[idx++] = (double)val / max_val;

		if (idx >= nr)
			break;
	}

}

void htm_chord_split_set(chord_t *hash, int nr, double *val_p, double max_val)
{
	uint64_t val;
	uint64_t span;
	int bit_idx;
	int idx;
	int tot;
	int of;
	int i;

	span = CHORD_SIZE / MAX(3, nr);
	max_val = (double)UINT64_MAX / span;

	memset(hash, 0, sizeof(chord_t));

	idx = 0;
	for (of = 0; of < CHORD_SIZE; of += span) {
		val = (uint64_t)(val_p[idx++] * max_val);

		i = MIN(15, (span - 1));
		for (; i >= 0; i--) {
			bit_idx = of + i;
			hash->vec[bit_idx] = select_bits(val & 0xFF);
			val = val >> 8;
		}

		if (idx >= nr)
			break;
	}

}

void htm_chord_rgba(chord_t *hash, uint8_t *rgba)
{
  double clr[4];

  htm_chord_split_get(hash, 4, clr, MIN_CHORD_SPLIT_SIZE);

  rgba[0] = MIN(255, (256.0 * clr[0]));
  rgba[1] = MIN(255, (256.0 * clr[1]));
  rgba[2] = MIN(255, (256.0 * clr[2]));
  rgba[3] = MIN(255, (256.0 * clr[3]));

}

void htm_chord_rgba_set(chord_t *hash, uint8_t *rgba)
{
  double clr[4];

  clr[0] = (double)rgba[0] / 256.0;
  clr[1] = (double)rgba[1] / 256.0;
  clr[2] = (double)rgba[2] / 256.0;
  clr[3] = (double)rgba[3] / 256.0;
  htm_chord_split_set(hash, 4, clr, MIN_CHORD_SPLIT_SIZE);
}

