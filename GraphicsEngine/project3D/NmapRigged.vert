// VERTEX SHADER 
#version 410 

layout(location=0) in vec4 position;
layout(location=1) in vec4 normal;
layout(location=2) in vec4 tangent;
layout(location=3) in vec2 texcoord;
layout(location=4) in vec4 weights;
layout(location=5) in vec4 indices;

out vec3 vNormal;
out vec3 vPosition;
out vec3 vTangent;
out vec3 vBiTangent;
out vec2 uv;

out vec3 debug;

uniform mat4 pvmMatrix;
uniform mat4 modelMatrix;
uniform bool hasBones = false;

const int MAX_BONES = 128;
uniform mat4 bones[MAX_BONES];

void main() {

	uv = texcoord;

	// Homogenise Position and Normal
	vec4 hP = vec4(position.xyz, 1);
	vec4 hN = vec4(normal.xyz, 0);
	vec4 hT = vec4(tangent.xyz, 0);

	if (hasBones) {
		ivec4 index = ivec4( indices );

		// Apply Rigging Transforms
		vec4 rP = bones[ index.x ] * hP * weights.x;
			rP += bones[ index.y ] * hP * weights.y;
			rP += bones[ index.z ] * hP * weights.z;
			rP += bones[ index.w ] * hP * weights.w;

		vec4 rN = inverse(transpose(bones[ index.x ])) * hN * weights.x;
			rN += inverse(transpose(bones[ index.y ])) * hN * weights.y;
			rN += inverse(transpose(bones[ index.z ])) * hN * weights.z;
			rN += inverse(transpose(bones[ index.w ])) * hN * weights.w;

		vec4 rT = inverse(transpose(bones[ index.x ])) * hT * weights.x;
			rT += inverse(transpose(bones[ index.y ])) * hT * weights.y;
			rT += inverse(transpose(bones[ index.z ])) * hT * weights.z;
			rT += inverse(transpose(bones[ index.w ])) * hT * weights.w;

		hP = vec4(rP.xyz, 1);
		hN = vec4(normalize(rN.xyz), 0);
		hT = vec4(normalize(rT.xyz), 0);
	}

	// Apply World-Space Transforms
	vPosition = (modelMatrix * hP).xyz;
	vNormal = normalize((inverse(transpose(modelMatrix)) * hN).xyz); 
	vTangent = normalize((inverse(transpose(modelMatrix)) * hT).xyz);
	vBiTangent = cross(vNormal, vTangent);

	// Set gl Position
	gl_Position = pvmMatrix * hP; 
}