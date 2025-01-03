#include "BaseSprite.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Renderer/SpriteRenderer.h>

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

	color = Color::White();
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