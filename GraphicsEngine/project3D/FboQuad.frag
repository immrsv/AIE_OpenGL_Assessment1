#version 410

in vec2 uv;

out vec4 fragColor;

uniform sampler2D decal;
uniform vec2 texelSize;

const int radius = 5;

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

	//compound /= totalWeight;

	fragColor = vec4(texture(decal, uv).rgb + compound, compoundAlpha / totalWeight);
	//fragColor = vec4( step( vec3(0.95), texture(decal, uv).rgb ), 1);
}