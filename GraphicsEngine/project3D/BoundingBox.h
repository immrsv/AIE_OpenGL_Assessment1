#pragma once

#include <glm/glm.hpp>

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

class BoundingBox
{
public:
	BoundingBox(vec3 min, vec3 max);
	~BoundingBox();

	vec3 m_min; // Minimum bounds (Model Space)
	vec3 m_max; // Maximum bounds (Model Space)

	bool isOffScreen(mat4& mvp);

	void draw();

private:
	bool hasGeomtery = false;
	void buildGeometry();

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_VIO;


};

