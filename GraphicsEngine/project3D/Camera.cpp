#include "Camera.h"
#include "Input.h"
#include <iostream>

const float deg2rad = glm::pi<float>() / 180.f;

Camera::Camera() : 
	isDirty(true), 
	m_projection(), 
	m_view(true)
{
}


Camera::~Camera()
{
}

void Camera::update(float deltaTime) {

	//return;

	static aie::Input* input = aie::Input::getInstance();

	// Capture Motion Input
	vec3 move = vec3(0.f);
	float scaleTranslation = 10.0;

	if (input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT))
		scaleTranslation *= 0.5;

	if (input->isKeyDown(aie::INPUT_KEY_W))
		move += vec3(0, 0, -1) * deltaTime * scaleTranslation; // Forward
	if (input->isKeyDown(aie::INPUT_KEY_S))
		move += vec3(0, 0, 1) * deltaTime * scaleTranslation;  // Back
	if (input->isKeyDown(aie::INPUT_KEY_A))
		move += vec3(-1, 0, 0) * deltaTime * scaleTranslation; // Left
	if (input->isKeyDown(aie::INPUT_KEY_D))
		move += vec3(1, 0, 0) * deltaTime * scaleTranslation; // Right
	if (input->isKeyDown(aie::INPUT_KEY_SPACE))
		move += vec3(0, 1, 0) * deltaTime * scaleTranslation; // Up
	if (input->isKeyDown(aie::INPUT_KEY_LEFT_CONTROL))
		move += vec3(0, -1, 0) * deltaTime * scaleTranslation;  // Down
	if (input->isKeyDown(aie::INPUT_KEY_Q))
		rotate(glm::quat(vec3(0,0,0.02f)));  // Roll CCW
	if (input->isKeyDown(aie::INPUT_KEY_E))
		rotate(glm::quat(vec3(0,0,-0.02f)));  // Roll CW


	if (move.length() > FLT_EPSILON) { 
		translate(move);
	}

	static vec2 lastMouse;
	static bool isMouseTracking = false;

	vec2 mouse = vec2(input->getMouseX(), input->getMouseY());

	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT)) {
		
		const float scaleRotation = 0.02f;
		vec2 deltaMouse = (mouse - lastMouse) * scaleRotation;

		vec3 rotParams = vec3(deltaMouse.y, -deltaMouse.x, 0);
		rotate(glm::quat(rotParams));
	}
	lastMouse = mouse;
}

void Camera::rotate(glm::quat delta) {
	m_view.rotate(delta);
	isDirty = true;
}

void Camera::translate(vec3 delta)
{
	m_view.translateInModel(delta);
	isDirty = true;
}

void Camera::translateWorld(vec3 delta)
{
	m_view.translateInWorld(delta);
	isDirty = true;
}

void Camera::setViewFor(vec3 position, glm::quat orientation) {
	setPosition(position);
	setOrientation(orientation);
}

void Camera::setOrientation(glm::quat orientation) {
	m_view.setOrientation(orientation);
	isDirty = true;
}

void Camera::setPosition(vec3 position) {
	m_view.setPosition(position);
	isDirty = true;
}
vec3 Camera::getPosition()
{
	return m_view.getPosition();
}

Transform& Camera::getView() {
	return m_view;
}

mat4 Camera::getProjection() {
	return m_projection;
}

void Camera::setProjection(mat4 projection)
{
	m_projection = projection;
	isDirty = true;
}

mat4 Camera::getPvMatrix()
{
	if (isDirty) {
		m_projectedView = getProjection() * getView().getMatrix();
		isDirty = false;
	}
	
	return m_projectedView;
}
