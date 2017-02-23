#version 410

layout(location=0) in vec4 position;
layout(location=1) in vec4 normal;

out vec4 vNormal;

uniform mat4 pvmMatrix;

void main() {
	vNormal = normal; 
	gl_Position = pvmMatrix * position; 
}