#include "BaseSprite.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Renderer/SpriteRenderer.h>
#include <Engine/Utility/Environment.h>
#include <Engine/Asset/Asset.h>

// imgui
#include <imgui.h>

//============================================================================*/
//	BaseSprite classMethods
//============================================================================*/

BaseSprite::~BaseSprite() {
	SpriteRenderer::EraseSprite(this);
}

void BaseSprite::Init(const std::string& textureName) {

	sprite = std::make_unique<Sprite>(textureName, transform);
	textureName_ = textureName;

	color = Color::White();

	// default
	drawType_ = SpriteDrawType::None;

}

void BaseSprite::Update() {

	sprite->Update(transform, color);
}

void BaseSprite::Draw() {

	sprite->Draw();
}

void BaseSprite::ImGui() {

	if (ImGui::TreeNode("Material")) {

		ImGui::ColorEdit4("color", &color.r);
		ImGui::Text("R:%4.2f G:%4.2f B:%4.2f A:%4.2f",
			color.r, color.g, color.b, color.a);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Transform")) {

		ImGui::DragFloat2("pos", &transform.pos.x);
		ImGui::DragFloat2("size", &transform.size.x);
		ImGui::DragFloat2("textureLeftTop", &transform.textureLeftTop.x);
		ImGui::DragFloat2("textureSize", &transform.textureSize.x);

		ImGui::TreePop();
	}

	ImGui::Separator();
	DerivedImGui();

}

void BaseSprite::SetSpriteRenderer(const std::string& name) {

	name_ = name;
	SpriteRenderer::SetSprite(this);
}

void BaseSprite::SetCenterPos() {

	transform.pos =
		Vector2(static_cast<float>(kWindowWidth) / 2.0f, static_cast<float>(kWindowHeight) / 2.0f);
}

Vector2 BaseSprite::GetTextureSize() const {

	const DirectX::TexMetadata& metadata = Asset::GetTexture()->GetMetaData(textureName_);
	return Vector2(static_cast<float>(metadata.width), static_cast<float>(metadata.height));
}
