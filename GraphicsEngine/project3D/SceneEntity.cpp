#include "SceneEntity.h"

#include <glm/ext.hpp>
#include <cmath>
#include <random>

SceneEntity::SceneEntity(FbxModel* model, Shader* shader, float scaleFactor)
	: m_model(model), m_shader(shader), m_scaleFactor(scaleFactor), m_timestep(0.0f), m_animSpeed(1.0f)
{
}


const glm::vec3& SceneEntity::GetPosition() { 
	return m_position;  
}

void SceneEntity::SetPosition(glm::vec3 position) {
	m_position = position;
	m_transformNeedsUpdate = true;
}

const glm::mat4& SceneEntity::GetTransform() {
	if (m_transformNeedsUpdate) {
		glm::mat4 transform = glm::mat4();
		transform = glm::translate(transform, m_position);
		transform = transform * glm::mat4_cast(m_rotation);
		transform = glm::scale(transform, glm::vec3(m_scaleFactor));

		m_transform = transform;
		m_transformNeedsUpdate = false;
	}

	return m_transform;
}


void SceneEntity::Update(float deltaTime) {
	m_timestep += deltaTime * m_animSpeed;

	m_position += (drift * deltaTime);
	m_rotation = m_rotation * glm::slerp(glm::quat(), spin, deltaTime);

	m_transformNeedsUpdate = true;	
}


void SceneEntity::Predraw(Shader* shader) {

	if (shader == 0) shader = m_shader;


}