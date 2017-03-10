#include "Scene.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <gl_core_4_4.h>
#include <FBXFile.h>

map<string, aie::Texture*> Scene::_TextureCache = map<string, aie::Texture*>();
map<string, FbxModel*> Scene::_ModelCache = map<string, FbxModel*>();

Scene *const Scene::instance = new Scene();

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

	// Load Shaders
	//Shader::CompileShaders("DefaultShader", "../Project3D/Basic.vert", "../Project3D/Basic.frag");
	//Shader::CompileShaders("LightedShader", "../Project3D/Lighted.vert", "../Project3D/Lighted.frag");
	//Shader::CompileShaders("TexturedShader", "../Project3D/Textured.vert", "../Project3D/Textured.frag");
	//Shader::CompileShaders("MorphingShader", "../Project3D/Morphing.vert", "../Project3D/Morphing.frag");
	//Shader::CompileShaders("RiggingShader", "../Project3D/Rigging.vert", "../Project3D/Rigging.frag");
	//Shader::CompileShaders("NmappedRiggedBasic", "../Project3D/NmapRigged.vert", "../Project3D/Basic.frag");
	Shader::CompileShaders("NmappedRiggedPhong", "../Project3D/NmapRigged.vert", "../Project3D/PhongMaps.frag");
	Shader::CompileShaders("BasicDecal", "../Project3D/Basic.vert", "../Project3D/Decal.frag");

	m_boundsShader = Shader::GetShader("BasicDecal");

	// Define start camera
	//m_camera.setViewFor(vec3(0, 10, -10), 90.f, -45.f);
	//m_camera.setViewFor(vec3(0, 0,0), glm::quat(vec3(3.14/4.0f, 3.14/2.0f,0 )));
	//m_camera.setViewFor(vec3(0, 10, -15), glm::quat(vec3(-glm::pi<float>() / 5.0f, glm::pi<float>(),0)));
	m_camera.setViewFor(vec3(10, 20, 0), glm::quat(vec3(-glm::pi<float>() / 2.0f, 0, 0)));

	m_ambientLight = vec3(0.05f);



	// Make Entities
	SceneEntity* entity;

	Mirror* mirror = new Mirror(vec2(20, 20));
	mirror->Init();
	entity = CreateEntity(mirror, Shader::GetShader("BasicDecal"), 1.0f);
	entity->GetTransform()->setPosition(vec3(0, 0, 0));
	entity->GetTransform()->setOrientation(quat(vec3(-glm::pi<float>()/2.0, 0, 0)));


	entity = CreateEntity(CachedModel("./models/Pyro/pyro.fbx"), Shader::GetShader("NmappedRiggedPhong"), 0.005f);

	entity->m_textures.diffuse = CachedTexture("./models/Pyro/Pyro_D.tga");
	entity->m_textures.normal = CachedTexture("./models/Pyro/Pyro_N.tga");
	entity->m_textures.specular = CachedTexture("./models/Pyro/Pyro_S.tga");
	entity->m_textures.weights = vec4(0, 1, 1, 0);
	entity->spin = glm::quat(glm::vec3(0, 0.5, 0));
	entity->GetTransform()->setPosition(vec3(-10, 0, 0));
	entity->m_bounds = new BoundingBox(vec3(-2, 0, -2) / entity->GetTransform()->getScale(), vec3(2, 10, 2) / entity->GetTransform()->getScale());

	//entity = CreateEntity(CachedModel("./models/Pyro/pyro.fbx"), Shader::GetShader("NmappedRiggedPhong"), 0.005f);

	//entity->m_textures.diffuse = CachedTexture("./models/Pyro/Pyro_D.tga");
	//entity->m_textures.normal = CachedTexture("./models/Pyro/Pyro_N.tga");
	//entity->m_textures.specular = CachedTexture("./models/Pyro/Pyro_S.tga");
	//entity->m_textures.weights = vec4(0, 1, 1, 0);
	//entity->spin = glm::quat(glm::vec3(0, 0.5, 0));
	//entity->GetTransform()->setPosition(vec3(5, 0, 5));

	//entity = CreateEntity(CachedModel("./models/Pyro/pyro.fbx"), Shader::GetShader("NmappedRiggedPhong"), 0.005f);

	//entity->m_textures.diffuse = CachedTexture("./models/Pyro/Pyro_D.tga");
	//entity->m_textures.normal = CachedTexture("./models/Pyro/Pyro_N.tga");
	//entity->m_textures.specular = CachedTexture("./models/Pyro/Pyro_S.tga");
	//entity->m_textures.weights = vec4(0, 1, 1, 0);
	//entity->spin = glm::quat(glm::vec3(0, 0.5, 0));
	//entity->GetTransform()->setPosition(vec3(-5, 0, 5));
	//entity->m_animSpeed = 0.5f;

	//entity = CreateEntity(CachedModel("./models/Pyro/pyro.fbx"), Shader::GetShader("NmappedRiggedPhong"), 0.005f);

	//entity->m_textures.diffuse = CachedTexture("./models/Pyro/Pyro_D.tga");
	//entity->m_textures.normal = CachedTexture("./models/Pyro/Pyro_N.tga");
	//entity->m_textures.specular = CachedTexture("./models/Pyro/Pyro_S.tga");
	//entity->m_textures.weights = vec4(0, 1, 1, 0);
	//entity->spin = glm::quat(glm::vec3(0, 0.5, 0));
	//entity->GetTransform()->setPosition(vec3(-5, 0, -5));

	entity = CreateEntity(CachedModel("./models/Pyro/pyro.fbx"), Shader::GetShader("NmappedRiggedPhong"), 0.005f);

	entity->m_textures.diffuse = CachedTexture("./models/Pyro/Pyro_D.tga");
	entity->m_textures.normal = CachedTexture("./models/Pyro/Pyro_N.tga");
	entity->m_textures.specular = CachedTexture("./models/Pyro/Pyro_S.tga");
	entity->m_textures.weights = vec4(0, 1, 1, 0);
	entity->spin = glm::quat(glm::vec3(0, 0.5, 0));
	entity->GetTransform()->setPosition(vec3(5, 0, -5));
	entity->m_animSpeed = 1.4f;

	entity->m_bounds = new BoundingBox(vec3(-2, 0, -2) / entity->GetTransform()->getScale(), vec3(2, 10, 2) / entity->GetTransform()->getScale());




	//entity = CreateEntity(CachedModel("./models/DarkSiderGun/GUN.fbx"), Shader::GetShader("NmappedRiggedPhong"), 0.2f);

	//entity->m_textures.diffuse = CachedTexture("./models/DarkSiderGun/Gun_D.tga"); 
	//entity->m_textures.normal = CachedTexture("./models/DarkSiderGun/Gun_N_1.tga");
	//entity->m_textures.specular = CachedTexture("./models/DarkSiderGun/Gun_S.tga"); 
	//entity->m_textures.ambient = CachedTexture("./models/DarkSiderGun/Gun_A.tga"); 
	//entity->m_textures.weights = vec4(1,1,1,0);
	//entity->spin = glm::quat(glm::vec3(0.5, 0, 0));
	//entity->GetTransform()->setPosition(vec3(4, 0, 4));

	//entity = CreateEntity(CachedModel("./models/DarkSiderGun/GUN.fbx"), Shader::GetShader("NmappedRiggedPhong"), 0.2f);

	//entity->m_textures.diffuse = CachedTexture("./models/DarkSiderGun/Gun_D.tga");
	//entity->m_textures.normal = CachedTexture("./models/DarkSiderGun/Gun_N_1.tga");
	//entity->m_textures.specular = CachedTexture("./models/DarkSiderGun/Gun_S.tga");
	//entity->m_textures.ambient = CachedTexture("./models/DarkSiderGun/Gun_A.tga");
	//entity->m_textures.weights = vec4(1, 1, 1, 0);
	//entity->spin = glm::quat(glm::vec3(1, 0, 1));
	//entity->GetTransform()->setPosition(vec3(-4, 0, 0));

	//entity = CreateEntity(CachedModel("./models/DarkSiderGun/GUN.fbx"), Shader::GetShader("NmappedRiggedPhong"), 0.2f);

	//entity->m_textures.diffuse = CachedTexture("./models/DarkSiderGun/Gun_D.tga");
	//entity->m_textures.normal = CachedTexture("./models/DarkSiderGun/Gun_N_1.tga");
	//entity->m_textures.specular = CachedTexture("./models/DarkSiderGun/Gun_S.tga");
	//entity->m_textures.ambient = CachedTexture("./models/DarkSiderGun/Gun_A.tga");
	//entity->m_textures.weights = vec4(1, 1, 1, 0);
	//entity->spin = glm::quat(glm::vec3(1, 0, 0));
	//entity->GetTransform()->setPosition(vec3(-4, 0, 4));

	//entity = CreateEntity(CachedModel("./models/DarkSiderGun/GUN.fbx"), Shader::GetShader("NmappedRiggedPhong"), 0.2f);

	//entity->m_textures.diffuse = CachedTexture("./models/DarkSiderGun/Gun_D.tga");
	//entity->m_textures.normal = CachedTexture("./models/DarkSiderGun/Gun_N_1.tga");
	//entity->m_textures.specular = CachedTexture("./models/DarkSiderGun/Gun_S.tga");
	//entity->m_textures.ambient = CachedTexture("./models/DarkSiderGun/Gun_A.tga");
	//entity->m_textures.weights = vec4(1, 1, 1, 0);
	//entity->spin = glm::quat(glm::vec3(0, 0.7, 0));
	//entity->GetTransform()->setPosition(vec3(-4, 0, -4));



	// Setup Lighting

	m_directLightDir = glm::normalize( vec3(-1, -1, 1) );
	m_directLightClr = vec3(0.5);

	AddLight(Light(0, vec3(0, 5, 0), vec3(0.7f), vec3(1), 10));
	AddLight(Light(0, vec3(0, 5, -10), vec3(0, 0.6f, 0.6f), vec3(1), 50));
	AddLight(Light(0, vec3(10, 5, 0), vec3(1, 0, 1), vec3(1), 50));


	return; // breakpoint

}


