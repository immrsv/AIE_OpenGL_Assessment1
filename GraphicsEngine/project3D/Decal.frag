#version 410

in vec2 uv;

out vec4 fragColor;

uniform sampler2D decalTex;

void main() { 
	vec3 fragPart = texture(decalTex, uv).xyz;
	fragColor = vec4(mix(vec3(0), fragPart, length(fragPart)), 1);
	//fragColor = vec4(0,0,0,1);
}