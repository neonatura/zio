

#ifndef __HTM_STRUCT_H__ 
#define __HTM_STRUCT_H__ 

#define MAX_BRANE_NAME_LENGTH 64

#define MAX_ENTITY_NAME_LENGTH 64

#define HTM_NONE 0
#define HTM_FEATURE 1
#define HTM_OBJECT 2 
#define HTM_EXPERIENCE 3

/**
 * Provides a thalamic relay connection to the neo-cortex.
 */
#define BR_NEO 0

/* brainstem */
#define BR_CONTROL 1

/* Epidermis:
 *
 * Senses: pressure / temperature (hot/cold) / pain / texture (smooth/rough) 
 *
 * Mechanoreceptors: These receptors perceive sensations such as pressure, vibrations, and texture.
 * Thermoreceptors: Two receptors (hot + cold) determine temperature.
 * Pain receptors: Receptors that detect sensations that indicate damage.
 * Proprioceptors: Receptors that sense the position of the different parts of the body in relation to each other and the surrounding environment. 
 */
#define BR_SENSORY 2

/* Occular:
 *   90 degrees * 60 arc-minutes/degree * 1/0.3 * 90 * 60 * 1/0.3 = 324,000,000 pixels (324 megapixels). 
 *   eyes will "follow" movement of [control brane] focused object.
 */
#define BR_VISION 3

#define BR_AUDIO 4

#define BR_SMELL 5

/**
 * Simulated impulses.
 */
#define BR_VIRTUAL 6

#define BR_MOTOR 7

#define BR_EMOTION 8
/**
 * Frontal Lobe:
 *
 * Functions:
 * - Drives motor planning
 * - Problem-solving
 * - Judgement
 * - Planning
 * - Deduces context between objects
 * - Language
 *
 * Communicates:
 * - Behaviour and emotions
 * - Olfaction (smell)
 * - Frontal vision area
 */
#define BR_EXEC 9 

#define MAX_ENTITY_BRANES 10


/**
 * L1 High-order inference (Pooling)
 * - L1 Independant "Experience" Pooling
 * - L1: Inputs "Experience" from memory
 * Molecular layer responsible for modulating excitability (dampening) for the cell. Triggerd by spikes in L2/3 and L5.
 * Connects L/R hemispheres through thalumus.
 * "inhibitory connections may serve to generate and/or terminate particular network states."
 * "The dense inhibition in layer 2/3 could play a role in this modulation of activity, helping computationally sparsify sensory information during transmission from layer 4 to layer 5."
 * "The function of this inhibition may be to guarantee that excitation is appropriately balanced by inhibition not only to prevent runaway excitability but also to produce specific computational goals such as dynamic gain control, temporal filtering, and gating of information."
 */
#define CELL_L1 1
/**
 * L2 High-order inference (Sequential)
 * - L2: Inputs "Feature" from external L3's
 * External granule cell layer.
 */
#define CELL_L2 2
/**
 * L3 High-order inference (Pooling)
 * - L3 Independant "Feature" Pooling
 * - L3 Outputs "Feature @ Location" to L5b
 * External pyramidal cell layer.
 */
#define CELL_L3 3
/**
 * L4 Sequence Memory  (Sensory-motor inference)
 * - L4: receives "Sensation @ Orientation" (Sequential memory)
 * - L4: updates L6a with "Orientation"
 */
#define CELL_L4 4
/**
 * L5 Sequence Memory (Motor Sequences)
 * - L5a: independant "Object" determined from L5b (Pooling)
 */
#define CELL_L5a 5
/**
 * L5 Sequence Memory (Motor Sequences)
 * - L5b: receives "Feature @ Location" from L3 (Sequential memory)
 * - L5b: determines and updates "Location" from/to L6b
 */
#define CELL_L5b 6
/**
 * L6 Sequence Memory (Attension/Modulation/Feedback)
 * - L6a: "Orientation" Motor updated [from/to L4] (like xy-axis cell)
 */
