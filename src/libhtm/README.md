

Purpose: Location Module
Location is updated by movement




mmap f/ entity-id+brane-id

read bmp file. scan over grid cells find edges.

pool "linked" cells for Feature, Object, and Experience

htm_mem:
  'hash' of experience is also key? bundles events?
  method of sequencing together. "cell id" is good key?

recording memory. variable degree weighted how?

"sequence of memory" .. cell references hash which has event providing access to next event?

brain "oscilation" (a ping to input receptors?) slows to 1/s during initial sleep

timeframe keyed hash of "long term memories" stored on neo-cortex. references to time-frame encompass 'begin of data segment' such as a song.

'idle state'
memory of recent experience is replayed in reverse-time order

'run state'
actively computing input data and executing timed events

'sleep state'
cached events are reprocessed for purge or permanent storage

'elliptic mode'
only executing real-time events




--






/*
 * Each set of sensor stimuli results in a 'hash' of stimulated neurons. Feature + location of object model = input. Each "feature+location" determines its own "guess" of object models with a calculated output hash. Geodetic location is modular of object model's default perspective point. Close pre-recorded hash matches are also stimulated. The most common hash results in the 'best' guess.   
 *
 * Object models are hierarchial. "Objects are always composed of other objects."
 *
 * Location representations are dimensionless. Dimensionality is defined by movement.
 * Movements do not have to be physical. The only have to exhibit path integration.
 * Features do not have to be sensory features. They can be outputs of other columns/neurons.
 *
 * These neurons repsent the particular input stimuli.
 * The hippocampus temporarily records the mapping between the has and the neurons stimulated. Invoking the hash also stimulates the same neurons.
 *
 * Neurons act as capacitors 'storing' and building stimulus until an activation occurs. Like a capacitor, the activation is based on voltage and not current. 
 *
 * Neurons have different attributes such as polarization.
 *
 * Creates predictive models for objects at orientations. The most stimulated hash (collection of neurons) is considered 'best'. Stores "set set of features @ locations for object models". 
 *
 * Resonance (past stimuli) determines resistance.
 *
 * Neurons are 'initialized onto a plane' but can encounter plasticity. 
 *
 * Neuron can be depalorized without activation.
 *
 * "htm" Hierchial Temporal Memory
 *
 * Neocortex Cellular Layers:
 * L1 inner-cortex communication
 * L2/3 High-order inference (Pooling)
 * - L3 "Feature" Pooling
 * - L3 Outputs "Feature @ Location" to L5b
 * - L3: cell output #1; representing feature [independant of object]
 * L4 Sequence Memory  (Sensory-motor inference)
 * - L4 receives "Sensation @ Orientation" (Sequential memory)
 * - L4 bi-comm with L6a, can update L6a's "Orientation"
 * L5 Sequence Memory (Motor Sequences)
 * - L5a: "Object" determined from L5b (Pooling)
 * - L5a: cell output #2; representing object [independant of feature] 
 * - L5b: receives "Feature @ Location" from L3 (Sequential memory)
 * - L5b: determines and updates "Location" from/to L6b
 * L6 Sequence Memory (Attension/Modulation/Feedback)
 * - L6a: "Orientation" Motor updated (like xy-axis cell)
 * - L6b: "Location" Motor updated (like grid cell)
 * - L6b: receives "Location" updates from L5b
 *
 * Note: "Orientation" of head to room
 *
 * Order of operations:
 * L4 receives external Sensation @ Orientation
 * L4 updates L6a with Orientation
 * L6a updates L4 with Orientation correction
 * L4 pools L3 Feature with Sensation @ Orientation
 * L3 updates L5b with Feature @ Location
 * L5b updates L6b with Location
 * L6b updates L5b with Location correction
 * L5b pools L5a Object with Feature @ Location
 * L3 notifies external cells' Feature set
 * L5a notifies external cells' Object set
 *
 * Combined cell function:
 * Every cell learns complete models of objects. They operate in parallel.
 * Inputs project to multiple levels at once. Columns operate at different scales of input. 
 * For example, multiple inputs from a touch sensor array may use a seperate cell to activate in order to indicate the scale of the "touched" area, and
 * a cell may represent input from sections of two different sensor areas.
 * Non-hierchial connections (combined cell) allow columns to vote on shared elements such as "Object" and "Feature".
 * Cells need motor-derived representations of location and orientation, of the sensor relative to the object. These are analogous to grid and head direction cells.
 * Cells learn complete models of objects as "features at locations".
 *
 *
 * Wiring:
 * L2a -> cortex
 * L2/3 -> L5
 * thalamus -> L4
 * L4 -> L2/3
 * L4 <-> L6a
 * L5 <-> L6b
 * L6 -> thalamus modulation
 * L5tt -> thalamus (relay to cortex) + motor
 *
 * Notes:
 * L4 learns sensory-motor sequences received from both sensor/afferent data and a direct copy of motor commands.
 * L4 is predicted sequences and L2/3 is stable projection. When no predicted sequences exist (matching bits in known hashes), it passes it up to L2/3 for memorization. L4 prediction is directly performed from raw sensory input.
 *
 * L4 is sequential memory (changes with each movement) that can be thought of encapsulating a feature @ a location,
 * where L2/3 is a "Pooling" layer (stable over movement of sensor) which can be thought of encapsulating an entire object.
 * Note: Each 'column' picks an object and the highest rate of single object is 'best guess'. Both L2/3 and L4 have their own 'cache' of hash sequences. 
 *
 * HTM neuron:
 * aux fire pin
 * output pin
 * feedback modulation based upon cached matrix matching
 * local cached matrix for predictive matching - threshold coincidence detectors
 *
 * local cache match activates neuron if in predictive state.
 *
 * Forward feedback from other neurons activate aux fire pin. 
 *
 * In order to compare predictive behaviour, it adds together (mem OR operation) the bits from ~ 100 remembered hashes and compares against the input (i.e. if input has all bits selected combined hash).
 * 
 * A neuron, when activated, will seek out (axon) other nearby neurons (and connect to it's dendrite) which have been recently activated. The neuron is then activated ("predictively") when the same set of nearby neurons are also activated.
 *
 * High-order Sequences
 * (before training)
 * A0-A6, C0-C6, .. are marked for A-B-C-D
 * D0-D6, C0-C6, .. are marked for X-B-C-Y
 * (after training)
 * each sequence clears all bit one of the six bits (in this example) in it's "column" based on prior sequence matrix/obj-model hash.
 * uint8_t cols[8];
 * if (cols[l_csum%8] == 0) (learn)
 *   cols[l_csum%8] = 0xFF; (enable all bits)
 * else
 *   cols[l_csum%8] = (1 << ((l_hash+l_csum) % 8));
 * 
 * NEURON_STATE_PREDICT
 * NEURON_STATE_IDLE
 * NEURON_STATE_FIRE
 * NEURON_STATE_FATIGUE
 * NEURON_STATE_SUPPRESS
 * <time> fire_expire (duty cycle)
 * <time> sleep_expire (fatigue/suppress/pulse)
 * <time> freq_ms (duration of fire/timer)
 * <double> voltage
 * <double> current
 *
 * Image Processing:
 * chunk into grid cell and compare contrast of center of each cell with its surrounding area in order to determine edges.
 */




