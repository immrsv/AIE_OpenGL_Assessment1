#include "BoundingBox.h"



BoundingBox::BoundingBox(vec3 min, vec3 max)
	: m_min(min), m_max(max)
{
}


BoundingBox::~BoundingBox()
{
}

bool BoundingBox::isOffScreen(mat4& mvp) {

	vec4 mCorner; // Model Space
	glm::vec3 screenMin(FLT_MAX, FLT_MAX, FLT_MAX);
	glm::vec3 screenMax(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (int i = 0; i < 8; i++) { // For 8 corners

		mCorner.x = (i & 1) ? m_min.x : m_max.x;
		mCorner.y = (i & 2) ? m_min.y : m_max.y;
		mCorner.z = (i & 4) ? m_min.z : m_max.z;
		mCorner.w = 1;

		vec4 cCorner = mvp * mCorner;	// Clip Space
		//do the perspective divide
		cCorner.x /= cCorner.w;
		cCorner.y /= cCorner.w;
		cCorner.z /= cCorner.w;

		screenMin = glm::min(vec3(cCorner), screenMin);
		screenMax = glm::max(vec3(cCorner), screenMax);
	}

	

	const vec3 edge = vec3(0.75);
	const vec3 test = vec3(1, 1, 0);

	auto offMin = glm::lessThan(screenMax * test, -edge);
	auto offMax = glm::greaterThan(screenMin * test, edge);

	return (glm::any(offMin) || glm::any(offMax));
}