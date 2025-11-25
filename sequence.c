#include "microtools.h"

void frame_sequence_next_frame(FrameSequenceState* out, FrameSequence sequence) {
	if (!out->reversed) {
		if (out->index >= sequence.frame_count - 1) {
			switch (sequence.on_end) {
				case FREEZE:
					return;
					break;
				case REPEAT:
					out->index = 0;
					break;
				case PING_PONG:
					out->reversed = true;
					out->index --;
					break;
			}
		} else {
			out->index ++;
		}
	} else {
		if (out->index <= 0) {
			switch (sequence.on_end) {
				case FREEZE:
					return;
					break;
				case REPEAT:
					out->index =
						sequence.frame_count - 1;
					break;
				case PING_PONG:
					out->reversed = false;
					out->index ++;
					break;
			}
		} else {
			out->index --;
		}
	}

}

float frame_sequence_state_update(FrameSequenceState* out, FrameSequence sequence) {
	if (out->timer->just_reached_max) {
		frame_sequence_next_frame(out, sequence);
		out->timer->value = 0;
		out->timer->max = sequence.frames[out->index].duration / 1000;
		out->prev_value = out->value;
	}
	switch (sequence.frames[out->index].interpolation) {
		case NONE:
			out->value = sequence.frames[out->index].value;
			break;
		case LINEAR:
			if (out->timer->max == 0) {
				out->value = sequence.frames[out->index].value;
				break;
			}
			float diff = sequence.frames[out->index].value 
				- out->prev_value;
			out->value = transit_linear_get(out->prev_value, 
					sequence.frames[out->index].value,
					fabs(diff) / out->timer->max,
					out->timer->value);
			break;
		case TWEEN:
			break;
	}
	return out->value;
}
