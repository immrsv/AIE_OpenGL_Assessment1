#include "Gui.h"

#include <imgui.h>
#include "Scene.h"

Gui *const Gui::instance = new Gui();

Gui::Gui() 
	: gui_entityIdx(0)
{}

void Gui::draw() {

	if (ImGui::Begin("Rendering")) {
		ImGui::Text("Render Mode: %s", *pp_Enabled ? "Post Processing" : "Forward Rendering");
		ImGui::Text("Frames per Second: %3i", *m_fps);

		if (ImGui::CollapsingHeader("Frustum Culling")) {
			ImGui::TreePush("Frustum Culling");
			ImGui::Checkbox("Draw Bounding Boxes", &Scene::instance->m_drawBounds);
			ImGui::Checkbox("Clip Bounding Boxes", &Scene::instance->m_clipBounds);
			ImGui::SliderFloat2("Screen Space", glm::value_ptr(BoundingBox::ClipSpace), 0.1f, 1.1f);
			ImGui::TreePop();
		}

		if (ImGui::CollapsingHeader("Post Processing")) {
			ImGui::TreePush("Post Processing");
			ImGui::Checkbox("PP Enabled", pp_Enabled);
			if (ImGui::TreeNode("Composition")) {
				ImGui::SliderInt("Blur Size", pp_BlurSize, 0, 10);
				ImGui::SliderInt("Bloom Size", pp_BloomSize, 0, 10);
				ImGui::SliderFloat("Sobel Weight", pp_SobelWeight, 0, 2);
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		if (ImGui::CollapsingHeader("Deferred Rendering")) {
			ImGui::TreePush("Deferred Rendering");
			ImGui::Text("Under Construction");
			ImGui::TreePop();
		}
	}
	ImGui::End();

	//if (ImGui::Begin("Lights")) {
	//	ImGui::Text("No Lights Available!");
	//}
	//ImGui::End();

	if (ImGui::Begin("Entities")) {
		if (Scene::instance->GetEntityCount() > 0) {
			if (ImGui::Button("Create New Pyro")) { _onCreateEntityClicked(); }

			ImGui::SliderInt("Index", &gui_entityIdx, 0, Scene::instance->GetEntityCount() - 1);

			SceneEntity* entity = Scene::instance->GetEntity(gui_entityIdx);
			_mapEntity(entity);

			ImGui::SliderFloat("Animation Speed", &entity->m_animSpeed, 0.0f, 3.0f); // Direct mode

			if (ImGui::CollapsingHeader("Model Transform")) {
				ImGui::TreePush("Model Transform");
				// Tree: Position + Orientation + Scale
				if (ImGui::SliderFloat3("Position", glm::value_ptr(gui_entityPosn), -20.0f, 20.0f)) { _onEntityPosnChanged(); }
				if (ImGui::SliderFloat3("Orientation", glm::value_ptr(gui_entityEulerOrientation), -180.0f, 180.0f)) { _onEntityOrientChanged(); }
				//if (ImGui::SliderFloat3("Position", gui_entityPosn, -20.0f, 20.0f)) { _onEntityPosnChanged(); }
				ImGui::TreePop();
			}

			if (ImGui::CollapsingHeader("Inertia")) {
				ImGui::TreePush("Inertia");
				// Tree: Drift + Spin
				if (ImGui::SliderFloat3("Drift", glm::value_ptr(gui_entityDrift), -20.0f, 20.0f)) { _onEntityDriftChanged(); }
				if (ImGui::SliderFloat3("Spin", glm::value_ptr(gui_entityEulerSpin), -180.0f, 180.0f)) { _onEntitySpinChange(); }
				ImGui::TreePop();
			}

			if (ImGui::CollapsingHeader("Texture Weights")) {
				ImGui::TreePush("Texture Weights");
				// Tree: Texture Weights: Ambient, Diffuse, Specular, Normal
				ImGui::SliderFloat("Ambient", glm::value_ptr(entity->m_textures.weights) + 0, 0, 2);
				ImGui::SliderFloat("Diffuse", glm::value_ptr(entity->m_textures.weights) + 1, 0, 2);
				ImGui::SliderFloat("Specular", glm::value_ptr(entity->m_textures.weights) + 2, 0, 2);
				ImGui::SliderFloat("Normal", glm::value_ptr(entity->m_textures.weights) + 3, 0, 2);
				ImGui::TreePop();
			}

		}
		else { // No Entities
			ImGui::Text("No Entities Available!");
		}
	}
	ImGui::End();

}


void Gui::_mapEntity(SceneEntity* entity) {
	gui_entityPosn = entity->GetTransform()->getPosition();
	gui_entityEulerOrientation = glm::eulerAngles(entity->GetTransform()->getOrientation()) * radToDeg;
	gui_entityDrift = entity->drift;
	gui_entityEulerSpin = glm::eulerAngles(entity->spin) * radToDeg;
}

void Gui::_onEntityPosnChanged() {
	SceneEntity* entity = Scene::instance->GetEntity(gui_entityIdx);
	entity->GetTransform()->setPosition(glm::vec3(gui_entityPosn[0], gui_entityPosn[1], gui_entityPosn[2]));
}

void Gui::_onEntityOrientChanged() {
	SceneEntity* entity = Scene::instance->GetEntity(gui_entityIdx);
	glm::quat orientation = glm::quat(gui_entityEulerOrientation / radToDeg);
	entity->GetTransform()->setOrientation(orientation);
}

void Gui::_onEntityDriftChanged() {
	SceneEntity* entity = Scene::instance->GetEntity(gui_entityIdx);
	entity->drift = gui_entityDrift;
}

void Gui::_onEntitySpinChange() {
	SceneEntity* entity = Scene::instance->GetEntity(gui_entityIdx);
	glm::quat spin = glm::quat(gui_entityEulerSpin / radToDeg);
	entity->spin = spin;
}

void Gui::_onCreateEntityClicked() {
	SceneEntity* entity = Scene::instance->CreateEntity(Scene::CachedModel("./models/Pyro/pyro.fbx"), Shader::GetShader("NmappedRiggedPhong"), 0.005f);

	entity->m_textures.diffuse = Scene::CachedTexture("./models/Pyro/Pyro_D.tga");
	entity->m_textures.normal = Scene::CachedTexture("./models/Pyro/Pyro_N.tga");
	entity->m_textures.specular = Scene::CachedTexture("./models/Pyro/Pyro_S.tga");
	entity->m_textures.weights = vec4(0, 1, 1, 0);
	entity->m_animSpeed = 1.0f;

	vec3 posn;
	
	posn.x = (float)(rand() % 20 - 10);
	posn.z = (float)(rand() % 20 - 10);

	entity->GetTransform()->setPosition(posn);

	entity->m_bounds = new BoundingBox(vec3(-2, 0, -2) / entity->GetTransform()->getScale(), vec3(2, 10, 2) / entity->GetTransform()->getScale());
}