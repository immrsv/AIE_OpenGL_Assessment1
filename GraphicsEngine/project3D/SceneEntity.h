#pragma once

#include "FbxModel.h"
#include "Mirror.h"
#include "Shader.h"
#include "Transform.h"
#include "BoundingBox.h"
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


	SceneEntity(Mirror* mirror, Shader* shader, float scaleFactor);
	SceneEntity(FbxModel* model, Shader* shader, float scaleFactor);

	FbxModel* m_model;
	Mirror* m_mirror;

	Shader* m_shader;
	
	BoundingBox* m_bounds = 0;
	
	Transform* GetTransform();

	void Update(float deltaTime);
	bool isOffScreen(glm::mat4& mvp);

	float m_timestep;
	float m_animSpeed;

	glm::vec3 drift = glm::vec3();
	glm::quat spin = glm::quat();

private:
	Transform* m_transform;
};