#define CELL_L6a 7
/**
 * L6 Sequence Memory (Attension/Modulation/Feedback)
 * - L6b: Receives "Location" updates from L5b.
 */
#define CELL_L6b 8
#define MAX_CELL_LAYER 9

#define INPUT_CELL_LAYER CELL_L4
#define FEATURE_CELL_LAYER CELL_L3
#define OBJECT_CELL_LAYER CELL_L5a

#define CHORD_SIZE 64

#define CACHE_SIZE 64

#define OP_WAKE 0
#define OP_RESET 1
#define OP_SEQ 2
#define OP_POOL 3
#define OP_FEEDBACK 4

#define MAX_CHORD_SPLIT_SIZE (double)UINT64_MAX
#define MID_CHORD_SPLIT_SIZE (double)UINT32_MAX
#define MIN_CHORD_SPLIT_SIZE (double)4096

/* threshold potentional (activation) */
#define HTM_ACTION_THRESHOLD -55
#define HTM_ACTION_POLORIZED -60
#define HTM_ACTION_REST -70
#define HTM_ACTION_INHIBIT -75

#define MAX_CACHE_BLOCK_SIZE 1024

#define MAX_ZPU_STACK 256

typedef uint64_t htime;

typedef struct chord_t
{
	uint8_t vec[CHORD_SIZE];

	/* .. move weight & stamp into layer_t, 
	 * create single optional 'output link', with bias, to forward-feed cell
	 */
//	uint8_t weight[CHORD_SIZE];
//	uint8_t bias[CHORD_SIZE];
//	htime stamp;
} chord_t;

typedef uint64_t hkey_t;

typedef struct hmap_entry_t
{
	hkey_t key;
	unsigned int hash;
	void *val;
	struct hmap_entry_t *next;
} hmap_entry_t;

struct hmap_t;
typedef struct hmap_index_t
{
	struct hmap_t *ht;
	hmap_entry_t *tthis, *next;
	unsigned int index;
} hmap_index_t;

typedef struct hmap_t
{
	hmap_entry_t **array;
	hmap_index_t iterator;  /* For hmap_first(...) */
	unsigned int count, max;
	hmap_entry_t *free;  /* List of recycled entries */
} hmap_t;

typedef uint64_t cachekey_t;

typedef struct cache_t
{
	uint64_t key; 
	chord_t seq;
	/* todo: back to -> uint64_t */
} cache_t;

struct layer_t;

typedef void (*htm_op_f)(struct layer_t *, chord_t *, int);

typedef struct layer_t
{
	chord_t hash;
	int8_t level[CHORD_SIZE];

	/* fills by raw input and hippocampus induced cache replay. first record is most important, and list is quasi-sorted based on occurence rate.*/

	htm_op_f op;
} layer_t;

typedef struct path_t
{
	double weight;
	uint32_t path_idx;
	uint32_t cell_idx;
	uint32_t next_idx;
	uint32_t __reserved_0__;
#if 0
	cell_t *cell;
	chord_t h_cell;
	struct path_t *next;
#endif
} path_t;

typedef struct cell_t
{
	layer_t layer[MAX_CELL_LAYER];
	uint64_t id;
	uint64_t bias;
	uint32_t brane_type;
	uint32_t cell_idx;
	uint32_t path_idx;

	/* grid-cell offset (displacement) of stimuli origin in relation to "body grid-cell" orientation -- updating dynamically in response to received motor loc/ori data (L6). */
	float loc_x, loc_y, loc_z;
	float ori_x, ori_y, ori_z;

//	uint64_t id_output; /* axon cell id on same brane */
	cache_t cache[CACHE_SIZE];
} cell_t;

typedef struct node_t {
	cell_t *cell;
	uint64_t key;
	struct node_t *next;
} node_t;

typedef struct tree_t {
	node_t *node_list[MAX_CACHE_BLOCK_SIZE]; 
//	struct tree_t *child_list;
} tree_t;

