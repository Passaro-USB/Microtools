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
	Shape triangle = glutils_setup_shape(vertices, 8, indices, 6);
	uint32_t sprite = glutils_load_texture("test_sprite.png", GL_RGBA, GL_UNSIGNED_BYTE);
	
	glUseProgram(program);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, sprite);
	glUniform1i(glGetUniformLocation(program, "tex"), 0);
	
	glClearColor(0.0, 0.4, 0.7, 1.0);
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(program);
		glBindVertexArray(triangle.vao);
		glDrawElements(GL_TRIANGLES, triangle.index_count, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}

