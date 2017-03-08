#version 410

in vec2 uv;

out vec4 fragColor;

uniform sampler2D decalTex;

void main() { 
	fragColor = vec4(texture(decalTex, uv).xyz, 1);
}