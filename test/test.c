#include <stdio.h>
#include "microtools.h"

int main() {
	uint32_t count = 4;
	Vector2 points[] = {
		{0.5, 0.1},
		{0.4, 0.2},
		{0.6, 0.3},
		{0.6, 0.4},
	};
	EntityChunk chunks[] = {
		{.id = 0},
		{.id = 1},
		{.id = 2},
		{.id = 3},
	};
	collision_update_point_chunks(chunks, points, count);
	printf("unordered: ");
	for (int i=0; i < count; i++) {
		printf("[%i](%i, %i), ", chunks[i].id, chunks[i].chunk.x, chunks[i].chunk.y);
	}
	printf("\n");
	collision_sort_point_chunks(chunks, count);
	printf("ordered: ");
	for (int i=0; i < count; i++) {
		printf("[%i](%i, %i), ", chunks[i].id, chunks[i].chunk.x, chunks[i].chunk.y);
	}
	printf("\n");
	EntityPairIter iter = {0};
	while (entity_pair_iter_next(&iter, chunks, count)) {
		printf("%i (%i, %i), %i (%i, %i)\n",
				chunks[iter.chunk_a].id,
				chunks[iter.chunk_a].chunk.x,
				chunks[iter.chunk_a].chunk.y,
				chunks[iter.chunk_b].id,
				chunks[iter.chunk_b].chunk.x,
				chunks[iter.chunk_b].chunk.y
				);
	}
}

