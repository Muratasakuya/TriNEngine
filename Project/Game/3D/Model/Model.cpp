#include "Model.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>
#include <Game/System/GameSystem.h>
#include <Engine/Asset/Asset.h>

//============================================================================*/
//	Model classMethods
//============================================================================*/

void Model::Draw(WorldTransform transform, std::vector<MaterialBuffer>& materials, RendererPipelineType pipeline) {

	auto commandList = GraphicsEngine::GetCommand()->GetCommandList();

	for (uint32_t meshIndex = 0; meshIndex < meshNum_; ++meshIndex) {

		GraphicsEngine::GetPipeline()->SetRendererPipeline(commandList, pipeline, kBlendModeNormal);
		inputAssembler_.SetBuffer(commandList, meshIndex);
		materials[meshIndex].SetCommand(commandList);
		transform.SetCommand(commandList);
		GameSystem::GameLight()->GetLightBuffer().SetCommand(commandList);
		GameSystem::GameCamera()->GetCamera3D()->GetCameraBuffer().SetCommand(commandList);
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = Asset::GetTexture()->GetTextureGPUHandle(modelData_.meshes[meshIndex].textureName.value());
		commandList->SetGraphicsRootDescriptorTable(2, gpuHandle);
		commandList->SetGraphicsRootConstantBufferView(5,
			GameSystem::GameCamera()->GetSunLightCamera()->GetLightVPBuffer().GetResource()->GetGPUVirtualAddress());
		
		// 影を落とすときのみ
		if (pipeline == RendererPipelineType::TargetShadowObject3D) {

			GraphicsEngine::SetShadowTextureCommand();
		}
		inputAssembler_.DrawCall(commandList, meshIndex);
	}
}

void Model::DrawShadowDepth(WorldTransform transform) {

	auto commandList = GraphicsEngine::GetCommand()->GetCommandList();

	for (uint32_t meshIndex = 0; meshIndex < meshNum_; ++meshIndex) {

		GraphicsEngine::GetPipeline()->SetShadowPipeline(commandList, ShadowDepth);
		inputAssembler_.SetBuffer(commandList, meshIndex);
		GameSystem::GameCamera()->GetSunLightCamera()->GetLightVPBuffer().SetCommand(commandList);
		transform.SetCommand(commandList);
		inputAssembler_.DrawCall(commandList, meshIndex);
	}

}

void Model::SetTexture(const std::string& textureName) {

	for (uint32_t meshIndex = 0; meshIndex < meshNum_; ++meshIndex) {

		modelData_.meshes[meshIndex].textureName = textureName;
	}
}