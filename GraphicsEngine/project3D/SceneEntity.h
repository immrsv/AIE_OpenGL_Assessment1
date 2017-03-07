#pragma once

#include "FbxModel.h"
#include "Mirror.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <Texture.h>

class SceneEntity
{
public:

	struct {
		glm::vec4 weights = glm::vec4(0); // Ambient, Diffuse, Specular, Normal

		aie::Texture* ambient = 0;
		aie::Texture* diffuse = 0;
		aie::Texture* specular = 0;
		aie::Texture* normal = 0;

		bool isEmissiveAmbient = false;
	} m_textures;

	SceneEntity(FbxModel* model, Shader* shader, float scaleFactor);

	FbxModel* m_model;
	Mirror* m_mirror;

	Shader* m_shader;

	void SetPosition(glm::vec3 pos);
	const glm::vec3& GetPosition();
	
	
	const glm::mat4& GetTransform();

	void Update(float deltaTime);
	

	void Predraw(Shader* shader = 0);
	void Draw();

	float m_timestep;
	float m_animSpeed;

	glm::vec3 drift = glm::vec3();
	glm::quat spin = glm::quat();

private:
	glm::vec3 m_position;
	glm::quat m_rotation;

	glm::mat4 m_transform;
	bool m_transformNeedsUpdate = true;

	float m_scaleFactor = 0.1f;

};

