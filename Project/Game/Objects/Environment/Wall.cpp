#include "Wall.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Lib/Adapter/JsonAdapter.h>
#include <Game/Objects/Player/Player.h>

// imgui
#include <imgui.h>

//============================================================================*/
//	Wall classMethods
//============================================================================*/

void Wall::Init(Player* player, uint32_t index) {

	player_ = player;

	startTime_ = std::chrono::steady_clock::now();

	BaseGameObject::Init("wall.obj");
	BaseGameObject::SetMeshRenderer("wall", index);
	// Light無効
	BaseGameObject::SetLightingEnable(false);

	if (index % 2 != 0) {
		transform_.rotation =
			Quaternion::MakeRotateAxisAngleQuaternion(Direction::Up(), std::numbers::pi_v<float> / 2.0f);
	}

	parentFolderName_ = "Wall/";

	BaseGameObject::ApplyJsonForTransform(transform_);
	ApplyJson();

}

void Wall::Update() {

	if (player_->IsPosClamped()) {

		materials_.front().properties.color.a = 1.0f;
	} else {

		// 経過時間の計算
		std::chrono::time_point<std::chrono::steady_clock> current_time = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration<float>(current_time - startTime_).count();

		materials_.front().properties.color.a
			= 0.6f * (1.0f + std::sin(0.8f * elapsed * std::numbers::pi_v<float>));
	}

	BaseGameObject::Update();
}

void Wall::DerivedImGui() {

	ImGui::Text("UVTransform");

	if (ImGui::Button("Save##uv")) {

		SaveJson();
	}
	ImGui::DragFloat3("uvScale", &uvTransform.scale.x, 0.01f);
	ImGui::DragFloat3("uvRotate", &uvTransform.rotate.x, 0.01f);
	ImGui::DragFloat3("uvTranslate", &uvTransform.translate.x, 0.01f);
	BaseGameObject::SetUVTransform(uvTransform);
}

void Wall::ApplyJson() {

	Json data = JsonAdapter::Load(parentFolderName_.value_or("") + "wall" + "SubParameter.json");

	// UVTransform
	uvTransform.scale = JsonAdapter::ToVector3(data["uvScale"]);
	uvTransform.rotate = JsonAdapter::ToVector3(data["uvRotate"]);
	uvTransform.translate = JsonAdapter::ToVector3(data["uvTranslate"]);
	BaseGameObject::SetUVTransform(uvTransform);
}

void Wall::SaveJson() {

	Json data;
	data["uvScale"] = JsonAdapter::FromVector3(uvTransform.scale);
	data["uvRotate"] = JsonAdapter::FromVector3(uvTransform.rotate);
	data["uvTranslate"] = JsonAdapter::FromVector3(uvTransform.translate);

	JsonAdapter::Save(parentFolderName_.value_or("") + "wall" + "SubParameter.json", data);
}