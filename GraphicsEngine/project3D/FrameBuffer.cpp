#include "FrameBuffer.h"


#include <gl_core_4_4.h>
#include <iostream>

FrameBuffer::FrameBuffer()
{
}


FrameBuffer::~FrameBuffer()
{
}

void FrameBuffer::Init(int width, int height) {
	m_viewport[0] = m_viewport[1] = 0;
	m_viewport[2] = width;
	m_viewport[3] = height;

	m_shader = Shader::CompileShaders("FboQuad", "../Project3D/FboQuad.vert", "../Project3D/FboQuad.frag");
	
	GLenum err;
	err = glGetError();
	if (err != GL_NO_ERROR)
		std::cerr << "FrameBuffer::Init() - Pre Init | " << err << std::endl;

	// Create Framebuffer on Gfx
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	glGenTextures(1, &m_TexId);
	glBindTexture(GL_TEXTURE_2D, m_TexId);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 1920, 1080);

	// set some filtering parameters on the texture.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// attach this texture and its data to the current framebuffer

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_TexId, 0);

	// create a new depth buffer and set it as the current one
	glGenRenderbuffers(1, &m_DepthId);
	glBindRenderbuffer(GL_RENDERBUFFER, m_DepthId);

	// allocate memory on the GPU for the depth buffer
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1920, 1080);

	// attach this depth buffer to the current frame buffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthId);

	// test everything's set up correctly
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };

	glDrawBuffers(1, drawBuffers);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		printf("FrameBuffer::Init() - Framebuffer Error!\n");

	// we're done! detach the frame buffer, so the current one is once again the default screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	err = glGetError();
	if (err != GL_NO_ERROR)
		std::cerr << "FrameBuffer::Init() - Post Init | " << err << std::endl;
}

void FrameBuffer::Begin() {
	glGetIntegerv(GL_VIEWPORT, m_oldViewport); // Cache "normal" viewport
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
}

void FrameBuffer::End() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(m_oldViewport[0], m_oldViewport[1], m_oldViewport[2], m_oldViewport[3]);
}

void FrameBuffer::DrawToScreen() {
	if (!hasQuad) buildQuad();

	// draw out full-screen quad
	GLenum err;
	err = glGetError();
	if (err != GL_NO_ERROR)
		std::cerr << "FrameBuffer::DrawToScreen() - Pre Draw | " << err << std::endl;

	m_shader->MakeActive();
	m_shader->SetTexture("decal", 0, m_TexId);
	m_shader->SetVec2("texelSize", m_texelSize);

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	err = glGetError();
	if (err != GL_NO_ERROR)
		std::cerr << "FrameBuffer::DrawToScreen() - Post Draw | " << err << std::endl;
}

void FrameBuffer::buildQuad() {
	float halfTexel[] = { 0.5 / m_viewport[2], 0.5 / m_viewport[3] };
	float coverage[] = { m_viewport[2] / (float)max_width, m_viewport[3] / (float)max_height };
	float vertices[] =
	{
		-1, -1, 0, 1, halfTexel[0], halfTexel[1],
		1, 1, 0, 1, coverage[0] - halfTexel[0], coverage[1] - halfTexel[1],
		-1, 1, 0, 1, halfTexel[0], coverage[1] - halfTexel[1],
		-1, -1, 0, 1, halfTexel[0], halfTexel[1],
		1, -1, 0, 1, coverage[0] - halfTexel[0], halfTexel[1],
		1, 1, 0, 1, coverage[0] - halfTexel[0], coverage[1] - halfTexel[1],
	};

	m_texelSize[0] = halfTexel[0] * 2;
	m_texelSize[1] = halfTexel[1] * 2;

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6, vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // position
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

	glEnableVertexAttribArray(1); // UVs
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, ((char*)0) + 16);

	// Detach
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void FrameBuffer::SetViewport(int width, int height) {
	if (m_viewport[2] != width || m_viewport[3] != height) {

		m_viewport[0] = m_viewport[1] = 0;
		m_viewport[2] = width;
		m_viewport[3] = height;


		if (hasQuad) {
			float halfTexel[] = { 0.5 / m_viewport[2], 0.5 / m_viewport[3] };
			float coverage[] = { width / (float)max_width, height / (float)max_height };
			float vertices[] =
			{
				-1, -1, 0, 1, halfTexel[0], halfTexel[1],
				1, 1, 0, 1, coverage[0] - halfTexel[0], coverage[1] - halfTexel[1],
				-1, 1, 0, 1, halfTexel[0], coverage[1] - halfTexel[1],
				-1, -1, 0, 1, halfTexel[0], halfTexel[1],
				1, -1, 0, 1, coverage[0] - halfTexel[0], halfTexel[1],
				1, 1, 0, 1, coverage[0] - halfTexel[0], coverage[1] - halfTexel[1],
			};

			m_texelSize[0] = halfTexel[0] * 2;
			m_texelSize[1] = halfTexel[1] * 2;

			glBindVertexArray(m_VAO);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

			//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 6 * 6, vertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6, vertices, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0); // position
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

			glEnableVertexAttribArray(1); // UVs
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, ((char*)0) + 16);

			// Detach
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}
}