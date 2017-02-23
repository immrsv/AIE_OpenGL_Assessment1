#pragma once

#include "Model.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <Texture.h>

class SceneEntity
{
public:

	struct {
		aie::Texture* diffuse;
		aie::Texture* normal;
		aie::Texture* specular;
		aie::Texture* gloss;
	} m_textures;

	SceneEntity(Model* model, aie::Texture* texture, Shader* shader, float scaleFactor);

	Model* m_model;
	Shader* m_shader;

	void SetPosition(glm::vec3 pos);
	const glm::vec3& GetPosition();
	
	
	const glm::mat4& GetTransform();

	void Update(float deltaTime);

private:
	glm::vec3 m_position;
	glm::quat m_rotation;

	glm::mat4 m_transform;
	bool m_transformNeedsUpdate = true;

	float m_scaleFactor = 0.1f;

	float m_timestep;
};

