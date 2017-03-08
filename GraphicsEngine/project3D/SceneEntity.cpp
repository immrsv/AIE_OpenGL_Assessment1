#include "SceneEntity.h"

#include <glm/ext.hpp>
#include <cmath>
#include <random>

SceneEntity::SceneEntity(FbxModel* model, Shader* shader, float scaleFactor)
	: m_model(model), m_mirror(0), m_shader(shader), m_timestep(0.0f), m_animSpeed(1.0f), m_transform(new Transform())
{
	m_transform->setScale(vec3(scaleFactor));
}

SceneEntity::SceneEntity(Mirror* mirror, Shader* shader, float scaleFactor)
	: m_model(0), m_mirror(mirror), m_shader(shader), m_timestep(0.0f), m_animSpeed(1.0f), m_transform(new Transform())
{
	m_transform->setScale(vec3(scaleFactor));
}

Transform* SceneEntity::GetTransform() {
	return m_transform;
}


void SceneEntity::Update(float deltaTime) {
	m_timestep += deltaTime * m_animSpeed;

	m_transform->translateInWorld(drift * deltaTime);
	m_transform->rotate(glm::slerp(glm::quat(), spin, deltaTime));
}

// Frustum Culling Check
bool SceneEntity::isOffScreen(glm::mat4& mvp) {
	return false;
}