#include "IBaseGameObject.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Lib/Adapter/JsonAdapter.h>
#include <Engine/Renderer/MeshRenderer.h>

// imgui
#include <imgui.h>

//============================================================================*/
//	IBaseGameObject classMethods
//============================================================================*/

IBaseGameObject::~IBaseGameObject() {
	MeshRenderer::EraseGameObject(this);
}

void IBaseGameObject::ImGui() {
#ifdef _DEBUG

	ImGui::Checkbox("drawEnable", &drawEnable_);

	ImGui::Separator();

	std::string materialHeader;
	if (materials_.size() == 1) {
		materialHeader = "Material";
	} else {
		materialHeader = "Materials";
	}

	if (ImGui::TreeNode(materialHeader.c_str())) {

		if (ImGui::Button("Save")) {
			SaveJsonForColor();
		}
		for (size_t i = 0; i < materials_.size(); ++i) {

			std::string materialLabel;
			if (materials_.size() == 1) {
				materialLabel = "Color";
			} else {
				materialLabel = "Color " + std::to_string(i);
			}

			if (ImGui::TreeNode(materialLabel.c_str())) {

				ImGui::ColorEdit4("", &materials_[i].properties.color.r);
				ImGui::Text("R:%4.2f G:%4.2f B:%4.2f A:%4.2f",
					materials_[i].properties.color.r, materials_[i].properties.color.g,
					materials_[i].properties.color.b, materials_[i].properties.color.a);
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}

	TransformImGui();

	ImGui::Separator();
	DerivedImGui();

#endif // _DEBUG
}

void IBaseGameObject::ApplyJsonForColor() {

	std::string jsonPath = parentFolderName_.value_or("") + name_ + "Color.json";
	Json data = JsonAdapter::Load(jsonPath);

	materials_.front().properties.color = JsonAdapter::ToColor(data["color"]);
}

void IBaseGameObject::SaveJsonForColor() {

	Json data;
	data["color"] = JsonAdapter::FromColor(materials_.front().properties.color);

	std::string jsonPath = parentFolderName_.value_or("") + name_ + "Color.json";
	JsonAdapter::Save(jsonPath, data);
}

void IBaseGameObject::ApplyJsonForTransform(BaseTransform& transform, const std::optional<std::string>& name) {

	std::string jsonPath = parentFolderName_.value_or("") + name.value_or(name_) + "Transform.json";
	Json data = JsonAdapter::Load(jsonPath);

	transform.translation = JsonAdapter::ToVector3(data["translation"]);
	transform.scale = JsonAdapter::ToVector3(data["scale"]);
}

void IBaseGameObject::SaveJsonForTransform(const BaseTransform& transform) {

	Json data;
	data["translation"] = JsonAdapter::FromVector3(transform.translation);
	data["scale"] = JsonAdapter::FromVector3(transform.scale);

	std::string jsonPath = parentFolderName_.value_or("") + name_ + "Transform.json";
	JsonAdapter::Save(jsonPath, data);
}

void IBaseGameObject::SetMeshRenderer(const std::string& name, uint32_t index) {

	name_ = name;
	if (index != 0) {

		name_ = name_ + std::to_string(index);
	}

	MeshRenderer::SetGameObject(this);

}

void IBaseGameObject::SetUVTransform(const UVTransform& uvTransform) {

	for (auto& material : materials_) {

		material.properties.SetUVTransform(uvTransform.scale, uvTransform.rotate, uvTransform.translate);
	}
}

void IBaseGameObject::SetLightingEnable(bool enable) {

	for (auto& material : materials_) {
		material.properties.enableLighting = enable;
	}
}