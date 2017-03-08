#include "Transform.h"


const vec3 Transform::vUp = vec3(0, 1, 0);
const vec3 Transform::vRight = vec3(1, 0, 0);
const vec3 Transform::vOut = vec3(0, 0, 1);


Transform::Transform(const vec3& position, const quat& orientation, const vec3& scale)
	: m_position(position), m_orientation(orientation), m_scale(scale), m_invert(false), isDirty(true)
{
}

Transform::Transform(const bool invert)
	: m_position(), m_orientation(), m_scale(vec3(1.0f)), m_invert(invert), isDirty(true)
{}

Transform::~Transform()
{}

void Transform::setPosition(const vec3 & position)
{
	m_position = position;
	isDirty = true;
}

vec3 Transform::getPosition()
{
	return m_position;
}


void Transform::setOrientation(const quat& orientation) {
	m_orientation = orientation;
	isDirty = true;
}
quat Transform::getOrientation() {
	return m_orientation;
}

void Transform::setScale(const vec3& scale) {
	m_scale = scale;
	isDirty = true;
}

vec3 Transform::getScale() {
	return m_scale;
}

vec3 Transform::translateInModel(const vec3& delta) {
	vec3 mDelta = vec3(glm::inverse(getMatrix()) * glm::vec4(delta, 0));
	return translateInWorld(mDelta);
}

vec3 Transform::translateInWorld(const vec3& delta) {
	m_position += delta;
	isDirty = true;
	return m_position;
}

quat Transform::rotate(const quat& delta) {
	m_orientation = m_orientation * delta;
	isDirty = true;
	return m_orientation;
}

mat4 Transform::getMatrix() {
	if (isDirty) {
		if ( m_invert)
			m_matrix = glm::inverse(glm::translate(m_position) * glm::mat4_cast(m_orientation)) * glm::scale(m_scale);
		else
			m_matrix = glm::translate(m_position) * glm::mat4_cast(m_orientation) * glm::scale(m_scale);
		isDirty = false;
	}
	return m_matrix;
}

