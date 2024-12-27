#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/ComPtr.h>

// driectX
#include <d3d12.h>
#include <dxgi1_6.h>

// c++
#include <vector>
#include <cassert>

//============================================================================*/
//	DXSwapChain class
//============================================================================*/
class DXSwapChain {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	DXSwapChain() = default;
	~DXSwapChain() = default;

	void Init(HWND hwnd, ID3D12CommandQueue* commandQueue);

	//* getter *//

	ID3D12Resource* GetResource(uint32_t index) const { return resources_[index].Get(); }

	IDXGISwapChain4* Get() const { return swapChain_.Get(); }

	DXGI_SWAP_CHAIN_DESC1& GetDesc() { return desc_; }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	ComPtr<IDXGISwapChain4> swapChain_;

	std::vector<ComPtr<ID3D12Resource>> resources_;

	DXGI_SWAP_CHAIN_DESC1 desc_;

};