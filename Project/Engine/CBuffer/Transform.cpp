#include "Transform.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>
#include <Game/System/GameSystem.h>
#include <Game/Utility/GameTimer.h>
#include <Engine/Asset/Asset.h>

//============================================================================*/
//	BaseTransform classMethods
//============================================================================*/

void BaseTransform::Init() {

	rootParameterIndex = 1;

	scale = Vector3(1.0f, 1.0f, 1.0f);
	rotation.Init();
	translation.Init();

	DXConstBuffer::CreateConstBuffer(GraphicsEngine::GetDevice()->Get());

}

Vector3 BaseTransform::GetWorldPos() const {

	Vector3 worldPos{};
	worldPos.x = matrix.world.m[3][0];
	worldPos.y = matrix.world.m[3][1];
	worldPos.z = matrix.world.m[3][2];

	return worldPos;
}

//============================================================================*/
//	WorldTransform classMethods
//============================================================================*/

void WorldTransform::Update() {

	Matrix4x4 worldMatrix =
		Matrix4x4::MakeAxisAffineMatrix(scale, rotation, translation);
	if (parent) {

		worldMatrix = Matrix4x4::Multiply(worldMatrix, parent->matrix.world);
	}
	Matrix4x4 wvpMatrix = worldMatrix * GameSystem::GameCamera()->GetCamera3D()->GetViewProjectionMatrix();
	Matrix4x4 worldInverseTranspose = Matrix4x4::Transpose(Matrix4x4::Inverse(worldMatrix));

	matrix.world = worldMatrix;
	matrix.wvp = wvpMatrix;
	matrix.worldInverseTranspose = worldInverseTranspose;

	DXConstBuffer::TransferData(matrix);

}

//============================================================================*/
//	AnimationTransform classMethods
//============================================================================*/

void AnimationTransform::Init(const std::string& modelName, const std::string& animationName) {

	BaseTransform::Init();

	animationData_[animationName] = Asset::GetModel()->GetAnimationData(animationName);
	skeleton_[animationName] = Asset::GetModel()->GetSkeletonData(animationName);
	skeleton_[animationName].value().name = animationName;
	skinCluster_[animationName] = Asset::GetModel()->GetSkinClusterData(animationName);

	modelData_ = Asset::GetModel()->GetModelData(modelName);

}

void AnimationTransform::Update() {

	float deltaTime = GameTimer::GetScaledDeltaTime();

	//============================================================================*/
	// 通常のAnimation再生
	//============================================================================*/
	if (!inTransition_) {
		if (roopAnimation_) {

			currentAnimationTimer_ += deltaTime;
			currentAnimationTimer_ = std::fmod(currentAnimationTimer_, animationData_[currentAnimationName_].duration);
		} else {
			if (animationData_[currentAnimationName_].duration > currentAnimationTimer_) {
				currentAnimationTimer_ += deltaTime;
			}
			if (currentAnimationTimer_ >= animationData_[currentAnimationName_].duration) {
				currentAnimationTimer_ = animationData_[currentAnimationName_].duration;
			}
		}

		if (skeleton_[currentAnimationName_]) {
			Asset::GetModel()->ApplyAnimation(skeleton_[currentAnimationName_].value().name, currentAnimationTimer_);
			Asset::GetModel()->SkeletonUpdate(skeleton_[currentAnimationName_].value().name);
			Asset::GetModel()->SkinClusterUpdate(skeleton_[currentAnimationName_].value().name);
		}
	}
	//============================================================================*/
	// 遷移中のAnimation再生
	//============================================================================*/
	else {
		
		// 遷移時間を進める
		transitionTimer_ += deltaTime;
		float alpha = transitionTimer_ / transitionDuration_;
		if (alpha > 1.0f) {
			alpha = 1.0f;
		}

		oldAnimationTimer_ += deltaTime;
		if (roopAnimation_) {
			oldAnimationTimer_ = std::fmod(oldAnimationTimer_, animationData_[oldAnimationName_].duration);
		}
		if (oldAnimationTimer_ >= animationData_[oldAnimationName_].duration) {
			oldAnimationTimer_ = animationData_[oldAnimationName_].duration;
		}

		nextAnimationTimer_ += deltaTime;
		if (roopAnimation_) {
			nextAnimationTimer_ = std::fmod(nextAnimationTimer_, animationData_[nextAnimationName_].duration);
		}
		if (nextAnimationTimer_ >= animationData_[nextAnimationName_].duration) {
			nextAnimationTimer_ = animationData_[nextAnimationName_].duration;
		}

		// AnimationをBlendして更新する
		Asset::GetModel()->BlendAnimation(skeleton_[oldAnimationName_].value().name,oldAnimationTimer_,
			skeleton_[nextAnimationName_].value().name,nextAnimationTimer_,alpha);
		Asset::GetModel()->SkeletonUpdate(skeleton_[oldAnimationName_].value().name);
		Asset::GetModel()->SkinClusterUpdate(skeleton_[oldAnimationName_].value().name);

		// 遷移終了
		if (alpha >= 1.0f) {

			inTransition_ = false;
			currentAnimationName_ = nextAnimationName_;
			currentAnimationTimer_ = nextAnimationTimer_;
		}
	}

	Matrix4x4 worldMatrix = Matrix4x4::MakeAxisAffineMatrix(scale, rotation, translation);
	Matrix4x4 wvpMatrix = worldMatrix * GameSystem::GameCamera()->GetCamera3D()->GetViewProjectionMatrix();
	Matrix4x4 worldInverseTranspose = Matrix4x4::Transpose(Matrix4x4::Inverse(worldMatrix));

	matrix.world = worldMatrix;
	matrix.wvp = wvpMatrix;
	matrix.worldInverseTranspose = worldInverseTranspose;

	DXConstBuffer::TransferData(matrix);

}

