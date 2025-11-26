#ifndef MICROTOOLS_H
#define MICROTOOLS_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <strings.h>
#include <ctype.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Structures for representing points
typedef struct Vector2 {
	float x;
	float y;
} Vector2;

typedef struct Vector2i {
	int32_t x;
	int32_t y;
} Vector2i;

typedef struct Vector3 {
	float x;
	float y;
	float z;
} Vector3;

// Functions for alinging entities in such a way to allow for fast removal or
// addition of big groups of entities as well as iterating over them.
typedef struct EntityNode {
	void* entities;
	size_t capacity;
	size_t count;
	bool is_on_heap;
	struct EntityNode* next;
	struct EntityNode* prev;
} EntityNode;

typedef struct EntityChain {
	EntityNode* first;
	EntityNode* last;
} EntityChain;

typedef struct EntityChainIter {
	EntityNode* curr;
	size_t index;
	size_t item_size;
} EntityChainIter;

EntityChainIter entity_chain_iter(EntityChain chain, size_t item_size);
void* entity_chain_iter_next(EntityChainIter* iter);
EntityNode* entity_chain_add(EntityChain* chain, void* entities,
		size_t capacity, bool is_on_heap);
bool entity_chain_remove(EntityChain* chain, EntityNode* node);
void entity_chain_free(EntityChain* chain);




// Functions for creating synchronized timers
typedef struct Time {
	EntityChain timers;
	float global_timer;
} Time;

typedef enum TimerType {
	COUNT_UP,
	COUNT_DOWN,
	CYCLE_UP,
	CYCLE_DOWN,
} TimerType;

typedef struct Timer {
	float value;
	float max;
	TimerType type;
	bool just_reached_max;
} Timer;

bool time_update(Time* time, float delta);



// Functions for handling timed sequences (animation)
typedef enum FrameInterpolation {
	NONE,
	LINEAR,
	TWEEN,
} FrameInterpolation;

typedef struct Frame {
	float duration;
	float value;
	FrameInterpolation interpolation;
} Frame;

typedef enum SequenceEndBehavior {
	FREEZE,
	REPEAT,
	PING_PONG,
} SequenceEndBehavior;

typedef struct FrameSequence {
	Frame* frames;
	uint32_t frame_count;
	SequenceEndBehavior on_end;
} FrameSequence;

typedef struct FrameSequenceState {
	Timer* timer;
	uint32_t index;
	float speed;
	bool reversed;
	float value;
	float prev_value;
} FrameSequenceState;

void frame_sequence_next_frame(FrameSequenceState* out, FrameSequence sequence);
float frame_sequence_state_update(FrameSequenceState* out, FrameSequence sequence);



// Functions for asset loading
typedef struct Asset {
	char name[16];
	uint64_t pos;
} Asset;

uint64_t asset_loader_find_next_tag(FILE* file);
uint64_t asset_loader_find_tag(FILE* file, char name[16]);
void asset_loader_goto(FILE* file, uint64_t pos);
Asset asset_loader_read_next(FILE* file);
bool asset_read_text_field(FILE* file, Asset asset, char name[16],
			char* out, uint32_t capacity);
bool asset_read_numeric_field(FILE* file, Asset asset, char name[16],
			int32_t* out, uint32_t capacity);
bool asset_read_frame_sequence_field(FILE* file, Asset asset, char name[16],
			FrameSequence* out, uint32_t capacity);



//  Functions for handling some opengl boilerplate
typedef struct Shape {
	uint32_t vao;
	uint32_t vbo;
	uint32_t ebo;
	uint32_t vertex_count;
	uint32_t index_count;
} Shape;

typedef struct Shader {
	uint32_t shader_id;
	GLenum shader_type;
} Shader;

uint32_t glutils_load_texture(char* path, GLenum format, GLenum type);
Shape glutils_setup_shape(float* vertices, uint32_t vertex_count,
		uint32_t* indices, uint32_t index_count);
Shader glutils_load_and_compile_shader(char* path, GLenum shader_type);
uint32_t glutils_link_shader_program(Shader* shaders, uint32_t count);
void glutils_config_window(GLFWwindow* window);



// Functions for in-betweening (transiting) values
float transit_linear_get(float start, float end, float angle, float t);
float transit_linear_get_next_instant(float current, float end, float angle, float delta);
float transit_linear_get_coefficient(float start, float end, float t);
float transit_linear_get_duration(float start, float end, float angle);
float transit_tween_get(float current, float end, float c, float t);
float transit_tween_get_next_instant(float current, float end, float c, float delta);
float transit_tween_get_coefficient(float t);
float transit_tween_get_duration(float coeficient);



// Functions for identifing and resolving collision
typedef struct EntityChunk {
	Vector2i chunk;
	uint32_t id;
} EntityChunk;

typedef struct EntityPairIter {
	uint32_t chunk_a;
	uint32_t chunk_b;
} EntityPairIter;

EntityChunk collision_get_point_chunk(uint32_t id, Vector2 point);
void collision_update_point_chunks(EntityChunk* chunks, Vector2* points, uint32_t count);
void collision_sort_point_chunks(EntityChunk* chunks, uint32_t count);
bool collision_are_broadly_colliding(EntityChunk* chunks, uint32_t a, uint32_t b);
bool entity_pair_iter_next(EntityPairIter* iter, EntityChunk* chunks, uint32_t count);
bool collision_aabb_check(Vector2 a_pos, Vector2 a_size, Vector2 b_pos, Vector2 b_size);

#endif
