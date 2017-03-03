#pragma once

#include "FrameBuffer.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include "Transform.h"

using glm::vec2;
using glm::vec3;
using glm::mat4;

class Mirror
{
public:
	Mirror(vec3 position, glm::quat orientation, vec2 size, float quality = 1.0f);
	~Mirror();

	FrameBuffer m_buffer;

	Transform m_transform;

	vec2 m_size;

	float m_reflectivity;

	Camera reflect(Camera& view);
	void Init();
	void Begin();
	void End();

	void Draw();

private:
	const int width = 512;
	const int height = 512;

	unsigned int m_VAO;
	unsigned int m_VBO;
	float m_texelSize[2];

	void buildQuad();
};

