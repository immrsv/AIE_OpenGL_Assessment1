#include "Mirror.h"

#include <gl_core_4_4.h>
#include <iostream>
#include <glm/ext.hpp>

Mirror::Mirror(vec2 size)
	: m_size(size)
{
	return; // breakpoint
}


Mirror::~Mirror()
{
}


Camera& Mirror::reflect(Transform* mirror, Transform* camera) {
	
	mat4 transform = mirror->getMatrix();

	vec3 worldNormal = vec3(transform * vec4(Transform::vOut,0));
	
	// Position and orientation
	vec3 view = camera->getPosition() - mirror->getPosition();	// Vector from centre point to camera
	vec3 nView = glm::normalize(view);								// Normalised vector

	vec3 axis = glm::cross(nView, worldNormal);						// Find axis of rotation from view to normal
	float halfAngle = acos(glm::dot(nView, worldNormal));			// Find angle between view and normal
	float inverseAngle = (glm::pi<float>() / 2.0f) - halfAngle;		// Find angle between view and reflection plane
	
	glm::quat rotation = glm::quat(2 * inverseAngle, axis);			// Construct quaternion

	vec3 rView = vec3(glm::mat4_cast(rotation) * glm::vec4(view, 0)); // vector from "reflection camera" to centre point


	// Construct Camera
	Camera& result = m_camera;
	result.setOrientation(mirror->getOrientation());
	result.setPosition(camera->getPosition() + rView);


	// Configure Frustum
	glm::vec4 topRight = glm::vec4((Transform::vUp + Transform::vRight) * vec3(m_size, 0) / 2.0f, 1); // Model Space
	glm::vec4 bottomLeft = glm::vec4((Transform::vUp + Transform::vRight) * vec3(m_size, 0) / -2.0f, 1);

	topRight = transform * topRight; // World Space
	bottomLeft = transform * bottomLeft;

	topRight = result.getPvMatrix() * topRight;	// Camera Space
	bottomLeft = result.getPvMatrix() * bottomLeft;

	vec3 mins = glm::min(vec3(topRight), vec3(bottomLeft)); // Find mins and maxes
	vec3 maxs = glm::max(vec3(topRight), vec3(bottomLeft));

	result.setProjection(glm::frustum(mins.x, maxs.x, mins.y, maxs.y, FLT_EPSILON, FLT_MAX));


	// Return
	return result;
}

void Mirror::Init() {
	m_buffer.Init(500,500);
	buildQuad();
}

void Mirror::Begin() {
	m_buffer.Begin();
}

void Mirror::End() {
	m_buffer.End();
}

void Mirror::draw() {
	// draw out full-screen quad
	GLenum err;
	err = glGetError();
	if (err != GL_NO_ERROR)
		std::cerr << "Mirror::Draw() - Pre Draw : " << err << std::endl;

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	err = glGetError();
	if (err != GL_NO_ERROR)
		std::cerr << "Mirror::Draw() - Post Draw : " << err << std::endl;
}

void Mirror::buildQuad() {
	float halfTexel[] = { 0.5f / width, 0.5f / height };
	float vertices[] = // Texture must be flipped on X (around Y)
	{
		-m_size.x / 2, -m_size.y / 2, 0, 1, 1 - halfTexel[0], halfTexel[1],
		m_size.x / 2, -m_size.y / 2, 0, 1, halfTexel[0], halfTexel[1],
		m_size.x / 2, m_size.y / 2, 0, 1, halfTexel[0], 1 - halfTexel[1],
		-m_size.x / 2, m_size.y / 2, 0, 1, 1 - halfTexel[0], 1 - halfTexel[1]
	};

	m_texelSize[0] = halfTexel[0] * 2;
	m_texelSize[1] = halfTexel[1] * 2;

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // position
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

	glEnableVertexAttribArray(2); // UVs
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, ((char*)0) + 16);

	// Detach
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


