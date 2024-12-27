#include "DXDevice.h"

//============================================================================*/
//	DXDevice classMethods
//============================================================================*/

DXDevice::~DXDevice() {
	int a = 1;
	a = 0;
}

void DXDevice::Init() {

	dxgiFactory_ = nullptr;

	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
	assert(SUCCEEDED(hr));

	useAdapter_ = nullptr;

	for (UINT i = 0;
		dxgiFactory_->EnumAdapterByGpuPreference(
			i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&useAdapter_)) != DXGI_ERROR_NOT_FOUND; ++i) {

		DXGI_ADAPTER_DESC3 adapterDesc{};
		hr = useAdapter_->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr));

		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {

			break;
		}
		useAdapter_ = nullptr;
	}

	assert(useAdapter_ != nullptr);

	device_ = nullptr;

	D3D_FEATURE_LEVEL featuerLevels[] = {
		D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0
	};
	const char* featuerLevelStrings[] = { "12.2","12.1","12.0" };

	for (size_t i = 0; i < _countof(featuerLevels); i++) {

		hr = D3D12CreateDevice(useAdapter_.Get(), featuerLevels[i], IID_PPV_ARGS(&device_));

		if (SUCCEEDED(hr)) {

			break;
		}
	}

	assert(device_ != nullptr);
}