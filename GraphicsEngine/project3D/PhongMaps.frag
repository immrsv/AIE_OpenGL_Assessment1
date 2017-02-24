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

uniform vec3 lightDir = normalize(vec3(-1,-1,-1));
uniform vec3 lightClr = vec3(1.0);

const int MAX_LIGHTS = 8;
uniform vec4 lightsPos[MAX_LIGHTS];
uniform vec3 lightsClr[MAX_LIGHTS];
uniform float lightsPwr[MAX_LIGHTS];

uniform vec3 cameraPos = vec3(0);
uniform float specPow = 20.0;

uniform sampler2D ambientTex;
uniform sampler2D diffuseTex;
uniform sampler2D specularTex;
uniform sampler2D normalTex;

uniform vec4 TexWeights = vec4(0,0,0,0);

void main() { 
	
	vec3 ambientPart = mix( globalAmbient, globalAmbient + texture(ambientTex, uv).rgb, TexWeights.x);

	// Mapped Normal Deviation
	vec3 mN = normalize(texture(normalTex, uv).rgb * 2 - vec3(1));
	mat3 TBN = (mat3( vTangent, vBiTangent, mN ));

	vec3 N = mix( vNormal, normalize(TBN * vNormal), TexWeights.w);
	

	diffusePart = lightClr * clamp(dot(N, -lightDir), 0., 1.);
	diffusePart = mix( diffusePart, diffusePart * texture(diffuseTex, uv).rgb, TexWeights.y);

	vec3 E = normalize( cameraPos - vPosition.xyz ).xyz; // Direction to Camera from vPosition
	vec3 R = reflect( lightDir.xyz, N.xyz).xyz; // Reflected Ray
	float s = max( 0, dot( R, E ));	// Specular value
	s = pow( s, specPow ); // Raise to Specular Power

	specularPart = lightClr * s;
	specularPart = mix( specularPart, specularPart * texture(specularTex, uv).rgb, TexWeights.z);

	// Apply Texture as decal
	//diffuse = texture(diffuseTex, uv).xyz;

	fragColor = clamp(vec4( ambientPart + diffusePart + specularPart, alpha), 0, 1);
	//fragColor = vec4(debug, 1);
	//fragColor = vec4(specular.rgb, 1);
}