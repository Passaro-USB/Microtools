#version 440 core

layout (location = 0) in vec2 position;

out vec2 texPos;

void main() {
	gl_Position = vec4(position, 1.0, 1.0);
	texPos = position / 2.0 + 0.5;
}
