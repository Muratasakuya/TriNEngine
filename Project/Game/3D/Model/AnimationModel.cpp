#include "AnimationModel.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>
#include <Game/System/GameSystem.h>
#include <Engine/Asset/Asset.h>

//============================================================================*/
//	AnimationModel classMethods
//============================================================================*/

void AnimationModel::Init(const std::string& modelName, const std::string& animationName) {

	animationName_ = animationName;

	BaseModel::Init(modelName);

	inputVertices_.Init(static_cast<UINT>(inputAssembler_.GetVertexData().data.size()),
		inputAssembler_.GetVertexData().GetResource());
	outputVertices_.Init(static_cast<UINT>(inputAssembler_.GetVertexData().data.size()));

	skinningInfoDates_.Init(static_cast<UINT>(inputAssembler_.GetVertexData().data.size()));
}

void AnimationModel::Draw(AnimationTransform transform, MaterialBuffer material, RendererPipelineType pipeline) {

	auto commandList = GraphicsEngine::GetCommand()->GetCommandList();

	// D3D12_RESOURCE_STATE_UNORDERED_ACCESS -> D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER
	GraphicsEngine::GetCommand()->TransitionBarrier(
		outputVertices_.GetResource(),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

	GraphicsEngine::GetPipeline()->SetRendererPipeline(commandList, pipeline, kBlendModeNormal);
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->IASetVertexBuffers(0, 1, &outputVertices_.GetVertexBuffer());
	commandList->IASetIndexBuffer(&inputAssembler_.GetIndexData().GetIndexBuffer());
	material.SetCommand(commandList);
	transform.SetCommand(commandList);
	GameSystem::GameLight()->GetLightBuffer().SetCommand(commandList);
	GameSystem::GameCamera()->GetCamera3D()->GetCameraBuffer().SetCommand(commandList);
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = Asset::GetTexture()->GetTextureGPUHandle(modelData_.meshes.front().textureName.value());
	commandList->SetGraphicsRootDescriptorTable(2, gpuHandle);
	commandList->SetGraphicsRootConstantBufferView(5,
		GameSystem::GameCamera()->GetSunLightCamera()->GetLightVPBuffer().GetResource()->GetGPUVirtualAddress());

	// 影を落とすときのみ
	if (pipeline == RendererPipelineType::TargetShadowObject3D) {

		GraphicsEngine::SetShadowTextureCommand();
	}
	inputAssembler_.DrawCall(commandList, 0);

	// D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER -> D3D12_RESOURCE_STATE_UNORDERED_ACCESS
	GraphicsEngine::GetCommand()->TransitionBarrier(
		outputVertices_.GetResource(),
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

}

void AnimationModel::DrawShadowDepth(AnimationTransform transform) {

	auto commandList = GraphicsEngine::GetCommand()->GetCommandList();

	// D3D12_RESOURCE_STATE_UNORDERED_ACCESS -> D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER
	GraphicsEngine::GetCommand()->TransitionBarrier(
		outputVertices_.GetResource(),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

	GraphicsEngine::GetPipeline()->SetShadowPipeline(commandList, ShadowDepth);
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->IASetVertexBuffers(0, 1, &outputVertices_.GetVertexBuffer());
	commandList->IASetIndexBuffer(&inputAssembler_.GetIndexData().GetIndexBuffer());
	GameSystem::GameCamera()->GetSunLightCamera()->GetLightVPBuffer().SetCommand(commandList);
	transform.SetCommand(commandList);
	inputAssembler_.DrawCall(commandList, 0);

	// D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER -> D3D12_RESOURCE_STATE_UNORDERED_ACCESS
	GraphicsEngine::GetCommand()->TransitionBarrier(
		outputVertices_.GetResource(),
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

}

void AnimationModel::SetComputeCommands() {

	auto commandList = GraphicsEngine::GetCommand()->GetCommandList();

	GraphicsEngine::GetPipeline()->SetComputePipeline(commandList, SkinningCS);
	commandList->SetComputeRootDescriptorTable(0,
		Asset::GetModel()->GetSkinClusterData(animationName_).paletteSrvHandle.second);
	commandList->SetComputeRootDescriptorTable(1, inputVertices_.GetGpuHandle());
	commandList->SetComputeRootDescriptorTable(2,
		Asset::GetModel()->GetSkinClusterData(animationName_).influenceSrvHandle.second);
	commandList->SetComputeRootDescriptorTable(3, outputVertices_.GetGpuHandle());
	commandList->SetComputeRootConstantBufferView(4, skinningInfoDates_.GetResource()->GetGPUVirtualAddress());
	// Compute起動
	commandList->Dispatch(static_cast<UINT>(modelData_.meshes.front().vertices.size() + 1023) / 1024, 1, 1);
}

void AnimationModel::SetTexture(const std::string& textureName) {

	for (uint32_t meshIndex = 0; meshIndex < meshNum_; ++meshIndex) {

		modelData_.meshes[meshIndex].textureName = textureName;
	}
}