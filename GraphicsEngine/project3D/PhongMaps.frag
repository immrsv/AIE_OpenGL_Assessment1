#version 410

in vec3 vNormal;
in vec3 vPosition;
in vec3 vTangent;
in vec3 vBiTangent;
in vec2 uv;

in vec3 debug;

out vec4 fragColor;

uniform vec3 globalAmbient = vec3(0.05);

vec3 ambientPart = vec3(0);
vec3 diffusePart = vec3(0);
vec3 specularPart = vec3(0);
float alpha = 1.0;

vec3 N;


uniform vec3 lightDir = normalize(vec3(-1,-1,1));
uniform vec3 lightClr = vec3(1.0);

const int MAX_LIGHTS = 8;


uniform int pointLtNum = 0;
uniform vec3 pointLtPos[MAX_LIGHTS];
uniform vec3 pointLtClr[MAX_LIGHTS];
uniform float pointLtPwr[MAX_LIGHTS];

uniform vec3 cameraPos = vec3(0);
uniform float specPow = 20.0;

uniform sampler2D ambientTex;
uniform sampler2D diffuseTex;
uniform sampler2D specularTex;
uniform sampler2D normalTex;

uniform vec4 TexWeights = vec4(0,0,0,0);

vec3 slerp( vec3 origin, vec3 target, float progress) {
	progress = clamp(progress,0,1);

	vec3 axis = cross(origin, target);
	float angle = acos( dot( target, origin) ) * progress;
	
	float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
	// Matrix Code Source: https://gist.github.com/neilmendoza/4512992
    mat3 xform = mat3(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c);

	return xform * origin;
}

vec3 PointLightDiffPart(int i, vec3 L, float att) {
	vec3 result = att * lightClr * max(0.f, dot(N, L));
	return mix( result, result * texture(diffuseTex, uv).rgb, TexWeights.y);
}

vec3 PointLightSpecPart(int i, vec3 L) {
	vec3 E = normalize( cameraPos - vPosition.xyz ).xyz; // Direction to Camera from vPosition
	vec3 R = reflect( lightDir.xyz, N.xyz).xyz; // Reflected Ray
	float s = max( 0, dot( R, E ));	// Specular value
	s = pow( s, specPow ); // Raise to Specular Power

	vec3 result = lightClr * s;
	return mix( specularPart, specularPart * texture(specularTex, uv).rgb, TexWeights.z);
}

void main() { 
	
	ambientPart = mix( globalAmbient, globalAmbient + texture(ambientTex, uv).rgb, TexWeights.x);

	// Mapped Normal Deviation
	vec3 mN = normalize(texture(normalTex, uv).xyz * 2 - vec3(1));
	mat3 TBN = (mat3( vTangent, vBiTangent, mN ));

	N = slerp( vNormal, normalize(TBN * vNormal), TexWeights.w);
	
	// Point Lights
	for ( int i = 0 ; i < min(pointLtNum, MAX_LIGHTS) ; i++ ) {
		vec3 L = normalize(  pointLtPos[i].xyz - vPosition.xyz );
		float t dist = length(pointLtPos[i].xyz - vPosition.xyz);
		float att = 1.0/(1.0 + 0.1*dist + 0.1*dist*dist);

		diffusePart += PointLightDiffPart(i,L);
		specularPart += PointLightSpecPart(i,L);
	}

	// Directional Lights
	diffusePart = lightClr * max(0.f, dot(N, -lightDir));
	diffusePart = mix( diffusePart, diffusePart * texture(diffuseTex, uv).rgb, TexWeights.y);

	vec3 E = normalize( cameraPos - vPosition.xyz ).xyz; // Direction to Camera from vPosition
	vec3 R = reflect( lightDir.xyz, N.xyz).xyz; // Reflected Ray
	float s = max( 0, dot( R, E ));	// Specular value
	s = pow( s, specPow ); // Raise to Specular Power

	specularPart = lightClr * s;
	specularPart = mix( specularPart, specularPart * texture(specularTex, uv).rgb, TexWeights.z);


	fragColor = clamp(vec4( ambientPart + diffusePart + specularPart, alpha), 0, 1);
}

