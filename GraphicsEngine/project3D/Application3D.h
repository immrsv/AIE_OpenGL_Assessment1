#pragma once


#include <glm/mat4x4.hpp>

#include "Application.h"
#include <Input.h>
#include "Scene.h"
#include "FrameBuffer.h"

class Application3D : public aie::Application {
public:

	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	// Post Processing
	bool pp_Enabled = false;

	int pp_BlurSize = 0;
	int pp_BloomSize = 4;
	float pp_SobelWeight = 1.0f;

	FrameBuffer m_fbo;


};