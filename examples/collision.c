#include "microtools.h"

#define OBJECT_COUNT 2

int main() {
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 600, "example", 0, 0);
	glfwMakeContextCurrent(window);
	glewInit();
	glutils_config_window(window);

	// setup objects
	Vector2 positions[OBJECT_COUNT] = {
		{0.0, 0.0}, {0.0, -0.8}
	};
	Vector2 sizes[OBJECT_COUNT] = {
		{0.2, 0.2}, {1.0, 0.1}
	};
	EntityChunk chunks[OBJECT_COUNT] = {
		{.id = 0}, {.id = 1}
	};
	float player_speed = 0;

	// load texture
	uint32_t my_texture = glutils_load_texture("../assets/example.png",
			GL_RGBA, GL_UNSIGNED_BYTE);

	// load shaders
	Shader shaders[] = {
		glutils_load_and_compile_shader("../assets/collision_frag.glsl",
				GL_FRAGMENT_SHADER),
		glutils_load_and_compile_shader("../assets/collision_vertex.glsl",
				GL_VERTEX_SHADER)
	};
	uint32_t shader_program = glutils_link_shader_program(shaders, 2);
	uint32_t positions_locations = glGetUniformLocation(shader_program, "offset");
	uint32_t sizes_locations = glGetUniformLocation(shader_program, "scale");

	// set the uniform to the texture
	glUseProgram(shader_program);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, my_texture);
	glUniform1i(glGetUniformLocation(shader_program, "tex"), 0);

	// setup square shape
	float vertices[] = {
		-1.0, 1.0,
		1.0, 1.0,
		1.0, -1.0,
		-1.0, -1.0
	};
	uint32_t indices[] = {0, 1, 2, 2, 3, 0};
	Shape square = glutils_setup_shape(vertices, 8, indices, 6);

	float global_timer = glfwGetTime();
	float delta = 0;
	while(!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader_program);

		// update the ugly physics
		player_speed += 10 * delta;
		positions[0].y -= player_speed * delta;

		// iter through broad collisions
		collision_update_point_chunks(chunks, positions, OBJECT_COUNT);
		collision_sort_point_chunks(chunks, OBJECT_COUNT);
		EntityPairIter iter = {};
		while (entity_pair_iter_next(&iter, chunks, OBJECT_COUNT)) {
			uint32_t a_id = chunks[iter.chunk_a].id;
			uint32_t b_id = chunks[iter.chunk_b].id;
			if (collision_aabb_check(positions[a_id], sizes[a_id],
						positions[b_id], sizes[b_id])) {
				printf("Look, a collision! (%i, %i)\n", a_id, b_id);
				// if the player is involved
				if (a_id == 0 || b_id == 0) {
					player_speed = 0;
					positions[0].y = positions[1].y + sizes[1].y 
									+ sizes[0].y;
				}
			}
		}

		// player input
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			printf("JUMP\n");
			player_speed -= 50 * delta;
		}

		// update object uniforms
		glUniform2fv(positions_locations, OBJECT_COUNT, positions);
		glUniform2fv(sizes_locations, OBJECT_COUNT, sizes);

		// render
		glBindVertexArray(square.vao);
		glDrawElementsInstanced(GL_TRIANGLES, square.index_count,
				GL_UNSIGNED_INT, 0, OBJECT_COUNT);

		glfwSwapBuffers(window);
		glfwPollEvents();

		//update delta
		delta = global_timer;
		global_timer = glfwGetTime();
		delta = global_timer - delta;
	}
}

