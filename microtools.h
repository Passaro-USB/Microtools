#ifndef MICROTOOLS_H
#define MICROTOOLS_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

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

#endif
