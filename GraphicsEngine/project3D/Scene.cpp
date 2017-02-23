#include "Scene.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <gl_core_4_4.h>
#include <FBXFile.h>


Scene* Scene::_instance = 0;
map<string, aie::Texture*> Scene::_TextureCache = map<string, aie::Texture*>();
map<string, Model*> Scene::_ModelCache = map<string, Model*>();


Scene* Scene::Instance() {
	if (_instance == 0)
		_instance = new Scene();

	return _instance;
}
Scene::Scene()
	: m_camera()
{
}


Scene::~Scene()
{
}


void Scene::Start() {

	m_camera.setViewFor(vec3(0, 10, -10), 90.f, -45.f);

	// Load Shaders
	Shader::CompileShaders("DefaultShader", "../Project3D/Basic.vert", "../Project3D/Basic.frag");
	//Shader::CompileShaders("LightedShader", "../Project3D/Lighted.vert", "../Project3D/Lighted.frag");
	//Shader::CompileShaders("TexturedShader", "../Project3D/Textured.vert", "../Project3D/Textured.frag");
	//Shader::CompileShaders("MorphingShader", "../Project3D/Morphing.vert", "../Project3D/Morphing.frag");
	Shader::CompileShaders("RiggingShader", "../Project3D/Rigging.vert", "../Project3D/Rigging.frag");

	// Load Textures
	CacheTexture("./textures/numbered_grid.tga");

	// Load Models
	CacheModel("./models/stanford/bunny.obj");
	CacheModel("./models/Pyro/pyro.fbx");


	// Make Entities
	//CreateEntity("./models/stanford/bunny.obj", "./textures/numbered_grid.tga", "DefaultShader", 1.0f);
	CreateEntity("./models/Pyro/pyro.fbx", "./textures/numbered_grid.tga", "RiggingShader", 0.01f);

	// Setup Lighting

}


void Scene::Update(float deltaTime) {

	for (auto entity : _Entities) {
		entity->Update(deltaTime);
	}

	m_camera.update(deltaTime);
}

void Scene::Draw() {
	
	for (auto entity : _Entities) {


		// Bind Shader
		entity->m_shader->MakeActive();

		entity->m_shader->SetMat4("pvmMatrix", glm::value_ptr(m_camera.getTransform() * entity->GetTransform()));
		entity->m_shader->SetMat4("modelMatrix", glm::value_ptr(entity->GetTransform()));
		entity->m_shader->SetMat4("pvmMatrix", glm::value_ptr(m_camera.getTransform() * entity->GetTransform()));

		entity->m_shader->SetVec3("cameraPos", glm::value_ptr(m_camera.getPosition()));

		entity->m_shader->SetTexture("diffuseTex", 0, entity->m_textures.diffuse->getHandle());

		auto skelCount = entity->m_model->m_fbx->getSkeletonCount();
		for (unsigned int i = 0; i < skelCount; ++i ) {
			FBXSkeleton* skeleton = entity->m_model->m_fbx->getSkeletonByIndex(i);
			skeleton->updateBones();

			entity->m_shader->SetMat4Array("bones", skeleton->m_boneCount, (float*)skeleton->m_bones);

			// Force ONE skeleton
			break;
		}
		

		// Draw Model
		entity->m_model->draw();
	}
}

void Scene::CacheModel(string filename) {

	std::cout << "Loading Model: " << filename << std::flush;

	std::map<std::string, Model*>::iterator iter = _ModelCache.find(filename);
	if (iter != _ModelCache.end()) {

		std::cout << " - Already Cached!" << std::endl;
		return;
	}

	auto model = new Model(filename);
	std::cout << " - Done!" << std::endl;

	_ModelCache[filename] = model;
}

void Scene::CacheTexture(string filename) {
	std::cout << "Loading Texture File: " << filename << std::flush;

	std::map<std::string, aie::Texture*>::iterator iter = _TextureCache.find(filename);
	if (iter != _TextureCache.end()) {

		std::cout << " - Already Cached!" << std::endl;
		return;
	}

	auto tex = new aie::Texture(filename.c_str());

	std::cout << " - Done!" << std::endl;

	_TextureCache[filename] = tex;
}

void Scene::CacheShader(string pseudonym, string vertexFilename, string fragmentFilename) {
	// Shader class handles own caching
	Shader::CompileShaders(pseudonym, vertexFilename, fragmentFilename);
}

SceneEntity* Scene::CreateEntity(string model, string texture, string shader, float scaleFactor) {
	auto entity = new SceneEntity(_ModelCache[model], _TextureCache[texture], Shader::GetShader(shader), scaleFactor);
	
	_Entities.push_back(entity);
	return entity;
}