#include "Transform.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>
#include <Game/System/GameSystem.h>
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

	animationController_.first = false;
	animationTime_ = 0.0f;

}

void AnimationTransform::Update() {

	if (animationController_.first) {

		//* repeat
		animationTime_ += 0.016f;
		animationTime_ = std::fmod(animationTime_, animationData_[animationController_.second].duration);

		// スケルトンが存在する場合
		if (skeleton_[animationController_.second]) {

			// アニメーション適応
			Asset::GetModel()->ApplyAnimation(skeleton_[animationController_.second].value().name, animationTime_);
			// スケルトンの更新
			Asset::GetModel()->SkeletonUpdate(skeleton_[animationController_.second].value().name);
			// スキンクラスターの更新
			Asset::GetModel()->SkinClusterUpdate(skeleton_[animationController_.second].value().name);
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

void AnimationTransform::SetPlayAnimation(bool isPlayAnimation, const std::string& animationName) {

	animationController_.first = isPlayAnimation;
	animationController_.second = animationName;
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