#pragma once

#include "FrameBuffer.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include "Transform.h"
#include "Shader.h"

using glm::vec2;
using glm::vec3;
using glm::mat4;

class Mirror
{
public:
	Mirror(vec2 size); // X/Y Plane centered on Origin.
	~Mirror();

	FrameBuffer m_buffer;
	Camera m_camera;
	//Shader* m_shader;

	vec2 m_size;


	Camera& reflect(Transform* mirror, Transform* camera);
	void Init();

	void Begin();
	void End();

	void draw();

private:
	const int width = 512;
	const int height = 512;

	unsigned int m_VAO;
	unsigned int m_VBO;
	float m_texelSize[2];

	void buildQuad();
};

