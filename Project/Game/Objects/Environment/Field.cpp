#include "Field.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Asset/Asset.h>
#include <Lib/Adapter/JsonAdapter.h>

// imgui
#include <imgui.h>

//============================================================================*/
//	Field classMethods
//============================================================================*/

void Field::Init() {

	CreateModel(16);

	BaseGameObject::Init("field.obj");

	BaseGameObject::SetMeshRenderer("field");

	// Light
	BaseGameObject::SetBlinnPhongLightingEnable(true);
	// material
	materials_.front().properties.specularColor = Vector3(0.869f, 0.479f, 0.447f);
	materials_.front().properties.phongRefShininess = 2.8f;

	drawDepthShadowEnable_ = false;

	model_->SetTexture("fieldBaseTile");
	parentFolderName_ = "Field/";

	BaseGameObject::ApplyJsonForTransform(transform_);
	ApplyJson();

}

void Field::DerivedImGui() {

	if (ImGui::Button("Model Output")) {

		Asset::GetModel()->ExportToOBJ("field.obj");
	}

	ImGui::Text("UVTransform");

	if (ImGui::Button("Save##uv")) {

		SaveJson();
	}
	ImGui::DragFloat3("uvScale", &uvTransform.scale.x, 0.01f);
	ImGui::DragFloat3("uvRotate", &uvTransform.rotate.x, 0.01f);
	ImGui::DragFloat3("uvTranslate", &uvTransform.translate.x, 0.01f);
	BaseGameObject::SetUVTransform(uvTransform);

}

void Field::ApplyJson() {

	Json data = JsonAdapter::Load(parentFolderName_.value_or("") + GetName() + "SubParameter.json");

	// UVTransform
	uvTransform.scale = JsonAdapter::ToVector3(data["uvScale"]);
	uvTransform.rotate = JsonAdapter::ToVector3(data["uvRotate"]);
	uvTransform.translate = JsonAdapter::ToVector3(data["uvTranslate"]);
	BaseGameObject::SetUVTransform(uvTransform);

}

void Field::SaveJson() {

	Json data;
	data["uvScale"] = JsonAdapter::FromVector3(uvTransform.scale);
	data["uvRotate"] = JsonAdapter::FromVector3(uvTransform.rotate);
	data["uvTranslate"] = JsonAdapter::FromVector3(uvTransform.translate);

	JsonAdapter::Save(parentFolderName_.value_or("") + GetName() + "SubParameter.json", data);

}

void Field::CreateModel(int division) {

	float halfWidth = 1.0f;
	float halfDepth = 1.0f;

	std::vector<ModelVertexData> vertices;

	for (int z = 0; z <= division; ++z) {
		for (int x = 0; x <= division; ++x) {

			ModelVertexData vertex;

			// X, Z方向の位置
			float xPos = -halfWidth + (x / static_cast<float>(division)) * halfWidth * 2.0f;
			float zPos = -halfDepth + (z / static_cast<float>(division)) * halfWidth * 2.0f;
			vertex.pos = Vector4(xPos, 0.0f, zPos, 1.0f);

			// UV座標
			vertex.texcoord = Vector2(x / static_cast<float>(division), z / static_cast<float>(division));
			vertex.texcoord.y *= -1.0f;
			// 上固定
			vertex.normal = Vector3(0.0f, 1.0f, 0.0f);

			vertices.push_back(vertex);

		}
	}

	std::vector<uint32_t> indices;

	for (int z = 0; z < division; ++z) {
		for (int x = 0; x < division; ++x) {

			int topLeft = z * (division + 1) + x;
			int topRight = topLeft + 1;
			int bottomLeft = (z + 1) * (division + 1) + x;
			int bottomRight = bottomLeft + 1;

			// 左上三角形
			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);

			// 右下三角形
			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
		}
	}

	// モデル追加
	Asset::GetModel()->MakeOriginalModel("field.obj", vertices, indices);

}