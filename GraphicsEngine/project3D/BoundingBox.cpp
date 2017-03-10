#include "BoundingBox.h"

#include <gl_core_4_4.h>
#include <iostream>

vec3 BoundingBox::ClipSpace = vec3(1,1,0);

BoundingBox::BoundingBox(vec3 min, vec3 max)
	: m_min(min), m_max(max)
{
}


BoundingBox::~BoundingBox()
{
	glDeleteBuffers(1, &m_IBO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteVertexArrays(1, &m_VAO);
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

	const vec3 test = vec3(1, 1, 0);

	auto offMin = glm::lessThan(screenMax * test, -BoundingBox::ClipSpace);
	auto offMax = glm::greaterThan(screenMin * test, BoundingBox::ClipSpace);

	return (glm::any(offMin) || glm::any(offMax));
}


void BoundingBox::draw() {
	if (!hasGeomtery) buildGeometry();

	GLenum err;
	err = glGetError();
	if (err != GL_NO_ERROR)
		std::cerr << "BoundingBox::draw() - Pre Draw : " << err << std::endl;

	glBindVertexArray(m_VAO);
	//glLineWidth(2.0f);
	glDrawElements(GL_LINE_STRIP, 20, GL_UNSIGNED_INT, 0);

	err = glGetError();
	if (err != GL_NO_ERROR)
		std::cerr << "BoundingBox::draw() - Post Draw : " << err << std::endl;
}


void BoundingBox::buildGeometry() {

	vec3 corners[8];
	int elements[] = // 20 points, 19 lines
		{ 4,5,7,6,7,3,2,3,7,5,1,5,4,0,4,6,2,0,1,3 }; // Unwrapped cube
	int idx = 0;

	for (int i = 0; i < 8; i++) { // For 8 corners
		corners[i] = m_min;

		corners[i].x = (i & 1) ? m_min.x : m_max.x;
		corners[i].y = (i & 2) ? m_min.y : m_max.y;
		corners[i].z = (i & 4) ? m_min.z : m_max.z;

		//if (i & 1) {
		//	corners[i].x = m_max.x;
		//	elements[idx++] = i; // This corner
		//	elements[idx++] = i ^ 1; // This corner's inverse-x partner
		//}

		//if (i & 2) {
		//	corners[i].y = m_max.y;
		//	elements[idx++] = i; // This corner
		//	elements[idx++] = i ^ 2; // This corner's inverse-y partner
		//}
		//
		//if (i & 4) {
		//	corners[i].y = m_max.z;
		//	elements[idx++] = i; // This corner
		//	elements[idx++] = i ^ 4; // This corner's inverse-y partner
		//}

	}

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * 8, corners, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 20, elements, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
	
	// Detach
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	hasGeomtery = true;
}