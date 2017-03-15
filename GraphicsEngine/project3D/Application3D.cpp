#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <imgui.h>

#include "Shader.h"
#include "Gui.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;


Application3D::Application3D() {

}

Application3D::~Application3D() {

}

bool Application3D::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f, 0.f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);


	Scene::instance->Start();

	Scene::instance->m_camera.setProjection(glm::perspective(glm::pi<float>() * 0.25f,
		getWindowWidth() / (float)getWindowHeight(),
		0.1f, 1000.f));

	m_fbo.Init(1920, 1080);

	// Bind GUI Vars
	Gui::instance->pp_Enabled = &pp_Enabled;
	Gui::instance->pp_BlurSize = &pp_BlurSize;
	Gui::instance->pp_BloomSize = &pp_BloomSize;
	Gui::instance->pp_SobelWeight = &pp_SobelWeight;
	Gui::instance->m_fps = &m_fps;

	return true;
}

void Application3D::shutdown() {

	Gizmos::destroy();
}

// World State Update
void Application3D::update(float deltaTime) {

	aie::Input* input = aie::Input::getInstance();

	// query time since application started
	float time = getTime();


	Scene::instance->Update(deltaTime);

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	// demonstrate a few shapes
	Gizmos::addAABBFilled(vec3(0), vec3(1), vec4(0, 0.5f, 1, 0.25f));
	Gizmos::addSphere(vec3(5, 0, 5), 1, 8, 8, vec4(1, 0, 0, 0.5f));
	Gizmos::addRing(vec3(5, 0, -5), 1, 1.5f, 8, vec4(0, 1, 0, 1));
	Gizmos::addDisk(vec3(-5, 0, 5), 1, 16, vec4(1, 1, 0, 1));
	Gizmos::addArc(vec3(-5, 0, -5), 0, 2, 1, 8, vec4(1, 0, 1, 1));

	mat4 t = glm::rotate(time, glm::normalize(vec3(1, 1, 1)));
	t[3] = vec4(-2, 0, 0, 1);
	Gizmos::addCylinderFilled(vec3(0), 0.5f, 1, 5, vec4(0, 1, 1, 1), &t);


	// quit if we press escape
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}


// Graphics Update
void Application3D::draw() {

	// wipe the screen to the background colour
	clearScreen();


	Gizmos::draw(Scene::instance->m_camera.getPvMatrix());

	// Do Predraw (basically, update Mirrors)
	Scene::instance->Predraw();

	// Bind FBO for post-proc
	if (pp_Enabled) {
		m_fbo.SetViewport(getWindowWidth(), getWindowHeight());
		m_fbo.Begin();

		// wipe the screen to the background colour
		clearScreen();
	}

	// update perspective in case window resized

	Scene::instance->m_camera.setProjection(glm::perspective(glm::pi<float>() * 0.25f,
		getWindowWidth() / (float)getWindowHeight(),
		0.1f, 1000.f));

	// TODO: REMOVE DEBUG
	//Scene::Instance()->m_camera.setViewFor(vec3(0, -20, 0), glm::quat(vec3(glm::pi<float>() / 2.0f, 0, 0)));
	//Scene::Instance()->m_camera.setProjection(glm::perspective(glm::pi<float>() * 0.25f, 1.0f, 0.1f, 1000.f));

	Scene::instance->Draw();


	// Unbind post-proc FBO, draw to screen
	if (pp_Enabled) {
		m_fbo.End();
		m_fbo.m_shader->MakeActive();
		m_fbo.m_shader->SetInt("blurSize", pp_BlurSize);
		m_fbo.m_shader->SetInt("bloomSize", pp_BloomSize);
		m_fbo.m_shader->SetFloat("sobelWeight", pp_SobelWeight);

		m_fbo.DrawToScreen();
	}

	Gui::instance->draw();
}

