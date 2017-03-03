#include "Mirror.h"

#include <gl_core_4_4.h>

Mirror::Mirror(vec3 position, glm::quat orientation, vec2 size, float quality)
	: m_transform(position, orientation), m_size(size), m_reflectivity(quality)
{
}


Mirror::~Mirror()
{
}


Camera Mirror::reflect(Camera& camera) {
	
	mat4 transform = m_transform.getMatrix();

	vec3 worldNormal = vec3(transform * vec4(Transform::vOut,0));
	
	// Position and orientation
	vec3 view = camera.getPosition() - m_transform.getPosition();	// Vector from centre point to camera
	vec3 nView = glm::normalize(view);								// Normalised vector

	vec3 axis = glm::cross(nView, worldNormal);						// Find axis of rotation from view to normal
	float halfAngle = acos(glm::dot(nView, worldNormal));			// Find angle between view and normal
	float inverseAngle = (glm::pi<float>() / 2.0f) - halfAngle;		// Find angle between view and reflection plane
	
	glm::quat rotation = glm::quat(2 * inverseAngle, axis);			// Construct quaternion

	vec3 rView = vec3(glm::mat4_cast(rotation) * glm::vec4(view, 0)); // vector from "reflection camera" to centre point


	// Construct Camera
	Camera result;
	result.setOrientation(m_transform.getOrientation());
	result.setPosition(camera.getPosition() + rView);


	// Configure Frustum
	glm::vec4 topRight = glm::vec4((Transform::vUp + Transform::vRight) * vec3(m_size, 0) / 2.0f, 1); // Model Space
	glm::vec4 bottomLeft = glm::vec4((Transform::vUp + Transform::vRight) * vec3(m_size, 0) / -2.0f, 1);

	topRight = transform * topRight; // World Space
	bottomLeft = transform * bottomLeft;

	topRight = result.getTransform() * topRight;	// Camera Space
	bottomLeft = result.getTransform() * bottomLeft;

	vec3 mins = glm::min(vec3(topRight), vec3(bottomLeft)); // Find mins and maxes
	vec3 maxs = glm::max(vec3(topRight), vec3(bottomLeft));

	result.setProjection(glm::frustum(mins.x, maxs.x, mins.y, maxs.y, FLT_EPSILON, FLT_MAX));


	// Return
	return result;
}

void Mirror::Init() {
	m_buffer.Init(500,500);
}



void Mirror::buildQuad() {
	float halfTexel[] = { 0.5 / width, 0.5 / height };
	float vertices[] =
	{
		-1, -1, 0, 1, halfTexel[0], halfTexel[1],
		1, 1, 0, 1, 1 - halfTexel[0], 1 - halfTexel[1],
		-1, 1, 0, 1, halfTexel[0], 1 - halfTexel[1],
		-1, -1, 0, 1, halfTexel[0], halfTexel[1],
		1, -1, 0, 1, 1 - halfTexel[0], halfTexel[1],
		1, 1, 0, 1, 1 - halfTexel[0], 1 - halfTexel[1],
	};

	m_texelSize[0] = halfTexel[0] * 2;
	m_texelSize[1] = halfTexel[1] * 2;

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6, vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // position
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

	glEnableVertexAttribArray(1); // UVs
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, ((char*)0) + 16);

	// Detach
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}