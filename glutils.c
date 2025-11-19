#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "microtools.h"

uint32_t glutils_load_texture(char* path, GLenum format, GLenum type) {
	int channels;
	uint32_t width;
	uint32_t height;
	uint32_t texture;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
	if (data == NULL) {
		printf("Could not open '%s'", path);
		return;
	}
	if (stbi_failure_reason()) {
		printf("STB failure: %s\n", stbi_failure_reason());
		return;
	}
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, channels, width, height,
			0, format, type, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	return texture;
}

Shape glutils_setup_shape(float* vertices, uint32_t vertex_count,
		uint32_t* indices, uint32_t index_count) {
	uint32_t vao;
	uint32_t vbo;
	uint32_t eao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(float),
			vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &eao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eao);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(uint32_t),
			indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);
	return (Shape) {vao, vbo, eao, vertex_count, index_count};
}

Shader glutils_load_and_compile_shader(char* path, GLenum shader_type) {
	FILE* file = fopen(path, "r");
	if (file == NULL) {
		printf("Could not open '%s'\n", path);
		return (Shader) {0};
	}
	fseek(file, 0, SEEK_END);
	size_t size = ftell(file);
	fseek(file, 0, SEEK_SET);
	char* source = calloc(size, sizeof(char));
	fread(source, sizeof(char), size, file);
	fclose(file);
	GLuint shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &source, &size);
	glCompileShader(shader);
	free(source);
	
	GLenum res;
	char message[256];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE) {
		glGetShaderInfoLog(shader, 256, NULL, message);
		printf("Could not compile %s: %s\n", path, message);
	}

	return (Shader) {shader, shader_type};
}

uint32_t glutils_link_shader_program(Shader* shaders, uint32_t count) {
	GLuint program = glCreateProgram();
	for (uint32_t i  =0; i < count; i ++) {
		glAttachShader(program, shaders[i].shader_id);
	}
	glLinkProgram(program);
	return program;
}

void __glutils_on_window_resize(GLFWwindow* window, int n_width, int n_height) {
	glViewport(0, 0, n_width, n_height);
}

void glutils_config_window(GLFWwindow* window) {
	glfwSwapInterval(1);
	glfwSetFramebufferSizeCallback(window, __glutils_on_window_resize);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GEQUAL);
}
