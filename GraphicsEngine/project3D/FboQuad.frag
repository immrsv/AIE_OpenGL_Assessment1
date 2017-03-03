#version 410

in vec2 uv;

out vec4 fragColor;

uniform sampler2D decal;
uniform vec2 texelSize;

const int radius = 5;
const mat3 kernelH = 
	mat3(vec3(-1., 0., 1.),
	  vec3(-2., 0., 2.),
	  vec3(-1., 0., 1.) );
const mat3 kernelV = 
	mat3( vec3(-1., -2., -1.),
	  vec3(0.,  0.,  0.),
	  vec3(1.,  2.,  1.) );

void main() { 

	vec3 compound; 
	float compoundAlpha = texture(decal, uv).a;
	float totalWeight = 0;

	vec2 maxOffset = texelSize * max(1, radius);

	for ( int i= -radius ; i <= radius; i++ ) {
		for ( int j = -radius ; j <= radius ; j++) {
			if ( i != 0 && j != 0 ) {
				vec2 offset = texelSize * vec2(i,j);
				float weight = smoothstep( -0.2*length(maxOffset), length(maxOffset), length(maxOffset - offset) );

				vec4 fragment = texture(decal, uv + offset);
				vec3 component = step( vec3(0.65), texture(decal, uv).rgb );
				if ( length( component ) >= 0.95 ) {
					compound += weight * pow( component * fragment.rgb, vec3(10));
					compoundAlpha += fragment.a;
					totalWeight += weight;
				}
			}
		}
	}

	// Sobel edges
	float edgeV, edgeH, edge;

	for ( int i = -1 ; i <= 1 ; i++ ) {
		for ( int j = -1; j <= 1 ; j++ ) {
			vec2 offset = texelSize * vec2(i,j);
			vec4 fragment = texture(decal, uv + offset);
			float lum = max(max( fragment.r, fragment.g), fragment.b);

			edgeH += kernelH[i+1][j+1] * lum;
			edgeV += kernelV[i+1][j+1] * lum;
		}
	}
	edge = sqrt( edgeH*edgeH + edgeV*edgeV ) / 2.;

	//compound /= totalWeight;

	fragColor = vec4(texture(decal, uv).rgb + compound - vec3(edge), compoundAlpha / totalWeight);
	//fragColor = vec4( vec3( edge ), 1);
}