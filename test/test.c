#include <stdio.h>
#include "microtools.h"

int main() {
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 600, "Wah Wah", 0, 0);
	glViewport(0, 0, 800, 600);
	glfwMakeContextCurrent(window);
	glewInit();
	glutils_config_window(window);

	Shader vertex = glutils_load_and_compile_shader("vertex.glsl", GL_VERTEX_SHADER);
	Shader frag = glutils_load_and_compile_shader("fragment.glsl", GL_FRAGMENT_SHADER);
	Shader shaders[] = {vertex, frag};
	GLuint program = glutils_link_shader_program(shaders, 2);

	float vertices[] = {
		-1.0, 1.0,
		1.0, 1.0,
		1.0, -1.0,
		-1.0, -1.0
	};
	uint32_t indices[] = {0, 1, 2, 2, 3, 0};
	Shape square = glutils_setup_shape(vertices, 8, indices, 6);
	uint32_t sprite = glutils_load_texture("test_sprite.png", GL_RGBA, GL_UNSIGNED_BYTE);
	
	glUseProgram(program);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, sprite);
	glUniform1i(glGetUniformLocation(program, "tex"), 0);
	glUniform2f(glGetUniformLocation(program, "offset"), 0.0, 0.0);

	Time time = {};
	Timer switch_timer = {.value = 1.0, .max = 3.0, .type = COUNT_DOWN};
	entity_chain_add(&time.timers, &switch_timer, 1, false)->count = 1;
	
	glClearColor(0.0, 0.4, 0.7, 1.0);
	float current_time = glfwGetTime();
	float delta = 0;
	float y = 0.0;
	float target = 0.8;
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(program);
		if (switch_timer.just_reached_max) {
			((Timer*)time.timers.first->entities)->value = switch_timer.max;
			target *= -1;
		}
		float oy = transit_linear_get(-target, target, 0.999, switch_timer.max - switch_timer.value);
		// diff is always lower than 0.01, so I am considering the two to be equivalent
		printf("%f | %f | %f | diff: %f\n", switch_timer.value, y, oy, fabs(y - oy));
		y = transit_linear_get_next_instant(y, target, 0.999, delta);
		glUniform2f(glGetUniformLocation(program, "offset"), 0.0, y);
		glBindVertexArray(square.vao);
		glDrawElements(GL_TRIANGLES, square.index_count, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
		time_update(&time, delta);

		delta = current_time;
		current_time = glfwGetTime();
		delta = current_time - delta;
	}
	glfwTerminate();
}

