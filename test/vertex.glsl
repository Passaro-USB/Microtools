#version 440 core

layout (location = 0) in vec2 position;

uniform vec2 offset[8];
uniform vec2 scale[8];

out vec2 texPos;

void main() {
	vec2 res = position * scale[gl_InstanceID] + offset[gl_InstanceID];
	gl_Position = vec4(res, 1.0, 1.0);
	texPos = position / 2.0 + 0.5;
}
