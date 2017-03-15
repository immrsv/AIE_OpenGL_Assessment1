#version 410

in vec2 uv;

out vec4 fragColor;

uniform vec3 decalClr = vec3(0);
uniform sampler2D decalTex;

void main() { 
	vec3 fragPart = texture(decalTex, uv).xyz;
	fragColor = vec4(fragPart + decalClr, 1);
	//fragColor = vec4(0,0,0,1);
}