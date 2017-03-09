#include "FrameBuffer.h"


#include <gl_core_4_4.h>
#include <iostream>

FrameBuffer::FrameBuffer()
{
}


FrameBuffer::~FrameBuffer()
{
	glDeleteRenderbuffers(1, &m_DepthId);
	glDeleteTextures(m_texCount, m_TexId);
	glDeleteFramebuffers(1, &m_FBO);

	if (hasQuad) {
		glDeleteBuffers(1, &m_VBO);
		glDeleteVertexArrays(1, &m_VAO);
	}
}

void FrameBuffer::Init(int width, int height, unsigned int count) {
	if (count < 1) throw "FrameBuffer::Init() - Argument Exception, count < 1";


	m_texCount = count;
	m_TexId = new unsigned int[m_texCount];

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

	// Generate Textures
	glGenTextures(m_texCount, m_TexId);
	GLenum* drawBuffers = new GLenum[m_texCount];

	for (int i = 0; i < m_texCount; i++) {
		
		glBindTexture(GL_TEXTURE_2D, m_TexId[i]);

		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 1920, 1080);

		// set some filtering parameters on the texture.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

		// attach this texture and its data to the current framebuffer
		drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
		glFramebufferTexture(GL_FRAMEBUFFER, drawBuffers[i], m_TexId[i], 0);

		
	}

	// create a new depth buffer and set it as the current one
	glGenRenderbuffers(1, &m_DepthId);
	glBindRenderbuffer(GL_RENDERBUFFER, m_DepthId);

	// allocate memory on the GPU for the depth buffer
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1920, 1080);

	// attach this depth buffer to the current frame buffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthId);

	// test everything's set up correctly
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
	glGetFloatv(GL_COLOR_CLEAR_VALUE, m_oldClearColor); // Cache clear color
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
}

void FrameBuffer::End() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(m_oldClearColor[0], m_oldClearColor[1], m_oldClearColor[2], m_oldClearColor[3]);
	glViewport(m_oldViewport[0], m_oldViewport[1], m_oldViewport[2], m_oldViewport[3]); // Restore "normal" viewport
}

void FrameBuffer::DrawToScreen() {
	if (!hasQuad) buildQuad();

	// draw out full-screen quad
	GLenum err;
	err = glGetError();
	if (err != GL_NO_ERROR)
		std::cerr << "FrameBuffer::DrawToScreen() - Pre Draw | " << err << std::endl;

	m_shader->MakeActive();
	m_shader->SetTexture("decal", 0, m_TexId[0]);
	m_shader->SetVec2("texelSize", m_texelSize);

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	err = glGetError();
	if (err != GL_NO_ERROR)
		std::cerr << "FrameBuffer::DrawToScreen() - Post Draw | " << err << std::endl;
}

void FrameBuffer::buildQuad() {
	float halfTexel[] = { 0.5f / m_viewport[2], 0.5f / m_viewport[3] };
	float coverage[] = { m_viewport[2] / (float)max_width, m_viewport[3] / (float)max_height };
	float vertices[] =
	{
		-1, -1, 0, 1, halfTexel[0], halfTexel[1],
		1, -1, 0, 1, coverage[0] - halfTexel[0], halfTexel[1],
		1, 1, 0, 1, coverage[0] - halfTexel[0], coverage[1] - halfTexel[1],
		-1, 1, 0, 1, halfTexel[0], coverage[1] - halfTexel[1],
	};

	m_texelSize[0] = halfTexel[0] * 2;
	m_texelSize[1] = halfTexel[1] * 2;

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // position
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

	glEnableVertexAttribArray(1); // UVs
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, ((char*)0) + 16);

	// Detach
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	hasQuad = true;
}

void FrameBuffer::SetViewport(int width, int height) {
	if (m_viewport[2] != width || m_viewport[3] != height) {

		m_viewport[0] = m_viewport[1] = 0;
		m_viewport[2] = width;
		m_viewport[3] = height;


		if (hasQuad) {
			float halfTexel[] = { 0.5f / m_viewport[2], 0.5f / m_viewport[3] };
			float coverage[] = { m_viewport[2] / (float)max_width, m_viewport[3] / (float)max_height };
			float vertices[] =
			{
				-1, -1, 0, 1, halfTexel[0], halfTexel[1],
				1, -1, 0, 1, coverage[0] - halfTexel[0], halfTexel[1],
				1, 1, 0, 1, coverage[0] - halfTexel[0], coverage[1] - halfTexel[1],
				-1, 1, 0, 1, halfTexel[0], coverage[1] - halfTexel[1],
			};

			m_texelSize[0] = halfTexel[0] * 2;
			m_texelSize[1] = halfTexel[1] * 2;

			glBindVertexArray(m_VAO);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

			//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 6 * 6, vertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, vertices, GL_STATIC_DRAW);

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