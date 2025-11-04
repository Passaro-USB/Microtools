#include "microtools.h"

EntityChainIter entity_chain_iter(EntityChain chain, size_t item_size) {
	return (EntityChainIter) {
		.curr = chain.first,
		.index = 0,
		.item_size = item_size,
	};
}

void* entity_chain_iter_next(EntityChainIter* iter) {
	if (iter->index >= iter->curr->count) {
		iter->index = 0;
		iter->curr = iter->curr->next;
	} 
	if (iter->curr == NULL) {
		return NULL;
	} else {
		void* item = (uint8_t*)iter->curr->entities 
			+ iter->index * iter->item_size;
		iter->index ++;
		return item;
	}
}

EntityNode* entity_chain_add(EntityChain* chain, void* entities, size_t capacity, bool is_on_heap) {
	EntityNode* node = malloc(sizeof(EntityNode));
	node->next = NULL;
	node->prev = NULL;
	node->entities = entities;
	node->capacity = capacity;
	node->count = 0;
	node->is_on_heap = is_on_heap;
	if (chain->last == NULL) {
		chain->first = node;
		chain->last = node;
	} else {
		chain->last->next = node;
		node->prev = chain->last;
		chain->last = node;
	}
	return node;
}

bool entity_chain_remove(EntityChain* chain, EntityNode* node) {
	EntityNode* current = chain->first;
	while (current != NULL) {
		if (current == node) {
			current->prev->next = current->next;
			current->next->prev = current->prev;
			free(current);
			return true;
		}
		current = current->next;
	}
	return false;
}

void entity_chain_free(EntityChain* chain) {
	EntityNode* current = chain->first;
	while (current != NULL) {
		if (current->is_on_heap) {
			free(current->entities);
		}
		void* next = current->next;
		free(current);
		current = next;
	}
}
