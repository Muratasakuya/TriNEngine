#include "GraphicsEngine.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Utility/Environment.h>
#include <Engine/Renderer/ImGuiRenderer.h>
#include <Engine/Renderer/MeshRenderer.h>
#include <Engine/Renderer/ParticleRenderer.h>
#include <Engine/Renderer/SpriteRenderer.h>
#include <Engine/Process/Input.h>

//============================================================================*/
//	GraphicsEngine classMethods
//============================================================================*/

std::unique_ptr<DXCommand> GraphicsEngine::command_ = nullptr;
std::unique_ptr<DXDevice> GraphicsEngine::device_ = nullptr;
std::unique_ptr<SrvManager> GraphicsEngine::srvManager_ = nullptr;
std::unique_ptr<PipelineManager> GraphicsEngine::pipelineManager_ = nullptr;
std::unique_ptr<ShadowMapRenderer> GraphicsEngine::shadowMapRenderer_ = nullptr;
PostProcessPipelineType GraphicsEngine::postProcessPipeline_ = PostProcessPipelineType::CopyTexture;

void GraphicsEngine::InitDXDevice() {
#ifdef _DEBUG
	ComPtr<ID3D12Debug1> debugController = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {

		// デバッグレイヤーを有効化する
		debugController->EnableDebugLayer();

		// さらにGPU側でもチェックを行うようにする
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif

	device_->Init();

#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue> infoQueue = nullptr;
	if (SUCCEEDED(device_->Get()->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {

		// やばいエラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		// エラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		// 警告時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

		D3D12_MESSAGE_ID denyIds[] = {

			// https://stackoverflow.com/questions/69805245/directx-12-application-is-crashing-in-windows-11
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};

		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;

		infoQueue->PushStorageFilter(&filter);
	}
#endif
}

void GraphicsEngine::Init() {
#pragma warning(push)
#pragma warning(disable:6031)
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
#pragma warning(pop)

	// ウィンドウ作成
	winApp_ = std::make_unique<WinApp>();
	winApp_->Init();

	command_ = std::make_unique<DXCommand>();
	device_ = std::make_unique<DXDevice>();

	// device、command初期化
	InitDXDevice();
	command_->Init(device_->Get());

	// 画面描画設定
	swapChain_ = std::make_unique<DXSwapChain>();
	swapChain_->Init(winApp_->GetHwnd(), command_->GetQueue());

	rtvManager_ = std::make_unique<RtvManager>();
	rtvManager_->Init();
	for (uint32_t index = 0; index < kBufferCount; ++index) {

		rtvManager_->Create(index, swapChain_->GetResource(index));
	}

	dsvManager_ = std::make_unique<DsvManager>();
	dsvManager_->Init();

	srvManager_ = std::make_unique<SrvManager>();
	srvManager_->Init();

#ifdef _DEBUG
	imguiManager_ = std::make_unique<ImGuiManager>();
	imguiManager_->Init(winApp_->GetHwnd(), swapChain_->GetDesc().BufferCount, srvManager_.get());
#endif

	// RenderTexture作成
	offscreenRenderer_ = std::make_unique<OffscreenRenderer>();
	offscreenRenderer_->Init(srvManager_.get(), rtvManager_.get());

	// ShadowMap作成
	shadowMapRenderer_ = std::make_unique<ShadowMapRenderer>();
	shadowMapRenderer_->Init(srvManager_.get(), dsvManager_.get());

	pipelineManager_ = std::make_unique<PipelineManager>();
	pipelineManager_->Create(device_->Get());

	rtvManager_->Reset();

	// Inputの初期化
	Input::GetInstance()->Init(winApp_.get());

}

void GraphicsEngine::Finalize() {

#ifdef _DEBUG
	imguiManager_->Finalize();
#endif

	command_->Finalize(winApp_->GetHwnd());
	device_.reset();
	command_.reset();

	pipelineManager_.reset();
	rtvManager_.reset();
	srvManager_.reset();
	dsvManager_.reset();
	swapChain_.reset();
	offscreenRenderer_.reset();
	shadowMapRenderer_.reset();
	imguiManager_.reset();

	// ComFinalize
	CoUninitialize();

}

bool GraphicsEngine::ProcessMessage() {
	return winApp_->ProcessMessage();
}

void GraphicsEngine::BeginRenderFrame() {

#ifdef _DEBUG
	imguiManager_->Begin();
#endif

	srvManager_->SetDescriptorHeaps(command_->GetCommandList());

}

void GraphicsEngine::Render() {

	// ShadowDepth
	BeginPreShadowDepth();
	MeshRenderer::RenderShadowDepth();
	command_->TransitionBarrier(shadowMapRenderer_->GetShadowResource(),
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	// RenderTexture
	BeginPreOffscreen();
	MeshRenderer::Render();
	ParticleRenderer::Render();
	SpriteRenderer::Render();
	command_->TransitionBarrier(offscreenRenderer_->GetRenderTexture(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	// SwapChain
	RenderOffscreen();

}

void GraphicsEngine::SetShadowTextureCommand() {

	ID3D12GraphicsCommandList* commandList = command_->GetCommandList();
	commandList->SetGraphicsRootDescriptorTable(6, shadowMapRenderer_->GetShadowGPUHandle());
}

void GraphicsEngine::BeginPreShadowDepth() {

	ID3D12GraphicsCommandList* commandList = command_->GetCommandList();
	D3D12_CPU_DESCRIPTOR_HANDLE dsvCPUHandle = dsvManager_->GetShadowMapCPUHandle();

	commandList->OMSetRenderTargets(0, nullptr, FALSE, &dsvCPUHandle);
	dsvManager_->ClearShadowDepthStencilView(commandList);
	SetViewportAndScissor(kShadowMapWidth, kShadowMapHeight);

}

void GraphicsEngine::BeginPreOffscreen() {

	ID3D12GraphicsCommandList* commandList = command_->GetCommandList();
	D3D12_CPU_DESCRIPTOR_HANDLE dsvCPUHandle = dsvManager_->GetNoramlCPUHandle();

	rtvManager_->BeginOffscreenSetRenderTargets(commandList, dsvCPUHandle);
	dsvManager_->ClearDepthStencilView(commandList);
	SetViewportAndScissor(kWindowWidth, kWindowHeight);

}

void GraphicsEngine::SetViewportAndScissor(uint32_t width, uint32_t height) {

	ID3D12GraphicsCommandList* commandList = command_->GetCommandList();
	D3D12_VIEWPORT viewport{};
	D3D12_RECT scissorRect{};

	viewport =
		D3D12_VIEWPORT(0.0f, 0.0f, float(width), float(height), 0.0f, 1.0f);
	commandList->RSSetViewports(1, &viewport);

	scissorRect = D3D12_RECT(0, 0, width, height);
	commandList->RSSetScissorRects(1, &scissorRect);
}

void GraphicsEngine::RenderOffscreen() {

	ID3D12GraphicsCommandList* commandList = command_->GetCommandList();
	backBufferIndex_ = swapChain_->Get()->GetCurrentBackBufferIndex();

	command_->TransitionBarrier(
		swapChain_->GetResource(backBufferIndex_),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	rtvManager_->SetRenderTargets(commandList, backBufferIndex_);
	SetViewportAndScissor(kWindowWidth, kWindowHeight);

	// Offscreen描画
	const UINT vertexCount = 3;

	pipelineManager_->SetPostProcessPipeline(commandList, postProcessPipeline_);
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->SetGraphicsRootDescriptorTable(0, offscreenRenderer_->GetRenderTextureGPUHandle());
	commandList->DrawInstanced(vertexCount, 1, 0, 0);

}

void GraphicsEngine::EndRenderFrame() {

#ifdef _DEBUG
	command_->CopyRenderTexture(
		offscreenRenderer_->GetGuiTexture(), D3D12_RESOURCE_STATE_RENDER_TARGET,
		swapChain_->GetResource(backBufferIndex_), D3D12_RESOURCE_STATE_RENDER_TARGET);

	command_->TransitionBarrier(
		offscreenRenderer_->GetGuiTexture(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	imguiManager_->End();
	imguiManager_->Draw(command_->GetCommandList());

	command_->TransitionBarrier(offscreenRenderer_->GetGuiTexture(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET);
#endif
	command_->TransitionBarrier(offscreenRenderer_->GetRenderTexture(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET);
	command_->TransitionBarrier(
		shadowMapRenderer_->GetShadowResource(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_DEPTH_WRITE);

	command_->TransitionBarrier(swapChain_->GetResource(backBufferIndex_),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT);

	command_->Execute(swapChain_->Get());

	rtvManager_->Reset();

}