core --
		 \   motor
		 		 concious state (percent)	
				 temperature

motor
	involuntary
		flinch
		triggered by hearing reflexes
	voluntary
		cerebellum guided planned motor sequence

supplementary motor cortex:
	attainment of new motor sequences
	execution of sequences of movements
	selection of movement based on perephial sensory data
	bimanual motor tasks
	motor imagery
	activated during memory guided tasks
	soley resonsible for mental rehearsal
	pre-sma
		prefrontal cortex
		may be specificly for initiating actions based on self-generated time estimates
			1) structure and initiate future motor actions based on info on time duration
			2) different neurons code for different time intervals
		cordination of;
			* Temporal processing
			* Motor timing
			* Time perception
	sma proper
		primary mortex cortex
		spinal chord

premotor cortex
	plans execution of sequence of motor commands
	sensor-motor associations
	observation of actions (i.e. mirror nuerons)
	selection of motor action
	nuerons experience directional coding (diff ones wire for moving in diff direction)
	
motor flow:
	task selection ->
	

cortical (action) -> basal ganglia (modular, chaining together complex movements) -> brainstem (complex movement) -> central pattern generator (movement) -> cerebellum (modular, organize movements to be compatible with each other, predictionary, self-learning) -> motor interneurons (simple movements) -> motoneurons (flexors, muscle contraction)

differnt types of movement:
	passive (floppy tone, cogwheel rigidity)
	reflexive 
	stereotyped (chewing, walking, standing, encoded through central pattern generator)
	self-generated
		volitional, deliberate (fresh)
    emotional reaction (like laughing at joke)


