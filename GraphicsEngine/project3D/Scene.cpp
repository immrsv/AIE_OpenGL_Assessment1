#include "Scene.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <gl_core_4_4.h>
#include <FBXFile.h>


Scene* Scene::_instance = 0;
map<string, aie::Texture*> Scene::_TextureCache = map<string, aie::Texture*>();
map<string, FbxModel*> Scene::_ModelCache = map<string, FbxModel*>();


Scene* Scene::Instance() {
	if (_instance == 0)
		_instance = new Scene();

	return _instance;
}
Scene::Scene()
	: m_camera()
{
	for (int i = 0; i < MAX_LIGHTS; i++) {
		pointLtPwr[i] = -1;
	}
}


Scene::~Scene()
{
}


void Scene::Start() {

	//m_camera.setViewFor(vec3(0, 10, -10), 90.f, -45.f);
	//m_camera.setViewFor(vec3(0, 0,0), glm::quat(vec3(3.14/4.0f, 3.14/2.0f,0 )));
	m_camera.setViewFor(vec3(0, 10, -15), glm::quat(vec3(-glm::pi<float>() / 5.0f, glm::pi<float>(),0)));

	m_ambientLight = vec3(0.05f);

	// Load Shaders
	Shader::CompileShaders("DefaultShader", "../Project3D/Basic.vert", "../Project3D/Basic.frag");
	//Shader::CompileShaders("LightedShader", "../Project3D/Lighted.vert", "../Project3D/Lighted.frag");
	//Shader::CompileShaders("TexturedShader", "../Project3D/Textured.vert", "../Project3D/Textured.frag");
	//Shader::CompileShaders("MorphingShader", "../Project3D/Morphing.vert", "../Project3D/Morphing.frag");
	Shader::CompileShaders("RiggingShader", "../Project3D/Rigging.vert", "../Project3D/Rigging.frag");
	Shader::CompileShaders("NmappedRiggedBasic", "../Project3D/NmapRigged.vert", "../Project3D/Basic.frag");
	Shader::CompileShaders("NmappedRiggedPhong", "../Project3D/NmapRigged.vert", "../Project3D/PhongMaps.frag");

	// Make Entities
	SceneEntity* entity;

	entity = CreateEntity(CachedModel("./models/Pyro/pyro.fbx"), Shader::GetShader("NmappedRiggedPhong"), 0.005f);

	entity->m_textures.diffuse = CachedTexture("./models/Pyro/Pyro_D.tga");
	entity->m_textures.normal = CachedTexture("./models/Pyro/Pyro_N.tga");
	entity->m_textures.specular = CachedTexture("./models/Pyro/Pyro_S.tga");
	entity->m_textures.weights = vec4(0, 1, 1, 0);
	entity->spin = glm::quat(glm::vec3(0, 0.5, 0));
	entity->SetPosition(vec3(5, 0, 5));

	entity = CreateEntity(CachedModel("./models/Pyro/pyro.fbx"), Shader::GetShader("NmappedRiggedPhong"), 0.005f);

	entity->m_textures.diffuse = CachedTexture("./models/Pyro/Pyro_D.tga");
	entity->m_textures.normal = CachedTexture("./models/Pyro/Pyro_N.tga");
	entity->m_textures.specular = CachedTexture("./models/Pyro/Pyro_S.tga");
	entity->m_textures.weights = vec4(0, 1, 1, 0);
	entity->spin = glm::quat(glm::vec3(0, 0.5, 0));
	entity->SetPosition(vec3(-5, 0, 5));
	entity->m_animSpeed = 0.5f;

	entity = CreateEntity(CachedModel("./models/Pyro/pyro.fbx"), Shader::GetShader("NmappedRiggedPhong"), 0.005f);

	entity->m_textures.diffuse = CachedTexture("./models/Pyro/Pyro_D.tga");
	entity->m_textures.normal = CachedTexture("./models/Pyro/Pyro_N.tga");
	entity->m_textures.specular = CachedTexture("./models/Pyro/Pyro_S.tga");
	entity->m_textures.weights = vec4(0, 1, 1, 0);
	entity->spin = glm::quat(glm::vec3(0, 0.5, 0));
	entity->SetPosition(vec3(-5, 0, -5));

	entity = CreateEntity(CachedModel("./models/Pyro/pyro.fbx"), Shader::GetShader("NmappedRiggedPhong"), 0.005f);

	entity->m_textures.diffuse = CachedTexture("./models/Pyro/Pyro_D.tga");
	entity->m_textures.normal = CachedTexture("./models/Pyro/Pyro_N.tga");
	entity->m_textures.specular = CachedTexture("./models/Pyro/Pyro_S.tga");
	entity->m_textures.weights = vec4(0, 1, 1, 0);
	entity->spin = glm::quat(glm::vec3(0, 0.5, 0));
	entity->SetPosition(vec3(5, 0, -5));
	entity->m_animSpeed = 1.4f;




	entity = CreateEntity(CachedModel("./models/DarkSiderGun/GUN.fbx"), Shader::GetShader("NmappedRiggedPhong"), 0.2f);

	entity->m_textures.diffuse = CachedTexture("./models/DarkSiderGun/Gun_D.tga"); 
	entity->m_textures.normal = CachedTexture("./models/DarkSiderGun/Gun_N_1.tga");
	entity->m_textures.specular = CachedTexture("./models/DarkSiderGun/Gun_S.tga"); 
	entity->m_textures.ambient = CachedTexture("./models/DarkSiderGun/Gun_A.tga"); 
	entity->m_textures.weights = vec4(1,1,1,0);
	entity->spin = glm::quat(glm::vec3(0.5, 0, 0));
	entity->SetPosition(vec3(4, 0, 4));

	entity = CreateEntity(CachedModel("./models/DarkSiderGun/GUN.fbx"), Shader::GetShader("NmappedRiggedPhong"), 0.2f);

	entity->m_textures.diffuse = CachedTexture("./models/DarkSiderGun/Gun_D.tga");
	entity->m_textures.normal = CachedTexture("./models/DarkSiderGun/Gun_N_1.tga");
	entity->m_textures.specular = CachedTexture("./models/DarkSiderGun/Gun_S.tga");
	entity->m_textures.ambient = CachedTexture("./models/DarkSiderGun/Gun_A.tga");
	entity->m_textures.weights = vec4(1, 1, 1, 0);
	entity->spin = glm::quat(glm::vec3(1, 0, 1));
	entity->SetPosition(vec3(-4, 0, 0));

	entity = CreateEntity(CachedModel("./models/DarkSiderGun/GUN.fbx"), Shader::GetShader("NmappedRiggedPhong"), 0.2f);

	entity->m_textures.diffuse = CachedTexture("./models/DarkSiderGun/Gun_D.tga");
	entity->m_textures.normal = CachedTexture("./models/DarkSiderGun/Gun_N_1.tga");
	entity->m_textures.specular = CachedTexture("./models/DarkSiderGun/Gun_S.tga");
	entity->m_textures.ambient = CachedTexture("./models/DarkSiderGun/Gun_A.tga");
	entity->m_textures.weights = vec4(1, 1, 1, 0);
	entity->spin = glm::quat(glm::vec3(1, 0, 0));
	entity->SetPosition(vec3(-4, 0, 4));

	entity = CreateEntity(CachedModel("./models/DarkSiderGun/GUN.fbx"), Shader::GetShader("NmappedRiggedPhong"), 0.2f);

	entity->m_textures.diffuse = CachedTexture("./models/DarkSiderGun/Gun_D.tga");
	entity->m_textures.normal = CachedTexture("./models/DarkSiderGun/Gun_N_1.tga");
	entity->m_textures.specular = CachedTexture("./models/DarkSiderGun/Gun_S.tga");
	entity->m_textures.ambient = CachedTexture("./models/DarkSiderGun/Gun_A.tga");
	entity->m_textures.weights = vec4(1, 1, 1, 0);
	entity->spin = glm::quat(glm::vec3(0, 0.7, 0));
	entity->SetPosition(vec3(-4, 0, -4));



	// Setup Lighting

	m_directLightDir = glm::normalize( vec3(-1, -1, 1) );
	m_directLightClr = vec3(0.5);

	AddLight(Light(0, vec3(0, 5, 0), vec3(0.7f), vec3(1), 10));
	AddLight(Light(0, vec3(0, 5, -10), vec3(0, 0.6f, 0.6f), vec3(1), 50));
	AddLight(Light(0, vec3(10, 5, 0), vec3(1, 0, 1), vec3(1), 50));




}


