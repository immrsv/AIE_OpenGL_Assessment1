#pragma once


#include "Shader.h"
#include <Texture.h>

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	void Init(int width, int height, unsigned int count = 1);

	void Begin();
	void End();

	void SetViewport(int width, int height);
	void DrawToScreen();

	int m_viewport[4];
	float m_texelSize[2];

	unsigned int m_texCount;
	unsigned int* m_TexId;
	unsigned int m_DepthId;
	unsigned int m_FBO; // Frame Buffer
	unsigned int m_VAO; // Quad
	unsigned int m_VBO;
	Shader* m_shader; // Shader for Quad

	const int max_width = 1920;
	const int max_height = 1080;

private:
	int m_oldViewport[4];
	float m_oldClearColor[4];
	bool hasQuad = false;
	void buildQuad();

};

