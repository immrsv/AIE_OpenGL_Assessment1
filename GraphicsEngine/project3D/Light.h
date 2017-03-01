#pragma once

#include <glm/glm.hpp>

using glm::vec4;
using glm::vec3;

class Light
{
public:
	Light(int idx, vec3 pos, vec3 clr, vec3 coeff, float pwr)
		: index(idx), Position(pos), Color(clr), Coeff(coeff), Power(pwr) {};
	~Light();

	int index;
	vec3 Position;
	vec3 Color;
	vec3 Coeff; // Coefficients: Constant, Linear, Quadratic
	float Power;

};

