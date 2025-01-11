#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/ComPtr.h>
#include <Engine/DXClass/DXSwapChain.h>
#include <Engine/DXClass/DXDevice.h>
#include <Engine/DXClass/DXCommand.h>
#include <Engine/DXClass/Managers/RtvManager.h>
#include <Engine/DXClass/Managers/DsvManager.h>
#include <Engine/DXClass/Managers/SrvManager.h>
#include <Engine//DXClass/Pipeline/PipelineTypes.h>
#include <Engine/DXClass/Managers/PipelineManager.h>
#include <Engine/Renderer/OffscreenRenderer.h>
#include <Engine/Renderer/ShadowMapRenderer.h>
#include <Engine/Renderer/EffectSceneRenderer.h>
#include <Engine/External/ImGuiManager.h>
#include <Engine/Window/WinApp.h>

// directX
#include <d3d12.h>
#include <dxgidebug.h>
#include <dxgi1_3.h>

// c++
#include <memory>

//============================================================================*/
//	GraphicsEngine class
//============================================================================*/
class GraphicsEngine {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	GraphicsEngine() = default;
	~GraphicsEngine() = default;

	void Init();

	void Finalize();

	bool ProcessMessage();

	void BeginRenderFrame();
	void EndRenderFrame();

	void Render();

	//* command *//

	static void SetShadowTextureCommand();

	//* getter *//

	static PipelineManager* GetPipeline() { return pipelineManager_.get(); }

	static DXDevice* GetDevice() { return device_.get(); };

	static DXCommand* GetCommand() { return command_.get(); }

	static SrvManager* SRV() { return srvManager_.get(); }

	D3D12_GPU_DESCRIPTOR_HANDLE GetGuiTextureGPUHandle() const { return offscreenRenderer_->GetGuiGPUHandle(); }

	D3D12_GPU_DESCRIPTOR_HANDLE GetDemoSceneGuiTextureGPUHandle() const { return effectSceneRenderer_->GetRenderTextureGPUHandle(); }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	static std::unique_ptr<DXDevice> device_;
	static std::unique_ptr<DXCommand> command_;

	static std::unique_ptr<SrvManager> srvManager_;
	static std::unique_ptr<PipelineManager> pipelineManager_;
	static std::unique_ptr<ShadowMapRenderer> shadowMapRenderer_;

	std::unique_ptr<WinApp> winApp_;

	std::unique_ptr<RtvManager> rtvManager_;
	std::unique_ptr<DsvManager> dsvManager_;

	std::unique_ptr<DXSwapChain> swapChain_;
	UINT backBufferIndex_;

	std::unique_ptr<OffscreenRenderer> offscreenRenderer_;
	std::unique_ptr<EffectSceneRenderer> effectSceneRenderer_;

	std::unique_ptr<ImGuiManager> imguiManager_;

	//========================================================================*/
	//* functoins

	void InitDXDevice();

	// shadowPre
	void BeginPreShadowDepth();
	// renderTexturePre
	void BeginPreOffscreen();
	// effectSceneRenderTexturePre
	void BeginPreEffectScene();

	void SetViewportAndScissor(uint32_t width, uint32_t height);

	// RenderTexture
	void RenderOffscreen();

};