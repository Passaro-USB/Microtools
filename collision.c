#include "microtools.h"

#define CHUNK_SIZE (1.0 / 5.0)

EntityChunk collision_get_point_chunk(uint32_t id, Vector2 point) {
	return (EntityChunk) { 
		.chunk = {
			.x = floor(point.x / CHUNK_SIZE - 0.5),
			.y = floor(point.y / CHUNK_SIZE - 0.5),
		},
		.id = id,
	};
}

void collision_update_point_chunks(EntityChunk* chunks, Vector2* points, uint32_t count) {
	for (uint32_t i=0; i < count; i ++) {
		chunks[i] = collision_get_point_chunk(
				chunks[i].id, points[chunks[i].id]);
	}
}

void collision_sort_point_chunks(EntityChunk* chunks, uint32_t count) {
	bool sorted = false;
	while (!sorted) {
		sorted = true;
		for (uint32_t i = 0; i < count - 1; i++) {
			int32_t ix = chunks[i].chunk.x;
			int32_t jx = chunks[i + 1].chunk.x;
			if (ix > jx) {
				sorted = false;
				EntityChunk tmp = chunks[i];
				chunks[i] = chunks[i + 1];
				chunks[i + 1] = tmp;
				continue;
			}
			int32_t iy = chunks[i].chunk.y;
			int32_t jy = chunks[i + 1].chunk.y;
			if (ix == jx && iy > jy) {
				sorted = false;
				EntityChunk tmp = chunks[i];
				chunks[i] = chunks[i + 1];
				chunks[i + 1] = tmp;
			}
		}
	}
}

bool collision_are_broadly_colliding(EntityChunk* chunks, uint32_t a, uint32_t b) {
	return a != b &&
		fabs(chunks[a].chunk.x - chunks[b].chunk.x) <= 1.0 &&
		fabs(chunks[a].chunk.y - chunks[b].chunk.y) <= 1.0;
}

bool entity_pair_iter_next(EntityPairIter* iter, EntityChunk* chunks, uint32_t count) {
	if (count <= 1) {
		return false;
	}
	iter->chunk_b ++;
	while (!collision_are_broadly_colliding(chunks, iter->chunk_a, iter->chunk_b)) {
		if (iter->chunk_b >= count) {
			iter->chunk_a ++;
			iter->chunk_b = iter->chunk_a;
		}
		if (iter->chunk_a >= count) {
			return false;
		}
		iter->chunk_b ++;
	}
	return true;
}
