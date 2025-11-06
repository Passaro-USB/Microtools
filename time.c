#include "microtools.h"

bool time_update(Time* time, float delta) {
	time->global_timer += delta;
	EntityChainIter iter = entity_chain_iter(time->timers, sizeof(Timer));
	Timer* timer = entity_chain_iter_next(&iter);
	while (timer != NULL) {
		timer->just_reached_max = false;
		switch (timer->type) {
			case COUNT_UP:
				timer->value += delta;
				if (timer->value >= timer->max) {
					timer->just_reached_max = true;
				}
				timer->value = fmin(timer->value, timer->max);
				break;
			case COUNT_DOWN:
				timer->value -= delta;
				if (timer->value <= 0.0) {
					timer->just_reached_max = true;
				}
				timer->value = fmax(timer->value, 0);
				break;
			case CYCLE_UP:
				timer->value += delta;
				if (timer->value >= timer->max) {
					timer->just_reached_max = true;
				}
				timer->value = fmod(timer->value, timer->max);
				break;
			case CYCLE_DOWN:
				timer->value -= delta;
				if (timer->value <= 0.0) {
					timer->just_reached_max = true;
				}
				timer->value = fmod(
						timer->value + timer->max,
						timer->max);
				break;
		}
		timer = entity_chain_iter_next(&iter);
	}
}
