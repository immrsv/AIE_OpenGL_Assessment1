#pragma once


#include "Shader.h"
#include <Texture.h>

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	void init(int width, int height, unsigned int count = 1);

	void begin();
	void end();

	void setViewport(int width, int height);
	void drawToScreen();

	int m_viewport[4];
	int m_textureSize[2];
	float m_texelSize[2];

	unsigned int m_texCount;
	unsigned int* m_TexId;
	unsigned int m_DepthId;
	unsigned int m_FBO; // Frame Buffer
	unsigned int m_VAO; // Quad
	unsigned int m_VBO;
	Shader* m_shader; // Shader for Quad

private:
	int m_oldViewport[4];
	float m_oldClearColor[4];
	bool hasQuad = false;
	void buildQuad();

};

