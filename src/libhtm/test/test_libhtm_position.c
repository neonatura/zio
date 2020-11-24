
#include "test_libhtm.h"

_TEST(position)
{
	brane_t *br;
	chord_t h_loc;
	chord_t h_ori;
	cell_t *cell;
	pos_t *c_pos;
	pos_t pos;
	int idx;

	memset(&pos, 0, sizeof(pos));
	memset(&h_loc, 0, sizeof(h_loc));
	memset(&h_ori, 0, sizeof(h_ori));

	br = htm_brane_get(test_entity, BR_EXEC);
	cell = htm_cell_init(br);

	for (idx = 1000; idx < 1010; idx++) {
		pos.loc_x = (double)((idx) % 4096) / 4096;
		pos.loc_y = (double)((idx + idx) % 4096) / 4096;
		pos.loc_z = (double)((idx + idx + idx) % 4096) / 4096;
		pos.ori_x = (double)((idx + 1) % 4096) / 4096;
		pos.ori_y = (double)((idx + 1 + idx) % 4096) / 4096;
		pos.ori_z = (double)((idx + 1 + idx + idx) % 4096) / 4096;
		htm_position_gen(&pos);

//fprintf(stderr, "DEBUG: loc(%-8.8f, %-8.8f, %-8.8f) = %s\n", pos.loc_x, pos.loc_y, pos.loc_z, htm_chord_hex(&pos.loc_hash));
//fprintf(stderr, "DEBUG: ori(%-8.8f, %-8.8f, %-8.8f) = %s\n", pos.ori_x, pos.ori_y, pos.ori_z, htm_chord_hex(&pos.ori_hash));

		for (int j = 0; j < 4; j++) {
			static chord_t blank_hash;
			htm_cell_motor_apply(test_entity, br, cell, &blank_hash, &blank_hash);
		}

		/* x3 to clear previous */
		htm_cell_motor_apply(test_entity, br, cell, &pos.loc_hash, &pos.ori_hash); 

		c_pos = htm_cell_position(cell);
//fprintf(stderr, "DEBUG: cell loc(%-8.8f, %-8.8f, %-8.8f) = %s\n", c_pos->loc_x, c_pos->loc_y, c_pos->loc_z, htm_chord_hex(&c_pos->loc_hash));
//fprintf(stderr, "DEBUG: cell ori(%-8.8f, %-8.8f, %-8.8f) = %s\n", c_pos->ori_x, c_pos->ori_y, c_pos->ori_z, htm_chord_hex(&c_pos->ori_hash));

		_TRUE(abs( (pos.loc_x * 100) - (c_pos->loc_x * 100) ) < 1.0);
		_TRUE(abs( (pos.loc_y * 100) - (c_pos->loc_y * 100) ) < 1.0);
		_TRUE(abs( (pos.loc_z * 100) - (c_pos->loc_z * 100) ) < 1.0);

		_TRUE(abs( (pos.ori_x * 100) - (c_pos->ori_x * 100) ) < 1.0);
		_TRUE(abs( (pos.ori_y * 100) - (c_pos->ori_y * 100) ) < 1.0);
		_TRUE(abs( (pos.ori_z * 100) - (c_pos->ori_z * 100) ) < 1.0);

		_TRUE(c_pos->loc_x != c_pos->ori_x);
		_TRUE(c_pos->loc_y != c_pos->ori_y);
		_TRUE(c_pos->loc_z != c_pos->ori_z);

	}


}
