#version 440 core

layout (location = 0) in vec2 position;

uniform vec2 offset[2];
uniform vec2 scale[2];

out vec2 texPos;

void main() {
	gl_Position = vec4(position * scale[gl_InstanceID] + offset[gl_InstanceID],
				1.0, 1.0);
	texPos = position / 2.0 + 0.5;
}