void Scene::Update(float deltaTime) {

	for (auto entity : _Entities) {
		entity->Update(deltaTime);
	}

	m_camera.update(deltaTime);
}

void Scene::Draw( SceneEntity* mirrorEntity) {
	
	for (auto entity : _Entities) {

		if (entity == mirrorEntity) continue; // Check we're not rendering the current mirror
		
		if (entity->m_mirror != 0) { // This is a mirror
			if (mirrorEntity == 0) {
				// We're in main draw and this is a mirror, let's update the reflection.
				entity->m_mirror->Begin();
				this->Draw(entity);
				entity->m_mirror->End();
			}
		}
		
		// Bind Shader
		entity->m_shader->MakeActive();


		entity->m_shader->SetVec3("directLightDir", glm::value_ptr(glm::normalize(m_directLightDir)));
		entity->m_shader->SetVec3("directLightClr", glm::value_ptr(m_directLightClr));

		entity->m_shader->SetVec3Array("pointLtPos", MAX_LIGHTS, (float*)pointLtPos);
		entity->m_shader->SetVec3Array("pointLtClr", MAX_LIGHTS, (float*)pointLtClr);
		entity->m_shader->SetVec3Array("pointLtCoeff", MAX_LIGHTS, (float*)pointLtCoeff);
		entity->m_shader->SetFloatArray("pointLtPwr", MAX_LIGHTS, (float*)pointLtPwr);

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

		for (unsigned int i = 0; i < skelCount; ++i) {
			FBXSkeleton* skel = entity->m_model->m_fbx->getSkeletonByIndex(i);
			FBXAnimation* anim = entity->m_model->m_fbx->getAnimationByIndex(0);

			float timestep = fmodf(entity->m_timestep, anim->totalTime());

			skel->evaluate(anim, timestep);

			for (unsigned int bone_index = 0; bone_index < skel->m_boneCount; ++bone_index)
			{
				skel->m_nodes[bone_index]->updateGlobalTransform();
			}

			// Force ONE skeleton
			// break;
		}

		for (unsigned int i = 0; i < skelCount; ++i) {
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

FbxModel* Scene::CachedModel(string filename) {

	std::cout << "Loading Model: " << filename << std::flush;

	std::map<std::string, FbxModel*>::iterator iter = _ModelCache.find(filename);
	if (iter != _ModelCache.end()) {

		std::cout << " - Already Cached!" << std::endl;
		return iter->second;
	}

	auto model = new FbxModel(filename);
	std::cout << " - Done!" << std::endl;

	_ModelCache[filename] = model;
	return model;
}

aie::Texture* Scene::CachedTexture(string filename) {
	std::cout << "Loading Texture File: " << filename << std::flush;

	std::map<std::string, aie::Texture*>::iterator iter = _TextureCache.find(filename);
	if (iter != _TextureCache.end()) {

		std::cout << " - Already Cached!" << std::endl;
		return iter->second;
	}

	auto tex = new aie::Texture(filename.c_str());

	std::cout << " - Done!" << std::endl;

	_TextureCache[filename] = tex;
	return tex;
}

Shader* Scene::CachedShader(string pseudonym, string vertexFilename, string fragmentFilename) {
	// Shader class handles own caching
	Shader::CompileShaders(pseudonym, vertexFilename, fragmentFilename);
	return Shader::GetShader(pseudonym);
}

SceneEntity* Scene::CreateEntity(FbxModel* model, Shader* shader, float scaleFactor) {
	auto entity = new SceneEntity(model, shader, scaleFactor);
	
	_Entities.push_back(entity);
	return entity;
}

int Scene::AddLight(Light& light) {

	
	for (int i = 0; i < MAX_LIGHTS; i++) {
		if (pointLtPwr[i] < 0.0f) {
			pointLtPos[i] = light.Position;
			pointLtClr[i] = light.Color;
			pointLtCoeff[i] = light.Coeff;
			pointLtPwr[i] = light.Power;
			std::cout << "Added Light: [" << light.Color.r << "," << light.Color.g << "," << light.Color.b << "] at [" << light.Position.x << "," << light.Position.y << "," << light.Position.z << "]" << std::endl;
			std::cout << "        -> : [" << pointLtClr[i].r << "," << pointLtClr[i].g << "," << pointLtClr[i].b << "] at [" << pointLtPos[i].x << "," << pointLtPos[i].y << "," << pointLtPos[i].z << "]" << std::endl;
			return i;
		}
	}

	return -1;
}

Light Scene::GetLight(int idx) {
	if (pointLtPwr[idx] < 0.0f)
		return Light(idx, vec3(0), vec3(0), vec3(0), 0.f);
	else
		return Light(idx, pointLtPos[idx], pointLtClr[idx], pointLtCoeff[idx], pointLtPwr[idx]);
}

bool Scene::RemoveLight(int idx) {
	if (pointLtPwr[idx] < 0.0f)
		return false;

	pointLtPwr[idx] = -1;
	return true;
}
