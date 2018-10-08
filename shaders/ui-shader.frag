#version 460 core
out vec4 FragColor;

in vec4 shapeFillColor;

void main() {
	FragColor = shapeFillColor;
}