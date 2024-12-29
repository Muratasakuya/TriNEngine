#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Utility/AssetStructure.h>

// assimp
#include <Externals/assimp/include/assimp/Importer.hpp>
#include <Externals/assimp/include/assimp/scene.h>
#include <Externals/assimp/include/assimp/postprocess.h>

// c++
#include <cstdint>
#include <filesystem>
#include <unordered_map>
#include <codecvt>
#include <locale>

//============================================================================*/
//	ModelManager class
//============================================================================*/
class ModelManager {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	ModelManager() = default;
	~ModelManager() = default;

	void LoadModel(const std::string& directoryPath, const std::string& modelName);
	void LoadAnimation(const std::string& directoryPath, const std::string& animationFileName, const std::string& modelName);

	void MakeOriginalModel(const std::string& modelName,
		const std::vector<ModelVertexData>& vertexData, const std::vector<uint32_t>& indexData);

	void SkeletonUpdate(const std::string& animationName);

	void ApplyAnimation(const std::string& animationName, float animationTime);

	void SkinClusterUpdate(const std::string& animationName);

	void BlendAnimation(
		const std::string& oldAnimName, float oldAnimTime,
		const std::string& nextAnimName, float nextAnimTime,
		float alpha);

	//* getter *//

	ModelData GetModelData(const std::string& modelName);

	AnimationData GetAnimationData(const std::string& animationName);
	Skeleton GetSkeletonData(const std::string& animationName);
	SkinCluster GetSkinClusterData(const std::string& animationName);

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	uint32_t srvIndex_ = 0;

	std::unordered_map<std::string, ModelData> models_;
	std::unordered_map<std::string, AnimationData> animations_;
	std::unordered_map<std::string, Skeleton> skeletons_;
	std::unordered_map<std::string, SkinCluster> skinClusters_;

	//========================================================================*/
	//* functions

	ModelData LoadModelFile(const std::string& directoryPath, const std::string& filename);

	Node ReadNode(aiNode* node);

	Skeleton CreateSkeleton(const Node& rootNode, const std::string& animationName);
	int32_t CreateJoint(const Node& node, const std::optional<int32_t> parent, std::vector<Joint>& joints);
	SkinCluster CreateSkinCluster(const std::string& modelName, const std::string& animationName);

	ComPtr<ID3D12Resource> CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);

	std::string RemoveFileExtension(const std::string& fileName);

};