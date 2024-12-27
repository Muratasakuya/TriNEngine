#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/ComPtr.h>

// directX
#include <d3d12.h>
#include <dxgi1_6.h>

// c++
#include <cassert>

//============================================================================*/
//	DXDevice class
//============================================================================*/
class DXDevice {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	DXDevice() = default;
	~DXDevice();

	void Init();
	
	//* getter *//

	ID3D12Device* Get() const { return device_.Get(); };

	IDXGIFactory7* GetDxgiFactory() const { return dxgiFactory_.Get(); };

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	ComPtr<ID3D12Device> device_;

	ComPtr<IDXGIFactory7> dxgiFactory_;

	ComPtr<IDXGIAdapter4> useAdapter_;

};