void Scene::Update(float deltaTime) {

	for (auto entity : _Entities) {
		entity->Update(deltaTime);
	}

	m_camera.update(deltaTime);
}


// Predraw primarily used to update Mirror reflection textures
void Scene::Predraw() {
	for (auto entity : _Entities) {
		//if (entity == mirrorEntity) continue; // Check we're not rendering the current mirror
		
		Camera* camera = &m_camera;
		mat4 mvp = camera->getPvMatrix() * entity->GetTransform()->getMatrix();

		if (m_clipBounds && entity->isOffScreen(mvp)) continue; // Frustum Culling

		if (entity->m_mirror != 0) { // This is a mirror, let's update the reflection.
			entity->m_mirror->reflect(entity->GetTransform(), &m_camera.getView());

			entity->m_mirror->Begin(); // Bind mirror buffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // clear buffer
			this->Draw(entity); // Render to buffer
			entity->m_mirror->End(); // Unbind buffer
		}
	}
}

void Scene::Draw(SceneEntity* mirrorEntity) {
	int clippedCount = 0;

	for (auto entity : _Entities) {

		static bool runOnce = false;
		if (runOnce) {
			std::cout << "Entity Transform: " << glm::to_string( entity->GetTransform()->getMatrix() ) << std::endl;
			runOnce = false;
		}

		if (entity == mirrorEntity) continue; // Check we're not rendering the current mirror

		//Camera* camera = &m_camera; // (mirrorEntity == 0 ? m_camera : mirrorEntity->m_mirror->reflect(mirrorEntity->GetTransform(), &m_camera.getView()));
		Camera* camera = &(mirrorEntity == 0 ? m_camera : mirrorEntity->m_mirror->m_camera);
		Shader* shader = entity->m_shader;

		mat4 mvp = camera->getPvMatrix() * entity->GetTransform()->getMatrix();

		// Frustum Culling
		if (m_clipBounds && entity->isOffScreen(mvp)) {
			if (!mirrorEntity) clippedCount++;
			continue; 
		}


		// Bind Shader
		shader->MakeActive();

		shader->SetVec3("directLightDir", glm::value_ptr(glm::normalize(m_directLightDir)));
		shader->SetVec3("directLightClr", glm::value_ptr(m_directLightClr));

		shader->SetVec3Array("pointLtPos", MAX_LIGHTS, (float*)pointLtPos);
		shader->SetVec3Array("pointLtClr", MAX_LIGHTS, (float*)pointLtClr);
		shader->SetVec3Array("pointLtCoeff", MAX_LIGHTS, (float*)pointLtCoeff);
		shader->SetFloatArray("pointLtPwr", MAX_LIGHTS, (float*)pointLtPwr);

		shader->SetMat4("pvmMatrix", glm::value_ptr(mvp));
		shader->SetMat4("modelMatrix", glm::value_ptr(entity->GetTransform()->getMatrix()));

		shader->SetVec3("cameraPos", glm::value_ptr(camera->getPosition()));
		shader->SetVec3("globalAmbient", glm::value_ptr(m_ambientLight));

		shader->SetVec4("TexWeights", glm::value_ptr(entity->m_textures.weights));

		if (entity->m_textures.diffuse) shader->SetTexture("diffuseTex", 0, entity->m_textures.diffuse->getHandle());
		if (entity->m_textures.specular) shader->SetTexture("specularTex", 1, entity->m_textures.specular->getHandle());
		if (entity->m_textures.normal) shader->SetTexture("normalTex", 2, entity->m_textures.normal->getHandle());
		if (entity->m_textures.ambient) shader->SetTexture("ambientTex", 3, entity->m_textures.ambient->getHandle());


		if (entity->m_mirror != 0) { // This is a Mirror
			shader->SetTexture("decalTex", 0, entity->m_mirror->m_buffer.m_TexId[0]);
			m_boundsShader->SetVec3("decalClr", glm::value_ptr(vec3(0)));
			entity->m_mirror->draw();
		}

		if (entity->m_model != 0) { // This is an FBX File

			auto skelCount = entity->m_model->m_fbx->getSkeletonCount();

			shader->SetBool("hasBones", skelCount > 0);

			for (unsigned int i = 0; i < skelCount; ++i) {
				FBXSkeleton* skel = entity->m_model->m_fbx->getSkeletonByIndex(i);
				FBXAnimation* anim = entity->m_model->m_fbx->getAnimationByIndex(0);

				float timestep = fmodf(entity->m_timestep, anim->totalTime());

				skel->evaluate(anim, timestep);

				for (unsigned int bone_index = 0; bone_index < skel->m_boneCount; ++bone_index)
				{
					skel->m_nodes[bone_index]->updateGlobalTransform();
				}
			}

			for (unsigned int i = 0; i < skelCount; ++i) {
				FBXSkeleton* skeleton = entity->m_model->m_fbx->getSkeletonByIndex(i);
				skeleton->updateBones();

				shader->SetMat4Array("bones", skeleton->m_boneCount, (float*)skeleton->m_bones);
			}


			// Draw Model
			entity->m_model->draw();
		}

		if (m_drawBounds && entity->m_bounds != 0) {
			m_boundsShader->MakeActive();
			m_boundsShader->SetTexture("decalTex", 0, 0);
			
			m_boundsShader->SetMat4("pvmMatrix", glm::value_ptr(mvp));
			m_boundsShader->SetVec3("decalClr", glm::value_ptr(vec3(1)));

			entity->m_bounds->draw();
		}
	}

	m_clippedLastFrame = clippedCount;
}

SceneEntity* Scene::GetEntity(int idx)
{
	return _Entities[idx];
}

int Scene::GetEntityCount()
{
	return _Entities.size();
}

FbxModel* Scene::CachedModel(string filename) {


	std::map<std::string, FbxModel*>::iterator iter = _ModelCache.find(filename);
	if (iter != _ModelCache.end()) {
		return iter->second;
	}


	std::cout << "Loading Model: " << filename << std::flush;
	auto model = new FbxModel(filename);
	std::cout << " - Done!" << std::endl;

	_ModelCache[filename] = model;
	return model;
}

aie::Texture* Scene::CachedTexture(string filename) {

	std::map<std::string, aie::Texture*>::iterator iter = _TextureCache.find(filename);
	if (iter != _TextureCache.end()) {
		return iter->second;
	}


	std::cout << "Loading Texture File: " << filename << std::flush;
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

SceneEntity* Scene::CreateEntity(Mirror* mirror, Shader* shader, float scaleFactor) {
	auto entity = new SceneEntity(mirror, shader, scaleFactor);

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
