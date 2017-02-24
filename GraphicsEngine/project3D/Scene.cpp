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

	m_ambientLight = vec3(0.05);

	// Load Shaders
	Shader::CompileShaders("DefaultShader", "../Project3D/Basic.vert", "../Project3D/Basic.frag");
	//Shader::CompileShaders("LightedShader", "../Project3D/Lighted.vert", "../Project3D/Lighted.frag");
	//Shader::CompileShaders("TexturedShader", "../Project3D/Textured.vert", "../Project3D/Textured.frag");
	//Shader::CompileShaders("MorphingShader", "../Project3D/Morphing.vert", "../Project3D/Morphing.frag");
	Shader::CompileShaders("RiggingShader", "../Project3D/Rigging.vert", "../Project3D/Rigging.frag");
	Shader::CompileShaders("NmappedRiggedBasic", "../Project3D/NmapRigged.vert", "../Project3D/Basic.frag");
	Shader::CompileShaders("NmappedRiggedPhong", "../Project3D/NmapRigged.vert", "../Project3D/PhongMaps.frag");

	// Load Textures
	CacheTexture("./textures/numbered_grid.tga");


	CacheTexture("./models/Pyro/Pyro_D.tga");
	CacheTexture("./models/Pyro/Pyro_N.tga");
	CacheTexture("./models/Pyro/Pyro_S.tga");

	CacheTexture("./models/DarkSiderGun/Gun_A.tga");
	CacheTexture("./models/DarkSiderGun/Gun_D.tga");
	CacheTexture("./models/DarkSiderGun/Gun_N_1.tga");
	CacheTexture("./models/DarkSiderGun/Gun_S.tga");

	// Load Models
	//CacheModel("./models/stanford/bunny.obj");
	CacheModel("./models/Pyro/pyro.fbx");
	CacheModel("./models/DarkSiderGun/GUN.fbx");


	// Make Entities
	SceneEntity* entity;

	//CreateEntity("./models/stanford/bunny.obj", "./textures/numbered_grid.tga", "DefaultShader", 1.0f);
	entity = CreateEntity("./models/Pyro/pyro.fbx", "./models/Pyro/Pyro_D.tga", "NmappedRiggedPhong", 0.005f);

	entity->m_textures.normal = _TextureCache["./models/Pyro/Pyro_N.tga"];
	entity->m_textures.specular = _TextureCache["./models/Pyro/Pyro_S.tga"];
	entity->m_textures.weights = vec4(0,1,1,0);
	entity->spin = glm::quat(glm::vec3(0, 0.5, 0));
	entity->SetPosition(vec3(5, 0, 5));

	entity = CreateEntity("./models/DarkSiderGun/GUN.fbx", "./models/DarkSiderGun/Gun_D.tga", "NmappedRiggedPhong", 0.2f);

	entity->m_textures.normal = _TextureCache["./models/DarkSiderGun/Gun_N_1.tga"];
	entity->m_textures.specular = _TextureCache["./models/DarkSiderGun/Gun_S.tga"];
	entity->m_textures.ambient = _TextureCache["./models/DarkSiderGun/Gun_A.tga"];
	entity->m_textures.weights = vec4(1,1,1,0);
	entity->spin = glm::quat(glm::vec3(1, 0, 0));
	entity->SetPosition(vec3(-4, 0, 0));


	// Setup Lighting

}


void Scene::Update(float deltaTime) {

	for (auto entity : _Entities) {
		entity->Update(deltaTime);
	}

	m_camera.update(deltaTime);
}

void Scene::Draw( ) {
	
	for (auto entity : _Entities) {


		// Bind Shader
		entity->m_shader->MakeActive();


		entity->m_shader->SetMat4("pvmMatrix", glm::value_ptr(m_camera.getTransform() * entity->GetTransform()));
		entity->m_shader->SetMat4("modelMatrix", glm::value_ptr(entity->GetTransform()));
		entity->m_shader->SetMat4("pvmMatrix", glm::value_ptr(m_camera.getTransform() * entity->GetTransform()));

		entity->m_shader->SetVec3("cameraPos", glm::value_ptr(m_camera.getPosition()));
		entity->m_shader->SetVec3("globalAmbient", glm::value_ptr(m_ambientLight));

		entity->m_shader->SetVec4("TexWeights", glm::value_ptr(entity->m_textures.weights));

		if (entity->m_textures.diffuse) entity->m_shader->SetTexture("diffuseTex", 0, entity->m_textures.diffuse->getHandle());
		if (entity->m_textures.specular) entity->m_shader->SetTexture("specularTex", 1, entity->m_textures.specular->getHandle());
		if (entity->m_textures.normal) entity->m_shader->SetTexture("normalTex", 2, entity->m_textures.normal->getHandle());
		if (entity->m_textures.ambient) entity->m_shader->SetTexture("ambientTex", 3, entity->m_textures.ambient->getHandle());


		auto skelCount = entity->m_model->m_fbx->getSkeletonCount();

		entity->m_shader->SetBool("hasBones", skelCount > 0);

		for (int i = 0; i < skelCount; ++i) {
			FBXSkeleton* skel = entity->m_model->m_fbx->getSkeletonByIndex(i);
			FBXAnimation* anim = entity->m_model->m_fbx->getAnimationByIndex(0);

			float timestep = fmodf(entity->m_timestep, anim->totalTime());

			skel->evaluate(anim, timestep);

			for (unsigned int bone_index = 0; bone_index < skel->m_boneCount; ++bone_index)
			{
				skel->m_nodes[bone_index]->updateGlobalTransform();
			}

			// Force ONE skeleton
			//break;
		}

		for (unsigned int i = 0; i < skelCount; ++i ) {
			FBXSkeleton* skeleton = entity->m_model->m_fbx->getSkeletonByIndex(i);
			skeleton->updateBones();

			entity->m_shader->SetMat4Array("bones", skeleton->m_boneCount, (float*)skeleton->m_bones);

			// Force ONE skeleton
			//break;
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