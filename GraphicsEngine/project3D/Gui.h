#pragma once
#include <glm/glm.hpp>
#include "SceneEntity.h"

using glm::vec3;

class Gui
{
public:
	static Gui *const instance;

	Gui();
	
	bool* pp_Enabled;
	int* pp_BlurSize;
	int* pp_BloomSize;
	float* pp_SobelWeight;
	unsigned int* m_fps;


	void draw();

private:
	const vec3 radToDeg = vec3(180 / glm::pi<float>());

	int gui_entityIdx = 0;
	vec3 gui_entityPosn;
	vec3 gui_entityEulerOrientation;
	vec3 gui_entityDrift;
	vec3 gui_entityEulerSpin;

	void _mapEntity(SceneEntity* entity);
	void _onEntityPosnChanged();
	void _onEntityOrientChanged();
	void _onEntityDriftChanged();
	void _onEntitySpinChange();

	void _onCreateEntityClicked();
};

