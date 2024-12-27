#include "Asset.h"

//============================================================================*/
//	Asset classMethods
//============================================================================*/

std::unique_ptr<TextureManager> Asset::textureManager_ = nullptr;
std::unique_ptr<ModelManager> Asset::modelManager_ = nullptr;

void Asset::Init() {

	textureManager_ = std::make_unique<TextureManager>();
	modelManager_ = std::make_unique<ModelManager>();
}

void Asset::LoadTexture(const std::string& textureName) {

	textureManager_->Load(textureName);
}

void Asset::LoadModel(const std::string& directoryPath, const std::string& modelName) {

	modelManager_->LoadModel(directoryPath, modelName);
}

void Asset::LoadAnimation(
	const std::string& directoryPath, const std::string& animationName, const std::string& modelName) {

	modelManager_->LoadAnimation(directoryPath, animationName, modelName);
}