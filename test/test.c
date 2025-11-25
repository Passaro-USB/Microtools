#include <stdio.h>
#include "microtools.h"

int main() {
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 600, "Wah", 0, 0);
	glfwMakeContextCurrent(window);
	glewInit();
	glutils_config_window(window);

	Timer animation_timer_x = {0};
	Timer animation_timer_y = {0};
	FILE* config_file = fopen("test.config", "r");
	Asset player = asset_loader_read_next(config_file);
	Frame xframes[4] = {0};
	Frame yframes[4] = {0};
	FrameSequence my_sequence_x = {.frames = xframes};
	FrameSequence my_sequence_y = {.frames = yframes};
	my_sequence_x.on_end = REPEAT;
	my_sequence_y.on_end = REPEAT;
	asset_read_frame_sequence_field(config_file, player,
				"xanimation", &my_sequence_x, 4);
	asset_read_frame_sequence_field(config_file, player,
				"yanimation", &my_sequence_y, 4);
	fclose(config_file);

	Time time = {0};
	entity_chain_add(&time.timers, &animation_timer_x, 1, false)->count = 1;
	entity_chain_add(&time.timers, &animation_timer_y, 1, false)->count = 1;

	FrameSequenceState my_animation_x = {.timer = &animation_timer_x};
	FrameSequenceState my_animation_y = {.timer = &animation_timer_y};

	float vertices[] = {
		-1.0, 1.0,
		1.0, 1.0,
		1.0, -1.0,
		-1.0, -1.0,
	};
	uint32_t indices[] = {0, 1, 2, 2, 3, 0};
	Shape rect = glutils_setup_shape(vertices, 8, indices, 6);
	Shader frag = glutils_load_and_compile_shader("fragment.glsl",
			GL_FRAGMENT_SHADER);
	Shader vertex = glutils_load_and_compile_shader("vertex.glsl",
			GL_VERTEX_SHADER);
	Shader shaders[] = {frag, vertex};
	uint32_t shader = glutils_link_shader_program(shaders, 2);
	glUseProgram(shader);
	uint32_t positions_loc = glGetUniformLocation(shader, "offset");
	uint32_t sizes_loc = glGetUniformLocation(shader, "scale");

	uint32_t count = 3;
	Vector2 positions[] = {
		{0.0, 0.0},
		{0.5, 0.5},
		{-0.5, -0.5},
	};
	Vector2 sizes[] = {
		{0.2, 0.2},
		{0.1, 0.1},
		{0.15, 0.2},
	};
	EntityChunk chunks[] = {
		{.id = 0},
		{.id = 1},
		{.id = 2},
	};

	glClearColor(0.0, 0.0, 0.3, 1.0);
	float global_time = glfwGetTime();
	float delta = 0.0;
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		float vx = frame_sequence_state_update(&my_animation_x, my_sequence_x);
		float vy = frame_sequence_state_update(&my_animation_y, my_sequence_y);
		positions[1].x = vx / 5.0 - 1;
		positions[1].y = vy / 5.0 - 1;
		EntityPairIter iter = {};
		collision_update_point_chunks(chunks, positions, count);
		collision_sort_point_chunks(chunks, count);
		while(entity_pair_iter_next(&iter, chunks, count)) {
			uint32_t a_id = chunks[iter.chunk_a].id;
			uint32_t b_id = chunks[iter.chunk_b].id;
			if (collision_aabb_check(positions[a_id], sizes[a_id],
						positions[b_id], sizes[b_id])) {
				printf("(%i, %i), ", a_id, b_id);
			}
		}
		printf("\n");

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			positions[0].x += delta;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			positions[0].x -= delta;
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			positions[0].y += delta;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			positions[0].y -= delta;
		}

		glBindVertexArray(rect.vao);
		glUseProgram(shader);
		glUniform2fv(positions_loc, count, positions);
		glUniform2fv(sizes_loc, count, sizes);
		glDrawElementsInstanced(GL_TRIANGLES, rect.index_count,
				GL_UNSIGNED_INT, 0, count);

		glfwSwapBuffers(window);
		glfwPollEvents();

		time_update(&time, delta);

		delta = global_time;
		global_time = glfwGetTime();
		delta = global_time - delta;
	}
	glfwTerminate();
}

