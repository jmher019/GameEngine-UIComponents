#version 460 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D text;
uniform vec4 textColor;

void main()
{    
    vec4 sampled = vec4(1.f, 1.f, 1.f, texture(text, TexCoords).r);
	gl_FragDepth = gl_FragCoord.z;
	if (sampled.w == 0.f) {
		gl_FragDepth = 0.f;
	}
    FragColor = textColor * sampled;
} 