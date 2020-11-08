
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



extern const lang_char lang_char_b;
extern const lang_char lang_char_c;
extern const lang_char lang_char_d;
extern const lang_char lang_char_e;





static unsigned int width = 16;
static unsigned int height = 16;

/*  Call this macro repeatedly.  After each use, the pixel data can be extracted  */

#define HEADER_PIXEL(data,pixel) {\
pixel[0] = (((data[0] - 33) << 2) | ((data[1] - 33) >> 4)); \
pixel[1] = ((((data[1] - 33) & 0xF) << 4) | ((data[2] - 33) >> 2)); \
pixel[2] = ((((data[2] - 33) & 0x3) << 6) | ((data[3] - 33))); \
data += 4; \
}
static const char *header_data =
  "````````````````````````````````````````````````````````````````"
  "````````!!!!!!!!!!!!!!!!````````````````````````````````````````"
  "````````!!!!!!!!!!!!!!!!````````````````````````````````````````"
  "````````````!!!!!!!!!!!!````````````````````````````````````````"
  "````````````````!!!!!!!!````````````````````````````````````````"
  "````````````````!!!!!!!!````!!!!!!!!!!!!!!!!!!!!````````````````"
  "````````````````!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!````````````"
  "````````````````!!!!!!!!!!!!!!!!````````!!!!!!!!!!!!!!!!````````"
  "````````````````!!!!!!!!````````````````````````!!!!!!!!````````"
  "````````````````!!!!!!!!````````````````````````!!!!!!!!!!!!````"
  "````````````````!!!!!!!!````````````````````````!!!!!!!!!!!!````"
  "````````````````!!!!!!!!````````````````````````!!!!!!!!````````"
  "````````````````!!!!!!!!!!!!````````````````!!!!!!!!!!!!````````"
  "````````!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!````````````"
  "````````!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!````````````````"
  "````````````````````````````````````````````````````````````````"
  "";


int test_net(entity_t *ent)
{
	brane_t *br_exec = htm_brane_get(ent, BR_EXEC);
	const char *input_text = "b";
	chord_t hash;
	path_t *p;
	cell_t *in[16];
	cell_t *mid[192];
	cell_t *out[96];
	uint64_t inset;
	uint64_t bias;
	int x, y;
	int i, j;

	/* input */
	for (i = 0; i < 16; i++) {
		in[i] = htm_cell_init(br_exec);
	}

	/* mid-layer */
	for (i = 0; i < 192; i++) {
		mid[i] = htm_cell_init(br_exec);
		htm_cell_bias_set(mid[i], i << 16);
	}

	/* connect input to mid 1:4 */
	for (i = 0; i < 16; i++) {
		for (j = 0; j < 192; j++) {
			if ((j % 4) == (i % 4)) {
				p = htm_path_new(br_exec, in[i], mid[j]);
				htm_path_weight_set(p, 1 / ((double)j / (double)i));
			}
		}
	}

	/* output */
	for (i = 0; i < 96; i++) {
		out[i] = htm_cell_init(br_exec);
	}

	/* connect mid to output 1:4 */
	for (i = 0; i < 192; i++) {
		for (j = 0; j < 96; j++) {
			if ((j % 4) == (i % 4)) {
				p = htm_path_new(br_exec, mid[i], out[j]);
				htm_path_weight_set(p, 1 / ((double)i / (double)j));
			}
		}
	}



	int k;
	for (k = 0; k < 16; k++) {
		/* provide each row of input. */
		uint8_t *data = (uint8_t *)header_data;
		chord_t inset_hash;
		int pixel[3];
		unsigned char val2;
		for (x = 0; x < width; x++) { 
			for (y = 0; y < width; y++) { 

				HEADER_PIXEL(data, pixel);
				val2 = (pixel[0] || pixel[1] || pixel[2]);

//				inset = htm_chord_inset(in[y]->id, x, y, 0);
				memset(&inset_hash, 0, sizeof(inset_hash));
				htm_chord_expand(&inset_hash, inset);
				htm_cell_motor_apply(ent, br_exec, in[y], &inset_hash, NULL);

				memset(&hash, 0, sizeof(hash));
				htm_chord_expand(&hash, crc64(0, &val2, sizeof(val2)));
				htm_cell_notify(ent, br_exec, in[y], &hash);
			}
		}
		usleep(1000);
	}

	/* all <-> all */
	for (k = 0; k < 16; k++) {
		/* inject result of input into mid-layer */
		for (i = 0; i < 16; i++) { 
			int j_st = MAX(0, MIN(160, (i*12) - 32));
			for (j = j_st; j < (j_st+64); j++) {
				uint64_t val = htm_chord_compact(htm_cell_object(in[i]));
				int idx;

				if (val == 0) continue;

				idx = j;
				if (idx < 0 || idx >= 192) continue;

fprintf(stderr, "DEBUG: MID: idx(%d) val(%llu)\n", idx, (unsigned long long)val);

				/* apply bias */
				val += htm_cell_bias(mid[idx]);

				memset(&hash, 0, sizeof(hash));
				htm_chord_expand(&hash, val);
				htm_cell_notify(ent, br_exec, mid[idx], &hash);
			}
		}
		usleep(1000);
	}

	for (k = 0; k < 32; k++) {

		/* inject mid results into output layer */
		for (i = 0; i < 192; i++) { 
			int j_st = MAX(0, MIN(160, (i/2) - 16));
			for (j = j_st; j < (j_st+32); j++) {
				uint64_t val = htm_chord_compact(htm_cell_object(mid[i]));
				int idx;

				if (val == 0) continue;

				idx = j;
				if (idx < 0 || idx >= 96) continue;
fprintf(stderr, "DEBUG: OUTPUT: idx(%d) val(%llu)\n", idx, (unsigned long long)val);

				/* apply bias */
				val += htm_cell_bias(out[idx]);


				memset(&hash, 0, sizeof(hash));
				htm_chord_expand(&hash, val);
				htm_cell_notify(ent, br_exec, out[idx], &hash);
			}
		}

		usleep(1000);

		
		for (i = 'a'; i <= 'z'; i++) {
			cell_t *cmp_cell = out[i-32];
			for (j = 'a'; j <= 'z'; j++) {
				if (i == j) continue;
				/* adjust bias */
				cell_t *o_cell = out[j - 32];
				layer_t *obj_l = &o_cell->layer[CELL_L5a]; /* Object (pool) */
#if 0
				if (htm_chord_weight(&obj_l->hash) >
						htm_chord_weight(htm_cell_object(cmp_cell))) {
					/* .. reduce path weight */
				}
#endif
			}

//			fprintf(stderr, "DEBUG: CHAR '%c': %s [value: %f] [weight: %f]\n", (char)i, htm_chord_hex(htm_cell_object(out[i-32])), htm_chord_value(htm_cell_object(out[i-32])), htm_chord_weight(htm_cell_object(out[i-32])));
		}
	}


 //htm_state_print();

return (0);
}


