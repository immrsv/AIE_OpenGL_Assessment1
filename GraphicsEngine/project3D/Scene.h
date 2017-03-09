#pragma once


#include <vector>
#include <iostream>
#include <glm/glm.hpp>

#include <Texture.h>

#include "SceneEntity.h"
#include "Camera.h"
#include "Shader.h"
#include "Light.h"
#include <Texture.h>
#include <FBXFile.h>
#include <vector>

using glm::mat4;
using glm::vec3;



using namespace std;


class Scene
{
public:
	static Scene* Instance();
	
	static FbxModel* CachedModel(string filename);
	static aie::Texture* CachedTexture(string filename);
	static Shader* CachedShader(string pseudonym, string vertexFilename, string fragmentFilename);

	SceneEntity* CreateEntity(FbxModel* model, Shader* shader, float scaleFactor = 1.f);
	SceneEntity* CreateEntity(Mirror* mirror, Shader* shader, float scaleFactor = 1.f);

	void Start();
	void Update(float deltaTime);
	void Predraw();
	void Draw(SceneEntity* mirrorEntity = 0);

	Camera m_camera;

	glm::vec3 m_ambientLight;
	glm::vec3 m_directLightDir;
	glm::vec3 m_directLightClr;

	int AddLight(Light& light);
	Light GetLight(int idx);
	bool RemoveLight(int idx);

	#define MAX_LIGHTS 128
	vec3 pointLtPos[MAX_LIGHTS];
	vec3 pointLtClr[MAX_LIGHTS];
	vec3 pointLtCoeff[MAX_LIGHTS];
	float pointLtPwr[MAX_LIGHTS];

	bool m_drawBounds = true;

private:
	Scene();
	~Scene();

	static Scene* _instance;
	static map<string, aie::Texture*> _TextureCache;
	static map<string, FbxModel*> _ModelCache;

	std::vector<SceneEntity*> _Entities;

	Shader* m_boundsShader;
};

