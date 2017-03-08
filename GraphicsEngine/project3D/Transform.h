#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>


using namespace glm;

class Transform
{
public:
	static const vec3 vUp;
	static const vec3 vRight;
	static const vec3 vOut;

	Transform(const vec3& position = vec3(), const quat& orientation = quat(), const vec3& scale = vec3(1.0f));
	Transform(const bool invert);
	~Transform();

	void setPosition(const vec3& position);
	vec3 getPosition();

	void setOrientation(const quat& orientation);
	quat getOrientation();

	void setScale(const vec3& scale);
	vec3 getScale();

	vec3 translateInModel(const vec3& delta);
	vec3 translateInWorld(const vec3& delta);
	quat rotate(const quat& delta);

	mat4 getMatrix();

	

private:
	bool m_invert;
	vec3 m_scale;
	vec3 m_position;
	quat m_orientation;

	mat4 m_matrix;

	bool isDirty;
};

