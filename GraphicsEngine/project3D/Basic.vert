// VERTEX SHADER 
#version 410 

layout(location=0) in vec4 position;
layout(location=1) in vec4 normal;
layout(location=2) in vec2 texcoord;

out vec3 vPosition;
out vec3 vNormal;
out vec2 uv;

uniform mat4 pvmMatrix;
uniform mat4 modelMatrix;

void main() {
	// Homogenise Position and Normal
	vec4 hP = vec4(position.xyz, 1);
	vec4 hN = vec4(normal.xyz, 0);

	// Apply Transforms
	vPosition = (modelMatrix * hP).xyz;
	vNormal = (inverse(transpose(modelMatrix)) * hN).xyz; 
	//vNormal = (modelMatrix * hN).xyz; 

	uv = texcoord;

	// Set gl Position
	gl_Position = pvmMatrix * hP; 
}