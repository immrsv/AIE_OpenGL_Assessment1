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

	bool usePostProc = true;

	FrameBuffer m_fbo;

};