struct brane_def_t;
struct entity_t;

typedef struct brane_t
{

	uint32_t cell_len;
	uint32_t cell_max;

	uint32_t path_len;
	uint32_t path_max;

	uint32_t type;
	uint32_t __reserved_0__;

	/* mmap of cells & paths */
	uint32_t map_fd;
	uint32_t map_size;

	uint64_t id;

	/* mapping table for cell ids */
	hmap_t *cell_map;

	/* cell (neuron columns) */
	cell_t *cell;
	path_t *path;

	char name[MAX_BRANE_NAME_LENGTH];

	/* brane defintion */
	struct brane_def_t *def;

	/* entity */
	struct entity_t *ent;

	/* hierarchy hashmap of cells. */
	tree_t cell_cache;
} brane_t;

typedef struct event_t
{
	chord_t h_feat;
	chord_t h_obj;
	chord_t h_exp;
	uint64_t cell_id;
	htime stamp;
	double resonance;
} event_t;

typedef qvar zvar;

typedef struct zinst_t {
	qvar param;
	uint8_t code;
} zinst_t;

typedef struct zpu_t {
	zinst_t inst[MAX_ZPU_STACK];
	uint32_t top;
} zpu_t;

typedef struct entity_t 
{

	/* identity */
	uint64_t id;
	char name[MAX_ENTITY_NAME_LENGTH];

	/* current attention session */
	uint64_t sess_id;
	htime sess_stamp;
	double sess_rate;
	double sess_freq; /* -1.0 - 1.0 ~ 20ms */

	hmap_t *path_map;

	brane_t *brane[MAX_ENTITY_BRANES];

	/* a "primary" cell for brane. */
	cell_t *brane_cell[MAX_ENTITY_BRANES];

	zpu_t zpu;

	struct {
		chord_t state_hash;
		htime stamp;
		htime state_stamp;
		htime birth_stamp;
		double rate;
		double pref_rate;
		double wave;
		uint32_t state;
		uint32_t __reserved0__;
		/* default posture stance */
		double x_axis, y_axis, z_axis;
		/* degree of focus on a particular set of features (0.0 <-> 1.0)
		 * Lower saliency control will utilize 'most common' alertness feature maps in order to concentrate on general cues such as faces, orientation, color, and intensity. ("bottom up attention")
		 * Higher saliency control will concentrate on a feed-forward driven set of features. ("top down attention")
		 */
		double saliency;
	} control;

} entity_t;

typedef void (*brane_f)(struct entity_t *ent, struct brane_t *);
typedef void (*brane_cell_f)(struct entity_t *ent, struct brane_t *, struct cell_t *);
typedef void *(*brane_proc_f)(struct entity_t *ent);
//#define BRANE_PROC(_a) (void *(*)(void *))_a

typedef struct brane_def_t
{
	int type;
	const char *label;
	size_t size;
	brane_f init;
	brane_proc_f run;
	brane_f term;
	brane_f wake;
	brane_cell_f read;
	brane_cell_f write;
} brane_def_t;

typedef struct pos_t {
	chord_t loc_hash;
	chord_t ori_hash;
	double loc_x, loc_y, loc_z;
	double ori_x, ori_y, ori_z;
} pos_t;

typedef struct tensor_t {
	/* the maximum x-y cordinate */
	uint32_t dim;
	/* the relative distance between cells */ 
	double step;
	/* x-y mapping */
	chord_t **grid;
} tensor_t;

typedef struct grid_t {
	/* the object this grid resides on. */
	chord_t parent;
	/* the position of the grid relative to it's parent. */
	pos_t pos;
	/* the scale of the grid in comparison to it's parent. */
	double scale;
	/* a tensor mapping objects to relative locations inside grid */
	uint32_t tensor_max;
	tensor_t *tensor;
} grid_t;

#endif /* ndef __HTM_STRUCT_H__ */ 

