#version 460 core
layout (location = 0) in vec3 pos; // position
layout (location = 1) in vec3 norm; // normal
layout (location = 2) in vec3 color; // color
layout (location = 3) in vec2 texCoord; // texture position
out vec2 TexCoords;

uniform mat4 PVM;

void main()
{
    gl_Position = PVM * vec4(pos, 1.0);
    TexCoords = texCoord;
} 