#if 0
void test_run(entity_t *ent)
{
	brane_t *br_exec = htm_brane_get(ent, BR_EXEC);
	cell_t *h1[CHORD_SIZE];
	cell_t *h2[CHORD_SIZE];
	cell_t *h3[96];
	cell_t *final;
	pos_t pos;
	int ok;
	int i;
	int j;
	int k;
	int x;

	final = htm_cell_init(br_exec);
	for (i = 0; i < CHORD_SIZE; i++) {
		h1[i] = htm_cell_init(br_exec);
	}

	for (i = 0; i < CHORD_SIZE; i++) {
		h2[i] = htm_cell_init(br_exec);
		for (j = 0; j < CHORD_SIZE; j++) {
			if (0 == ((i + j) % 2)) {
				/* <h1> -> *<h2> */
				htm_path_new(br_exec, h1[j], h2[i]);
			}
		}
	}

	for (i = 0; i < 96; i++) {
		h3[i] = htm_cell_init(br_exec);
		for (j = 0; j < CHORD_SIZE; j++) {
			if (0 == ((i + j) % 3)) {
				/* <h2> -> *<h2> */
				htm_path_new(br_exec, h2[j], h3[i]);
			}
		}
		/* <all> -> final */
		htm_path_new(br_exec, h3[i], final);
	}

	chord_t inset_hash;
	chord_t hash;
	uint64_t inset;
	uint32_t seed;
	int idx;
	uint32_t val;
	int y;
	char ch;



	/* #10000 iterations of each identical visual character. */
	for (ch = 'b'; ch <= 'c'; ch++) {
		const lang_char *lch = LANG_CHAR(ch);

		for (k = 0; k < 128; k++) {
			fprintf(stderr, "DEBUG: ITER #%d [CHAR: %c]\n", k, ch);

			idx = 0;
			seed = 0;
			memset(&pos, 0, sizeof(pos));
			for (y = 0; y < lch->height; y++) {
				size_t raw_of = (y * lch->width * lch->bytes_per_pixel); 
				const uint8_t *raw = (lch->pixel_data + raw_of);

//				inset = (uint64_t)y;
//				memset(&inset_hash, 0, sizeof(inset_hash));
				htm_chord_digest(&inset_hash, &seed, (uint8_t *)&inset, sizeof(inset));

				for (x = 0; x < lch->width; x++) {

					memset(&hash, 0, sizeof(hash));
					ok = htm_chord_digest(&hash, &seed, 
							(uint8_t *)raw + (x * lch->bytes_per_pixel), lch->bytes_per_pixel);
					if (!ok) 
						continue; /* blank */

					pos.loc_x = x;
					pos.loc_y = y;
					htm_cell_position_gen(NULL, &pos);
					memcpy(&inset_hash, &pos.loc_hash, sizeof(inset_hash));
					fprintf(stderr, "DEBUG: INSET: (x %d, y %d): %s\n", x, y, htm_chord_hex(&inset_hash));

					htm_cell_motor_apply(ent, br_exec, h1[idx % CHORD_SIZE], &inset_hash, NULL);

					htm_cell_notify(ent, br_exec, h1[idx % CHORD_SIZE], &hash);

					idx++;
				}
			}
		}

		{
			uint32_t val;
			int x, y;
			int i;
			int of;
			memset(&val, 0, sizeof(val));
			for (y = 0; y < lch->height; y++) {
				for (x = 0; x < lch->width; x++) {
					of = (y * lch->width) + x;
					of *= lch->bytes_per_pixel;
					memcpy(&val, lch->pixel_data + of, lch->bytes_per_pixel); 
					if (val == 0)
						printf (" ");
					else
						printf ("%c", ((val % 95) + 33));
				}
				printf ("\n");
			}
			printf ("\n");
		}
		{
			layer_t *obj_l = &final->layer[CELL_L5a]; /* Object (pool) */
			layer_t *exp_l = &final->layer[CELL_L1];

			fprintf(stderr, "DEBUG: FINAL OBJ HASH '%c': %s\n", ch, htm_chord_hex(&obj_l->hash));
			fprintf(stderr, "DEBUG: FINAL EXP HASH '%c': %s\n", ch, htm_chord_hex(&exp_l->hash));
		}

		sleep(1);
	}

}
#endif
void test_run(entity_t *ent)
{
	brane_t *br_vision = htm_brane_get(ent, BR_VISION);
	cell_t *h1[4096];
	cell_t *final;
	pos_t pos;
	int ok;
	int i;
	int j;
	int k;
	int x;
	chord_t inset_hash;
	chord_t hash;
	int idx;
	uint32_t val;
	int y;
	char ch;

	final = htm_cell_init(br_vision);
	for (i = 0; i < 4096; i++) {
		h1[i] = htm_cell_init(br_vision);
		/* <all> -> final */
//		htm_path_new(br_vision, h1[i], final);
	}




	/* 128 iterations of each identical visual character. */
	for (ch = 'b'; ch <= 'c'; ch++) {
		const lang_char *lch = LANG_CHAR(ch);

		for (k = 0; k < 128; k++) {
			fprintf(stderr, "DEBUG: ITER #%d [CHAR: %c]\n", k, ch);

			idx = 0;
//			seed = 0;
			memset(&pos, 0, sizeof(pos));
			for (y = 0; y < lch->height; y++) {
				size_t raw_of = (y * lch->width * lch->bytes_per_pixel); 
				const uint8_t *raw = (lch->pixel_data + raw_of);

//				inset = (uint64_t)y;
//				memset(&inset_hash, 0, sizeof(inset_hash));
//				htm_chord_digest(&inset_hash, &seed, (uint8_t *)&inset, sizeof(inset));

				for (x = 0; x < lch->width; x++) {

				
					htm_chord_rgba_set(&hash, (uint8_t *)raw + (x * lch->bytes_per_pixel));
#if 0
					memset(&hash, 0, sizeof(hash));
					ok = htm_chord_digest(&hash, &seed, 
							(uint8_t *)raw + (x * lch->bytes_per_pixel), lch->bytes_per_pixel);
					if (!ok) 
						continue; /* blank */
#endif

					pos.loc_x = x;
					pos.loc_y = y;
					htm_cell_position_gen(NULL, &pos);
					memcpy(&inset_hash, &pos.loc_hash, sizeof(inset_hash));
					//fprintf(stderr, "DEBUG: INSET: (x %d, y %d): %s\n", x, y, htm_chord_hex(&inset_hash));

					{
						int h_idx = (y * lch->width) + x;
						fprintf(stderr, "DEBUG: h_idx %d, h1[h_idx] {%x}\n", h_idx, h1[h_idx]);
						htm_cell_motor_apply(ent, br_vision, h1[h_idx], &inset_hash, NULL);
						//htm_cell_notify(ent, br_vision, h1[idx % CHORD_SIZE], &hash);
						htm_cell_notify(ent, br_vision, h1[h_idx], &hash);
					}

					idx++;
				}
			}
		}

		{
			uint32_t val;
			int x, y;
			int i;
			int of;
			memset(&val, 0, sizeof(val));
			for (y = 0; y < lch->height; y++) {
				for (x = 0; x < lch->width; x++) {
					of = (y * lch->width) + x;
					of *= lch->bytes_per_pixel;
					memcpy(&val, lch->pixel_data + of, lch->bytes_per_pixel); 
					if (val == 0)
						printf (" ");
					else
						printf ("%c", ((val % 95) + 33));
				}
				printf ("\n");
			}
			printf ("\n");
		}
#if 0
		{
			layer_t *obj_l = &final->layer[CELL_L5a]; /* Object (pool) */
			layer_t *exp_l = &final->layer[CELL_L1];

			fprintf(stderr, "DEBUG: FINAL OBJ HASH '%c': %s\n", ch, htm_chord_hex(&obj_l->hash));
			fprintf(stderr, "DEBUG: FINAL EXP HASH '%c': %s\n", ch, htm_chord_hex(&exp_l->hash));
		}
#endif

		sleep(1);
	}

}


