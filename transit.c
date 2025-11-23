#include <microtools.h>

float transit_linear_get(float start, float end, float angle, float t) {
	float dir = end > start? 1.0 : -1.0;
	float value = start + angle * dir * t;
	if ((end > start && value > end) || (end < start && value < end)) {
		value = end;
	}
	return value;
}

float transit_linear_get_next_instant(float current, float end, float angle, float delta) {
	float dir = end > current? 1.0 : -1.0;
	float value = current + angle * dir * delta;
	if ((end > current && value > end) || (end < current && value < end)) {
		value = end;
	}
	// stop jittering when value is close to end
	if (fabs(value - end) / delta <= 1.0) {
		value = end;
	}
	return value;
}

float transit_linear_get_duration(float start, float end, float angle) {
	return fabs(end - start / angle);
}

float transit_tween_get(float start, float end, float c, float t) {
	float dir = end > start ? 1.0 : -1.0;
	return end - fmaxf(fabs(end - start) * pow(1 - c, t), 0.0) * dir;
}

float transit_tween_get_next_instant(float current, float end, float c, float delta) {
	float dir = end > current ? 1.0 : -1.0;
	return end - fmaxf(fabs(end - current) * pow(1 - c, delta), 0.0) * dir;
}

float transit_tween_get_duration(float coeficient) {
	return log2(0.0001) / log2(1 - coeficient);
}
