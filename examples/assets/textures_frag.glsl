#version 440 core

in vec2 texPos;

uniform sampler2D tex;

void main() {
	gl_FragColor = texture(tex, texPos);
}
