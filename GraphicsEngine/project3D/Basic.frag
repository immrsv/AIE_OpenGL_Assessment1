#version 410

in vec4 vNormal;

out vec4 fragColor;

uniform vec3 globalAmbient = vec3(0.2,0.2,0.2);
vec4 lightDir = normalize(vec4(0.7,0.7,0,0));
vec3 lightClr = vec3(0.5,0.5,0);

void main() { 

	//fragColor = vColour;
	//fragColor = vec4(0.5);

	vec3 diffuse = clamp(lightClr * dot(vNormal, -lightDir),0,1);

	fragColor = vec4( globalAmbient + diffuse, 1.0);
}