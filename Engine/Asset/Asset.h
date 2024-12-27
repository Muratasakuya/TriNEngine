#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Asset/TextureManager.h>
#include <Engine/Asset/ModelManager.h>

// c++
#include <memory>

//============================================================================*/
//	Asset class
//============================================================================*/
class Asset {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	Asset() = default;
	~Asset() = default;

	static void Init();

	static void LoadTexture(const std::string& textureName);

	static void LoadModel(const std::string& directoryPath, const std::string& modelName);

	static void LoadAnimation(const std::string& directoryPath, const std::string& animationName, const std::string& modelName);

	//* getter *//

	static TextureManager* GetTexture() { return textureManager_.get(); }

	static ModelManager* GetModel() { return modelManager_.get(); }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	static std::unique_ptr<TextureManager> textureManager_;

	static std::unique_ptr<ModelManager> modelManager_;

};