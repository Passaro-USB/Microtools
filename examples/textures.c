#include "microtools.h"

int main() {
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 600, "example", 0, 0);
	glfwMakeContextCurrent(window);
	glewInit();
	glutils_config_window(window);

	// load texture
	uint32_t my_texture = glutils_load_texture("../assets/example.png",
			GL_RGBA, GL_UNSIGNED_BYTE);

	// load shaders
	Shader shaders[] = {
		glutils_load_and_compile_shader("../assets/textures_frag.glsl",
				GL_FRAGMENT_SHADER),
		glutils_load_and_compile_shader("../assets/textures_vertex.glsl",
				GL_VERTEX_SHADER)
	};
	uint32_t shader_program = glutils_link_shader_program(shaders, 2);

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

	while(!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader_program);
		glBindVertexArray(square.vao);
		glDrawElements(GL_TRIANGLES, square.index_count,
				GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