int main(int argc, char *argv[])
{
	struct stat st;
	FILE *fl;
	entity_t *ent;
	chord_t hash;
	chord_t nhash;
	uint8_t *data;
	int fl_len;
	int err;

	if (argc > 1) {
		err = stat(argv[1], &st);
		if (err) {
			perror(argv[1]);
			return (1);
		}
		fl_len = st.st_size;
		
		fl = fopen(argv[1], "rb");
		if (!fl) {
			perror(argv[1]);
			return (1);
		}
		data = (uint8_t *)calloc(fl_len, sizeof(char));
		fread(data, sizeof(char), fl_len, fl); 
		fclose(fl);
	}




	err = htm_entity_init(&ent, "amica");
	if (err)
		exit(1);

#if 0
	{
		uint64_t csum;
		brane_t *br;
		chord_t inset_hash;
		cell_t *cell;
		cell_t *obj;
		uint64_t inset;
		int err;
		int k;
		int i;


		br = htm_brane_get(ent, BR_VISION);
		if (!br)
			exit(1);

		/* create obj encapsulating picture */
		obj = htm_cell_init(br);
//		htm_cell_notify(ent, obj, 0, 0, data, fl_len);
		cell = htm_cell_init(br);

		htm_cell_wake(ent, br, cell);

		uint32_t seed = 0;

//		for (k = 0; k < 16; k++) {
			for (i = 0; i < fl_len; i += 32) {
//				inset = htm_chord_inset(obj->id, (i/8), 0, 0);
				double x = (i % 136) / 16;
				double y = (i / 136) / 16;
				inset = (uint64_t)(sqrt((1 + x) * (1 + y)) * 4);
				memset(&inset_hash, 0, sizeof(inset_hash));
				htm_chord_digest(&inset_hash, &seed, (uint8_t *)&inset, sizeof(inset));
fprintf(stderr, "DEBUG: INSET HASH: %s [x%f, y%f] (inset %llu)\n", htm_chord_hex(&inset_hash), x, y, (unsigned long long)inset);

//				csum = crc64(0, data + i, MIN(8, fl_len - i)); 
				memset(&hash, 0, sizeof(hash));
//				memcpy(&hash, (data + i), MIN(32, fl_len - i));
				htm_chord_digest(&hash, &seed, (data + i), MIN(32, fl_len - i));
fprintf(stderr, "DEBUG: DIGEST HASH: %s\n", htm_chord_hex(&hash));
				//csum = *((uint64_t *)(data + i));
				//htm_chord_expand(&hash, csum);

				htm_cell_notify(ent, br, cell, &hash, &inset_hash, NULL);

//				usleep(1000);
			}
//		}

		//htm_brane_free(&br)
	}
#endif



	test_run(ent);

#if 0
	uint64_t csum = htm_chord_csum(data, fl_len);

	memset(&hash, 0, sizeof(hash));
	htm_chord_expand(&hash, csum);
	fprintf(stderr, "DEBUG: HASH: %s\n", htm_chord_hex(&hash));

	memset(&nhash, 0, sizeof(nhash));
	htm_chord_hash_init(&nhash, &hash);
	fprintf(stderr, "DEBUG: NHASH: %s\n", htm_chord_hex(&nhash));

	int ok = htm_chord_match(&nhash, &hash);
fprintf(stderr, "DEBUG: %d = htm_chord_match(<nhash>, <hash>)\n", ok);
#endif


	//htm_state_print();

/*
 * if (!prediction) - blank slate, no firing neurons
 * input -> init mode
 * else if (prediction) - predicted neurons are firing
 * input -> apply mode
 */



//	test_net(ent);

	return (0);
}



