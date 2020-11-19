
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

static int htm_tree_block(uint64_t key)
{
	return ( (int) ((uint32_t)key % MAX_CACHE_BLOCK_SIZE) );
}

#if 0
static node_t *htm_tree_index(tree_t *tree, int idx)
{
	if (idx < 0 || idx >= MAX_CACHE_BLOCK_SIZE)
		return (NULL);

	return (tree->node_list[idx]);
}
#endif

void htm_tree_append(tree_t *tree, uint64_t key, cell_t *cell)
{
	unsigned char *raw = (unsigned char *)&key;
	int block_nr = htm_tree_block(key);
	node_t *node;
	
	/* ensure this key is not already set. */
	for (node = tree->node_list[block_nr]; node; node = node->next) {
		if (node->key == key) {
			/* found existing node */
			node->cell = cell;
			return;
		}
	}

	/* create new node */
	node = (node_t *)calloc(1, sizeof(node_t));
	if (!node)
		return;

	node->cell = cell;
	node->key = key;
	node->next = tree->node_list[block_nr];
	tree->node_list[block_nr] = node;
}

#if 0
cell_t *htm_tree_create(brane_t *br, tree_t *tree, uint64_t key)
{
	tree_t *t;
	cell_t *cell;

	t = (tree_t *)calloc(1, sizeof(tree_t));
	if (!t)
		return (NULL);

	if (tree->cell)
		t->cell = htm_cell_derive(br, tree->cell);
	else
		t->cell = htm_cell_init(br);
	t->key = key;
	t->next = tree->child_list;
	tree->child_list = t;

	return (cell);
}
#endif

tree_t *htm_tree_init(void)
{
	tree_t *t;

	t = (tree_t *)calloc(1, sizeof(tree_t));
	if (!t)
		return (NULL);

	return (t);
}

#if 0
tree_t *htm_tree_cache_init(void)
{
	tree_t *ret_tree;
	tree_t *last_t;
	tree_t *t;
	int i;

fprintf(stderr, "DEBUG: htm_tree_cache_init()\n");

	ret_tree = last_t = htm_tree_init();
	for (i = 1; i < MAX_CACHE_BLOCK_SIZE; i++) {
		t = htm_tree_init();
		last_t->next = t;
		last_t = t;
	}

	return (ret_tree);
}
#endif

void htm_tree_cache_set(tree_t *tree, uint64_t key, cell_t *cell)
{
	htm_tree_append(tree, key, cell);
}

cell_t *htm_tree_cache_get(tree_t *tree, uint64_t key)
{
	unsigned char *raw = (unsigned char *)&key;
	int block_nr = htm_tree_block(key);
	node_t *ret_node;
	node_t *node;

	for (node = tree->node_list[block_nr]; node; node = node->next) {
		if (node->key == key)
			return (node->cell);
	}

	return (NULL);
}

