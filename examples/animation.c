#include "microtools.h"

int main() {
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 600, "example", 0, 0);
	glfwMakeContextCurrent(window);
	glewInit();
	glutils_config_window(window);

	// setup face object
	Vector2 position = {0.0, 0.0};
	Vector2 size = {0.4, 0.4};
	EntityChunk chunks = {.id = 1};
	uint32_t target_column = 0;
	uint32_t target_row = 0;
	uint32_t frame = 0;
	bool is_blinking = false;

	// setup animation timers
	Time time = {};
	Timer blink_timer = {};
	Timer line_timer = {};
	entity_chain_add(&time.timers, &blink_timer, 1, false)->count = 1;
	entity_chain_add(&time.timers, &line_timer, 1, false)->count = 1;

	// load animations
	FILE* config = fopen("../assets/animation.config", "r");
	Asset animations = asset_loader_read_next(config);
	Frame blink_frames[2] = {0};
	Frame line_frames[3] = {0};
	FrameSequence blink_control = {.frames = blink_frames};
	FrameSequence line_animation = {.frames = line_frames};
	asset_read_frame_sequence_field(config, animations,
			"blink", &blink_control, 2);
	asset_read_frame_sequence_field(config, animations,
			"line", &line_animation, 3);
	FrameSequenceState blink_control_state = {.timer = &blink_timer};
	FrameSequenceState line_animation_state = {.timer = &line_timer};
	fclose(config);

	// load texture
	uint32_t my_texture = glutils_load_texture("../assets/animation_atlas.png",
			GL_RGBA, GL_UNSIGNED_BYTE);

	// load shaders
	Shader shaders[] = {
		glutils_load_and_compile_shader("../assets/animation_frag.glsl",
				GL_FRAGMENT_SHADER),
		glutils_load_and_compile_shader("../assets/collision_vertex.glsl",
				GL_VERTEX_SHADER)
	};
	uint32_t shader_program = glutils_link_shader_program(shaders, 2);
	uint32_t positions_locations = glGetUniformLocation(shader_program, "offset");
	uint32_t sizes_locations = glGetUniformLocation(shader_program, "scale");
	uint32_t frame_location = glGetUniformLocation(shader_program, "frame");

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

		// update object positions
		float target_x = (((float)target_column - 3) / 5.0);
		float target_y = (((float)target_row - 1) / 3.0);
		float x_c = transit_tween_get_coefficient(0.5);
		float y_c = transit_tween_get_coefficient(0.7);
		position.x = transit_tween_get_next_instant(
				position.x, target_x, x_c, delta);
		position.y = transit_tween_get_next_instant(
				position.y, target_y, y_c, delta);


		// update object targets
		int window_x;
		int window_y;
		glfwGetWindowSize(window, &window_x, &window_y);
		double mouse_x;
		double mouse_y;
		glfwGetCursorPos(window, &mouse_x, &mouse_y);
		float rel_mouse_x = ((float)mouse_x / (float)window_x) * 2.0 - 1.0;
		float rel_mouse_y = (1.0 - (float)mouse_y / (float)window_y) * 2.0 - 1.0;
		if (fabs(target_x - rel_mouse_x) < 0.3) {
			if (target_x < rel_mouse_x) {
				target_column = (uint32_t)fmax((int32_t)target_column - 1, 0);
			} else {
				target_column = (uint32_t)fmin(target_column + 1, 5);
			}
		}
		if (fabs(target_y - rel_mouse_y) < 0.3) {
			if (target_y < rel_mouse_y) {
				target_row = (uint32_t)fmax((int32_t)target_row - 1, 0);
			} else {
				target_row = (uint32_t)fmin(target_row + 1, 3);
			}
		}

		// update object animations
		is_blinking = (uint32_t) frame_sequence_state_update(
				&blink_control_state, blink_control) == 1;
		frame = (uint32_t) frame_sequence_state_update(
				&line_animation_state, line_animation);
		if (is_blinking) {
			frame = 3;
		}
		if (fabs(position.x - rel_mouse_x) < 0.1 
				&& fabs(position.y - rel_mouse_y) < 0.2) {
			frame = 4;
		}

		// update object uniforms
		glUniform2fv(positions_locations, 1, &position);
		glUniform2fv(sizes_locations, 1, &size);
		glUniform1i(frame_location, frame);

		// render
		glBindVertexArray(square.vao);
		glDrawElementsInstanced(GL_TRIANGLES, square.index_count,
				GL_UNSIGNED_INT, 0, 1);

		glfwSwapBuffers(window);
		glfwPollEvents();

		// update time
		time_update(&time, delta);

		//update delta
		delta = global_timer;
		global_timer = glfwGetTime();
		delta = global_timer - delta;
	}
}

