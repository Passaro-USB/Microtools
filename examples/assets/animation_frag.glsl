#version 440 core

in vec2 texPos;

uniform sampler2D tex;
uniform int frame;

out vec4 fragColor;

void main() {
	vec2 resPos = vec2(texPos.x / 5.0  + frame / 5.0, texPos.y);
	fragColor = texture(tex, resPos);
}
