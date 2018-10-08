#version 460 core
layout (location = 0) in vec3 pos; // position
layout (location = 1) in vec3 norm; // normal
layout (location = 2) in vec3 color; // color
layout (location = 3) in vec2 texCoord; // texture position

uniform mat4 PVM;
uniform vec4 fillColor;

out vec4 shapeFillColor; // specify a color output to the fragment shader;

void main() {
	gl_Position = PVM * vec4(pos, 1.f);
	shapeFillColor = fillColor;
}