void AnimationTransform::AnimationInfo() {

	ImGui::Checkbox("roopAnimation", &roopAnimation_);
	ImGui::SameLine();
	if (ImGui::Button("Restart")) {
		currentAnimationTimer_ = 0.0f;
	}
	float progress = currentAnimationTimer_ / animationData_[currentAnimationName_].duration;
	ImGui::ProgressBar(progress);
}

void AnimationTransform::SetPlayAnimation(const std::string& animationName, bool roopAnimation) {

	// Animationの再生設定
	currentAnimationTimer_ = 0.0f;
	currentAnimationName_ = animationName;
	roopAnimation_ = roopAnimation;

}

void AnimationTransform::SwitchAnimation(const std::string& nextAnimName, bool loopAnimation, float transitionDuration) {

	// 現在のAnimationを設定
	oldAnimationName_ = currentAnimationName_;
	oldAnimationTimer_ = currentAnimationTimer_;

	// 次のAnimationを設定
	nextAnimationName_ = nextAnimName;
	nextAnimationTimer_ = 0.0f;

	// 遷移開始
	inTransition_ = true;
	transitionTimer_ = 0.0f;
	transitionDuration_ = transitionDuration;

	roopAnimation_ = loopAnimation;
}

void AnimationTransform::SetNewAnimationData(const std::string& animationName) {

	animationData_[animationName] = Asset::GetModel()->GetAnimationData(animationName);
	skeleton_[animationName] = Asset::GetModel()->GetSkeletonData(animationName);
	skeleton_[animationName].value().name = animationName;
	skinCluster_[animationName] = Asset::GetModel()->GetSkinClusterData(animationName);

}

//============================================================================*/
//	SpriteTransformBuffer classMethod
//============================================================================*/

void SpriteTransformBuffer::Init() {

	rootParameterIndex = 1;

	DXConstBuffer::CreateConstBuffer(GraphicsEngine::GetDevice()->Get());

}

void SpriteTransformBuffer::Update(const Transform2D& transform) {

	Vector3 scale = { transform.size.x,transform.size.y,1.0f };
	Vector3 rotate = { 0.0f,0.0f,transform.rotate };
	Vector3 translate = { transform.pos.x,transform.pos.y,0.0f };

	Matrix4x4 worldMatrix =
		Matrix4x4::MakeAffineMatrix(scale, rotate, translate);
	Matrix4x4 wvpMatrix = worldMatrix * GameSystem::GameCamera()->GetCamera2D()->GetViewOrthoMatrix();

	DXConstBuffer::TransferData(wvpMatrix);

}