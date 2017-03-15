#version 410

in vec2 uv;

out vec4 fragColor;

uniform int blurSize = 5;
uniform int bloomSize = 0;
uniform float sobelWeight = 1.0;

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

vec3 compositeClr;
float compositeAlpha;
float compositeWeight;

void blur( ) {
	vec3 compound;
	float compoundAlpha = 0.;
	float compoundWeight = 1.;

	for ( int i= -blurSize ; i <= blurSize; i++ ) {
		for ( int j = -blurSize ; j <= blurSize ; j++) {
			if ( i != 0 && j != 0 ) {
				vec2 offset = texelSize * vec2(i,j);
				float weight = 1;// - smoothstep( 0, blurSize, length(vec2(i,j)) );

				vec4 fragment = texture(decal, uv + offset);

				compound += weight * fragment.rgb;
				compoundAlpha += fragment.a;
				compoundWeight += 1;
				
			}
		}
	}

	compositeClr += compound;
	compositeClr /= compoundWeight;
	compositeAlpha += compoundAlpha;
	compositeWeight += compoundWeight;
}

void bloom( ) {
	vec3 compound;
	float compoundAlpha = 0.;
	float compoundWeight = 0.;

	vec2 maxOffset = texelSize * max(1, bloomSize);

	for ( int i= -bloomSize ; i <= bloomSize; i++ ) {
		for ( int j = -bloomSize ; j <= bloomSize ; j++) {
			if ( i != 0 && j != 0 ) {
				vec2 offset = texelSize * vec2(i,j);
				float weight = 1 - smoothstep( length(vec2(i,2*j)), 0., 1. * bloomSize ); // Smoothly weight texels based on radius from origin

				vec4 fragment = texture(decal, uv + offset);
				vec3 component = step( vec3(0.65), fragment.rgb ); // Ignore colour channels with < 65% saturation

				if ( length( component ) >= 0.95 ) {
					compound += weight * pow( component * fragment.rgb, vec3(14));
					compoundAlpha += fragment.a;				
					compoundWeight += weight;
				}
			}
		}
	}

	compositeClr += compound;
	compositeAlpha += compoundAlpha;
	compositeWeight += compoundWeight;
}

void sobel( ) {
	// Sobel edges
	float edgeV, edgeH;

	for ( int i = -1 ; i <= 1 ; i++ ) {
		for ( int j = -1; j <= 1 ; j++ ) {
			vec2 offset = texelSize * vec2(i,j);
			vec4 fragment = texture(decal, uv + offset);
			float lum = max(max( fragment.r, fragment.g), fragment.b);

			edgeH += kernelH[i+1][j+1] * lum;
			edgeV += kernelV[i+1][j+1] * lum;
		}
	}

	compositeClr -= sobelWeight * vec3( sqrt( edgeH*edgeH + edgeV*edgeV ) / 2.);
}

void main() { 

	vec4 sourcePart = texture(decal, uv);

	compositeClr = sourcePart.rgb;
	compositeAlpha = sourcePart.a;

	blur();
	bloom();
	sobel();
	
	fragColor = vec4(compositeClr.rgb, compositeAlpha / compositeWeight);
}