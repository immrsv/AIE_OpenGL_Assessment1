#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Transform.h"

using glm::mat4;
using glm::vec3;
using glm::vec2;

class Camera
{
public:
	Camera();
	~Camera();

	
	float fov;

	virtual void update(float deltaTime);

	//void lookAt(vec3 from, vec3 to, vec3 up);
	void rotate(glm::quat delta);
	void translate(vec3 delta);
	void translateWorld(vec3 delta);

	void setViewFor(vec3 position, glm::quat orientation);
	void setOrientation(glm::quat orientation);
	glm::quat getOrientation() { return m_view.getOrientation(); }
	void setPosition(vec3 position);
	vec3 getPosition();

	void setProjection(mat4 projection);
	Transform& getView();
	mat4 getProjection();
	mat4 getPvMatrix();
	
private:
	bool m_isDirty;
	
	Transform m_view;
	mat4 m_projection;

	mat4 m_projectedView;
};

