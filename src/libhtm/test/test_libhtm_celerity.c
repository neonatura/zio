
#include "test_libhtm.h"

_TEST(celerity)
{
	pos_t l_pos;
	pos_t n_pos;
	pos_t pos;
	double y;
	double x;
	unsigned int dir;
	int dir_tot[4];
	int tot;
	int ok;
	int p_tot;
	int k;
	int n_tot;
	int n_cnt, p_cnt;
	int i;

	for (k = 0; k < 10; k++) {
		n_cnt = 0;
		p_cnt = 0;
		n_tot = 0;
		p_tot = 0;
		memset(&l_pos, 0, sizeof(pos_t));
		for (y = 0.1; y <= 0.9; y += 0.1) {
			for (x = 0.1; x <= 0.9; x += 0.1) {
				memset(&pos, 0, sizeof(pos_t));
				pos.loc_x = x;
				pos.loc_y = y;
				htm_position_gen(&pos);

				htm_celerity_motion_commit(test_entity, &l_pos.loc_hash, &pos.loc_hash);

				if (x == 0.9) { /* reset */
					goto next;
				}

				{
					dir = (( (int)(x*10) + (int)(y*10) ) % 4);
					memcpy(&n_pos, &pos, sizeof(pos_t));
					switch (dir) {
						case 0: n_pos.loc_x += 0.1; break;
						case 1: n_pos.loc_y += 0.1; break;
						case 2: n_pos.loc_x -= 0.1; break;
						case 3: n_pos.loc_y -= 0.1; break;
					}
					htm_position_gen(&n_pos);
					ok = htm_celerity_motion_plan(test_entity, &pos.loc_hash, &n_pos.loc_hash);
					if (dir == 0) {
						if (ok) p_cnt++;
						p_tot++;
					} else {
						if (ok) n_cnt++;
						n_tot++;
					}
				}

	next:
				memcpy(&l_pos, &pos, sizeof(pos_t));
			}
		}

		/* 100% of valid "east" sequences were accepted. */
		_TRUE(p_cnt >= (p_tot-1));
		if (k > 2) { /* not training */
			/* more "east" sequences were accepted them remainder. */
			_TRUE( ((double)p_cnt / (double)p_tot) > ((double)n_cnt / (double)n_tot) );
		}
	}

}
