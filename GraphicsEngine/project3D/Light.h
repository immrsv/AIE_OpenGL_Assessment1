#pragma once

#include <glm/glm.hpp>

using glm::vec4;
using glm::vec3;

class Light
{
public:
	Light();
	~Light();

	static vec3 _positions[10];
	static vec3 _colour[10];
	static float _intensity[10];

};

