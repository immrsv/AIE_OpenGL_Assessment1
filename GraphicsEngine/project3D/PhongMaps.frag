#version 410

in vec3 vNormal;
in vec3 vPosition;
in vec3 vTangent;
in vec3 vBiTangent;
in vec2 uv;

in vec3 debug;

out vec4 fragColor;

uniform vec3 ambient = vec3(0.05);
vec3 diffuse = vec3(0);
vec3 specular = vec3(0);
float alpha = 1.0;

uniform vec3 lightDir = normalize(vec3(-1,-1,-1));
uniform vec3 lightClr = vec3(1.0);

const int MAX_LIGHTS = 8;
uniform vec4 lightsPos[MAX_LIGHTS];
uniform vec3 lightsClr[MAX_LIGHTS];
uniform float lightsPwr[MAX_LIGHTS];

uniform vec3 cameraPos = vec3(0);
uniform float specPow = 20.0;

uniform sampler2D diffuseTex;
uniform sampler2D specularTex;
uniform sampler2D normalTex;

void main() { 
	
	// Mapped Normal Deviation
	vec3 mN = normalize(texture(normalTex, uv).rgb * 2 - vec3(1));
	mat3 TBN = (mat3( vTangent, vBiTangent, mN ));

	vec3 N = normalize(TBN * vNormal);

	diffuse = lightClr * texture(diffuseTex, uv).rgb * clamp(dot(N, lightDir), 0., 1.);
	 

	vec3 E = normalize( cameraPos - vPosition.xyz ).xyz; // Direction to Camera from vPosition
	vec3 R = reflect( -lightDir.xyz, N.xyz).xyz; // Reflected Ray
	float s = max( 0, dot( R, E ));	// Specular value
	s = pow( s, specPow ); // Raise to Specular Power

	specular = lightClr * texture(specularTex, uv).rgb * s;
	

	// Apply Texture instead
	//diffuse *= texture(diffuseTex, uv).xyz;

	fragColor = clamp(vec4( ambient + diffuse + specular, alpha), 0, 1);
	//fragColor = vec4(debug, 1);
	//fragColor = vec4(specular.rgb, 1);
}