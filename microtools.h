#ifndef MICROTOOLS_H
#define MICROTOOLS_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <strings.h>
#include <ctype.h>

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

// Functions for asset loading
typedef struct Asset {
	char name[16];
	uint64_t pos;
} Asset;

uint64_t asset_loader_find_next_tag(FILE* file);
uint64_t asset_loader_find_tag(FILE* file, char name[16]);
void asset_loader_goto(FILE* file, uint64_t pos);
Asset asset_loader_read_next(FILE* file);
bool asset_read_field(FILE* file, Asset asset, char name[16],
			int32_t* out, uint32_t count);

#endif
