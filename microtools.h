#ifndef MICROTOOLS_H
#define MICROTOOLS_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

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
EntityNode* entity_chain_add(EntityChain* chain, void* entities, size_t capacity, bool is_on_heap);
bool entity_chain_remove(EntityChain* chain, EntityNode* node);
void entity_chain_free(EntityChain* chain);

#endif
