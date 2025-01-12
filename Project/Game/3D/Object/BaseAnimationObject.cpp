#include "BaseAnimationObject.h"

//============================================================================*/
//	include
//============================================================================*/

// imgui
#include <imgui.h>

//============================================================================*/
//	BaseAnimationObject classMethods
//============================================================================*/

void BaseAnimationObject::Init(const std::string& modelName, const std::string& animationName) {

	model_ = std::make_unique<AnimationModel>();
	model_->Init(modelName, animationName);

	transform_.Init(modelName, animationName);

	materials_.emplace_back();
	for (auto& material : materials_) {

		material.Init();
		material.properties.color = Color::White();
		material.properties.enableLighting = true;
		material.properties.enableHalfLambert = true;
	}

	isTransitioning_ = false;

}

void BaseAnimationObject::Update() {

	transform_.Update();
	// dispatch
	model_->SetComputeCommands();

	for (auto& material : materials_) {

		material.Update();
	}

	if (isTransitioning_) {
		if (!transform_.IsTransition()) {

			// Animation遷移完了
			model_->SetAnimationName(nextAnimationKey_);

			currentAnimationKey_ = nextAnimationKey_;
			isTransitioning_ = false;
		}
	}

}

void BaseAnimationObject::Draw(RendererPipelineType pipeline) {

	model_->Draw(transform_, materials_.front(), pipeline);
}

void BaseAnimationObject::DrawShadowDepth() {

	model_->DrawShadowDepth(transform_);
}

void BaseAnimationObject::SwitchAnimation(
	const std::string& animationKey, bool loopAnimation, float transitionDuration) {

	if (nextAnimationKey_ == animationKey) {
		return;
	}

	// 次のAnimationの名前を保持
	nextAnimationKey_ = animationKey;

	// 遷移開始
	transform_.SwitchAnimation(animationKey, loopAnimation, transitionDuration);
	isTransitioning_ = true;

}

void BaseAnimationObject::TransformImGui() {

	if (ImGui::TreeNode("Transform")) {

		if (ImGui::Button("Save")) {
			IBaseGameObject::SaveJsonForTransform(transform_);
		}

		ImGui::DragFloat3("Scale", &transform_.scale.x, 0.01f);
		ImGui::DragFloat3("Translate", &transform_.translation.x, 0.01f);

		ImGui::TreePop();
	}
}

void BaseAnimationObject::SetWorldTransform(const AnimationTransform& transform) {

	transform_.translation = transform.translation;
	transform_.scale = transform.scale;
	transform_.rotation = transform.rotation;
}
