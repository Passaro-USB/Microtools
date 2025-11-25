#include "microtools.h"

uint64_t asset_loader_find_next_tag(FILE* file) {
	char c;
	fread(&c, sizeof(char), 1, file);
	while (c != '@') {
		fread(&c, sizeof(char), 1, file);
	}
	return ftell(file);
}

uint64_t asset_loader_find_tag(FILE* file, char name[16]) {
	char buffer[16] = {0};
	uint32_t count = 0;
	uint64_t pos = -1;
	char c;
	fseek(file, 0, SEEK_SET);
	while (c != EOF) {
		memset(buffer, 0, sizeof(buffer));
		count = 0;
		uint64_t curr = asset_loader_find_next_tag(file);
		fread(&c, sizeof(char), 1, file);
		while (c != '\n') {
			if (isalnum(c) || c == '_') {
				buffer[count++] = c;
			}
			fread(&c, sizeof(char), 1, file);
		}
		if (strcmp(buffer, name) == 0) {
			pos = curr;
			break;
		}
	}
	return pos;
}

void asset_loader_goto(FILE* file, uint64_t pos) {
	fseek(file, pos, SEEK_SET);
}
Asset asset_loader_read_next(FILE* file) {
	Asset asset = {.pos = -1, .name = {0}};
	uint32_t count = 0;
	char c;
	fread(&c, sizeof(char), 1, file);
	while (c != EOF && c != '*') {
		fread(&c, sizeof(char), 1, file);
	}
	asset.pos = ftell(file);
	while (c != EOF && c != '\n') {
		fread(&c, sizeof(char), 1, file);
		if (isalnum(c) || c == '_') {
			asset.name[count++] = c;
		}
	}
	return asset;
}

bool asset_read_numeric_field(FILE* file, Asset asset, char name[16],
			int32_t* out, uint32_t capacity) {
	char buffer[16] = {0};
	uint32_t buffer_len = 0;
	uint32_t number_lenght = 0;
	uint32_t number_count = 0;
	bool is_number_negative = false;
	uint64_t pos = -1;
	char c;
	fseek(file, asset.pos, SEEK_SET);
	fread(&c, sizeof(char), 1, file);
	while (c != EOF) {
		memset(out, 0, sizeof(int32_t) * capacity);
		memset(buffer, 0, sizeof(buffer));
		buffer_len = 0;
		number_lenght = 0;
		number_count = 0;
		is_number_negative = false;
		while (c != '.') {
			fread(&c, sizeof(char), 1, file);
		}
		pos = ftell(file);
		while (!(isalnum(c) || c == '_')) {
			fread(&c, sizeof(char), 1, file);
		}
		while (isalnum(c) || c == '_') {
			buffer[buffer_len++] = c;
			fread(&c, sizeof(char), 1, file);
		}
		if (strcmp(buffer, name) != 0) {
			continue;
		}
		while (c != '\n') {
			fread(&c, sizeof(char), 1, file);
			if (c == '\n' || c == ' ') {
				continue;
			}
			if (c == ',') {
				if (is_number_negative) {
					out[number_count] *= -1;
				}
				if (number_count >= capacity) {
					return true;
				}
				number_lenght = 0;
				number_count ++;
				is_number_negative = false;
				continue;
			}
			if (c == '-') {
				is_number_negative = true;
				continue;
			}
			out[number_count] *= 10;
			out[number_count] += c - '0';
			number_lenght ++;
		}
		if (is_number_negative) {
			out[number_count] *= -1;
		}
		return true;
	}
	return false;
}

bool asset_read_text_field(FILE* file, Asset asset, char name[16],
			char* out, uint32_t capacity) {
	char buffer[16] = {0};
	uint32_t buffer_len = 0;
	uint64_t pos = -1;
	uint32_t text_len = 0;
	char c;
	fseek(file, asset.pos, SEEK_SET);
	fread(&c, sizeof(char), 1, file);
	while (c != EOF) {
		memset(out, 0, sizeof(int32_t) * capacity);
		memset(buffer, 0, sizeof(buffer));
		buffer_len = 0;
		text_len = 0;
		while (c != '.') {
			fread(&c, sizeof(char), 1, file);
		}
		pos = ftell(file);
		while (!(isalnum(c) || c == '_')) {
			fread(&c, sizeof(char), 1, file);
		}
		while (isalnum(c) || c == '_') {
			buffer[buffer_len++] = c;
			fread(&c, sizeof(char), 1, file);
		}
		if (strcmp(buffer, name) != 0) {
			continue;
		}
		while (c != '\"') {
			fread(&c, sizeof(char), 1, file);
		}
		fread(&c, sizeof(char), 1, file);
		while (c != '\"') {
			if (c == '\"') {
				continue;
			}
			if (text_len >= capacity) {
				return true;
			}
			out[text_len] = c;
			text_len++;
			fread(&c, sizeof(char), 1, file);
		}
		return true;
	}
	return false;
}


bool asset_read_frame_sequence_field(FILE* file, Asset asset, char name[16],
			FrameSequence* out, uint32_t capacity) {
	char buffer[16] = {0};
	uint32_t buffer_len = 0;
	uint64_t pos = -1;
	uint32_t index = 0;
	char c;
	fseek(file, asset.pos, SEEK_SET);
	fread(&c, sizeof(char), 1, file);
	while (c != EOF) {
		memset(out->frames, 0, sizeof(Frame) * capacity);
		memset(buffer, 0, sizeof(buffer));
		buffer_len = 0;
		index = 0;
		while (c != '.') {
			fread(&c, sizeof(char), 1, file);
		}
		pos = ftell(file);
		while (!(isalnum(c) || c == '_')) {
			fread(&c, sizeof(char), 1, file);
		}
		while (isalnum(c) || c == '_') {
			buffer[buffer_len++] = c;
			fread(&c, sizeof(char), 1, file);
		}
		if (strcmp(buffer, name) != 0) {
			continue;
		}
		while (c != '!') {
			fread(&c, sizeof(char), 1, file);
			if (c == '!') {
				continue;
			}
			if (c == '\n' || c == ' ') {
				continue;
			}
			if (c == ',') {
				index ++;
				continue;
			}
			if (c == '[') {
				continue;
			}
			if (c == ']') {
				index = 0;
				out->frame_count ++;
				if (out->frame_count >= capacity) {
					return true;
				}
				continue;
			}
			if (index == 0) {
				out->frames[out->frame_count].value *= 10;
				out->frames[out->frame_count].value += c - '0';
			} else if (index == 1) {
				out->frames[out->frame_count].duration *= 10;
				out->frames[out->frame_count].duration += c - '0';
			} else if (index == 2) {
				out->frames[out->frame_count].interpolation *= 10;
				out->frames[out->frame_count].interpolation += c - '0';
			} 
		}
		return true;
	}
	return false;
}
