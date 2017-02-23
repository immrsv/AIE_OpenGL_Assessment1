#pragma once


#include <vector>
#include <iostream>
#include <glm/glm.hpp>

#include <Texture.h>

#include "SceneEntity.h"
#include "Camera.h"
#include "Model.h"
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
	
	static void CacheModel(string filename);
	static void CacheTexture(string filename);
	static void CacheShader(string pseudonym, string vertexFilename, string fragmentFilename);

	SceneEntity* CreateEntity(string model, string texture, string shader, float scaleFactor = 1.f);
	
	void Start();
	void Update(float deltaTime);
	void Draw();

	Camera m_camera;

	glm::vec3 m_ambientLight;
	

private:
	Scene();
	~Scene();

	static Scene* _instance;
	static map<string, aie::Texture*> _TextureCache;
	static map<string, Model*> _ModelCache;

	std::vector<SceneEntity*> _Entities;


};

