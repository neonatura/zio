
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

const char *htm_brane_dir(void)
{
	static char ret_path[PATH_MAX+1];

#ifdef WINDOWS
	sprintf(ret_path, "%s\\HTM\\", getenv("ProgramData"));
#else
	sprintf(ret_path, "/var/lib/HTM/");
#endif
	mkdir(ret_path, 0777);

	return (ret_path);
}

const char *htm_brane_fname(const char *name)
{
	static char ret_path[PATH_MAX+1];
	int i;

	memset(ret_path, 0, sizeof(ret_path));
	for (i = 0; i < strlen(name); i++)
		sprintf(ret_path+strlen(ret_path), "%-2.2x", name[i]);
	strcat(ret_path, ".bin");

	return (ret_path);
}

brane_t *htm_brane_init(uint64_t ent_id, const char *tag, int size)
{
	struct stat st;
	brane_t *br;
	char path[PATH_MAX+1];
	uint8_t *raw;
	int err;
	int fd;

	size = (size + 16383) / 16384;
	size = (size * 16384);

	memset(path, 0, sizeof(path));
	sprintf(path, "%s%s", 
			htm_brane_dir(), htm_brane_fname(tag));
#ifdef WINDOWS
  fd = open(path, O_RDWR | O_CREAT, 00777);
#else
  fd = open(path, O_RDWR | O_CREAT, 00700);
#endif
	if (fd == -1)
		return (NULL);

	memset(&st, 0, sizeof(st));
	(void)fstat(fd, &st);
	if (st.st_size < size) {
		err = ftruncate(fd, size);
		if (err) {
			close(fd);
			return (NULL);
		}
	}

	br = (brane_t *)calloc(1, sizeof(brane_t));
	if (!br) {
		close(fd);
		return (NULL);
	}

	br->id = crc64(ent_id, tag, strlen(tag)); 

#ifdef WINDOWS
  fsync(map->fd); /* quash cache */
#endif

	raw = (uint8_t *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (raw == MAP_FAILED) {
		free(br);
		return (NULL);
	}

	br->map_fd = fd;
	br->map_size = size;

	br->cell = (cell_t *)raw;
	br->cell_len = 1;
	br->cell_max = (size / 2 / sizeof(cell_t)); 

	/* TODO: .. count actual set nuerons via bit/state.. */

	br->path = (path_t *)(raw + (size / 2));
	br->path_len = 1;
	br->path_max = (size / 2 / sizeof(path_t));

	/* TODO: .. count actual set paths via !0 */

	br->cell_map = hmap_init();

	zpu_init(&br->zpu);

	return (br);
}

void htm_brane_free(brane_t **br_p)
{
	brane_t *br;

	if (!br_p)
		return;

	br = *br_p;
	*br_p = NULL;
	if (!br)
		return;

	/* dealloc persistent cell storage. */
	if (br->map_fd) {
		munmap(br->cell, br->map_size);
		close(br->map_fd);
		br->map_fd = 0;
	}

	/* dealloc cell mapping table. */
	hmap_free(&br->cell_map);

	free(br);
}

chord_t *htm_brane_map_set(brane_t *br, cell_t *cell)
{
	hmap_set(br->cell_map, cell->id, cell);
}
#if 0
chord_t *htm_brane_map_set(brane_t *br, cell_t *cell)
{
	static chord_t hash;

	memset(&hash, 0, sizeof(hash));
	htm_chord_expand(&hash, cell->id);
	hmap_set(br->cell_map, &hash, cell);

	return (&hash);
}
#endif

cell_t *htm_brane_map_get(brane_t *br, chord_t *hash)
{
	return ((cell_t *)hmap_get(br->cell_map, htm_chord_compact(hash)));
}

tree_t *htm_brane_cache(brane_t *br)
{
	if (!br)
		return (NULL);
	return (&br->cell_cache);
}

cell_t *htm_brane_cell(entity_t *ent, int br)
{

	if (br < 0 || br >= MAX_ENTITY_BRANES)
		return (NULL);

	return (ent->brane_cell[BR_MOTOR]);
}

