#include "DXCommand.h"

//============================================================================*/
//	DXCommand classMethods
//============================================================================*/

void DXCommand::Init(ID3D12Device* device) {

	reference_ = std::chrono::steady_clock::now();

	commandQueue_ = nullptr;
	HRESULT hr = device->CreateCommandQueue(&commandQueueDesc_, IID_PPV_ARGS(&commandQueue_));
	assert(SUCCEEDED(hr));

	commandAllocator_ = nullptr;
	hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
	assert(SUCCEEDED(hr));

	commandList_ = nullptr;
	hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));
	assert(SUCCEEDED(hr));

	fence_ = nullptr;
	fenceValue_ = 0;
	hr = device->CreateFence(fenceValue_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	assert(SUCCEEDED(hr));

	// FenceのSignalを待つためのイベントの作成する
	fenceEvent_ = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent_ != nullptr);

}

void DXCommand::Execute(IDXGISwapChain4* swapChain) {

	HRESULT hr = commandList_->Close();
	assert(SUCCEEDED(hr));

	// GPUにコマンド実行
	ID3D12CommandList* commandLists[] = { commandList_.Get() };
	commandQueue_->ExecuteCommandLists(1, commandLists);

	// GPUとOSに画面の交換を行うように通知する
	swapChain->Present(1, 0);

	// Feneceの値を更新
	fenceValue_++;
	commandQueue_->Signal(fence_.Get(), fenceValue_);

	// Fenceの値が指定したSignal値にたどり着いているか確認する
	if (fence_->GetCompletedValue() < fenceValue_) {

		fence_->SetEventOnCompletion(fenceValue_, fenceEvent_);
		// イベントを待つ
		WaitForSingleObject(fenceEvent_, INFINITE);
	}

	// FPS固定
	UpdateFixFPS();

	// 次のフレーム用のコマンドリストを準備
	hr = commandAllocator_->Reset();
	assert(SUCCEEDED(hr));
	hr = commandList_->Reset(commandAllocator_.Get(), nullptr);
	assert(SUCCEEDED(hr));
}

void DXCommand::WaitForGPU() {

	// コマンドリストの内容を確定させる。すべてのコマンドを積んでからCloseする
	HRESULT hr = commandList_->Close();
	assert(SUCCEEDED(hr));

	// GPUにコマンドリストの実行を行わせる
	ID3D12CommandList* commandLists[] = { commandList_.Get() };
	commandQueue_->ExecuteCommandLists(1, commandLists);

	// Feneceの値を更新
	fenceValue_++;
	commandQueue_->Signal(fence_.Get(), fenceValue_);

	// Fenceの値が指定したSignal値にたどり着いているか確認する
	if (fence_->GetCompletedValue() < fenceValue_) {

		fence_->SetEventOnCompletion(fenceValue_, fenceEvent_);
		// イベントを待つ
		WaitForSingleObject(fenceEvent_, INFINITE);
	}

	hr = commandAllocator_->Reset();
	assert(SUCCEEDED(hr));
	hr = commandList_->Reset(commandAllocator_.Get(), nullptr);
	assert(SUCCEEDED(hr));
}

void DXCommand::TransitionBarrier(
	ID3D12Resource* resource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter) {

	D3D12_RESOURCE_BARRIER barrier{};

	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	// バリアを貼る対象のリソース。引数で渡されたリソースに対して行う
	barrier.Transition.pResource = resource;
	// 遷移前(現在)のResourceState
	barrier.Transition.StateBefore = stateBefore;
	// 遷移後のResourceState
	barrier.Transition.StateAfter = stateAfter;

	commandList_->ResourceBarrier(1, &barrier);
}

void DXCommand::CopyRenderTexture(
	ID3D12Resource* dstResource, D3D12_RESOURCE_STATES dstState,
	ID3D12Resource* srcResource, D3D12_RESOURCE_STATES srcState) {

	// 状態遷移
	TransitionBarrier(srcResource, srcState, D3D12_RESOURCE_STATE_COPY_SOURCE);
	TransitionBarrier(dstResource, dstState, D3D12_RESOURCE_STATE_COPY_DEST);

	commandList_->CopyResource(dstResource, srcResource);

	// 元の状態に戻す
	TransitionBarrier(srcResource, D3D12_RESOURCE_STATE_COPY_SOURCE, srcState);
	TransitionBarrier(dstResource, D3D12_RESOURCE_STATE_COPY_DEST, dstState);
}

void DXCommand::UpdateFixFPS() {

	// フレームレートピッタリの時間
	constexpr std::chrono::microseconds kMinTime(static_cast<uint64_t>(1000000.0f / 60.0f));

	// 1/60秒よりわずかに短い時間
	constexpr std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 64.0f));

	// 現在時間を取得する
	auto now = std::chrono::steady_clock::now();
	// 前回記録からの経過時間を取得する
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	// 1/60秒 (よりわずかに短い時間) 経っていない場合
	if (elapsed < kMinCheckTime) {
		// 1/60秒経過するまで微小なスリープを繰り返す
		auto wait_until = reference_ + kMinTime;
		while (std::chrono::steady_clock::now() < wait_until) {
			std::this_thread::yield();
		}
	}

	// 現在の時間を記録する
	reference_ = std::chrono::steady_clock::now();
}

void DXCommand::Finalize(HWND hwnd) {

	CloseHandle(fenceEvent_);
	CloseWindow(hwnd);
}