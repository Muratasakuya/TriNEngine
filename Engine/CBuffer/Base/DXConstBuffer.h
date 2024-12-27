#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/ComPtr.h>

// directX
#include <d3d12.h>

// c++
#include <vector>
#include <cstdint>
#include <cassert>

//============================================================================*/
//	DXConstBuffer class
//============================================================================*/
template<typename T>
class DXConstBuffer {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	DXConstBuffer() = default;
	virtual ~DXConstBuffer() = default;

	//* Create *//
	void CreateConstBuffer(ID3D12Device* device);
	void CreateVertexBuffer(ID3D12Device* device, UINT vertexCount);
	void CreateUavVertexBuffer(ID3D12Device* device, UINT vertexCount);
	void CreateStructuredBuffer(ID3D12Device* device, UINT instanceCount);
	void CreateIndexBuffer(ID3D12Device* device, UINT indexCount);

	//* Transfer *//
	void TransferData(const T& data);
	void TransferVectorData(const std::vector<T>& data);

	void SetCommand(ID3D12GraphicsCommandList* commandList);

	//* getter *//

	ID3D12Resource* GetResource() const;

	D3D12_VERTEX_BUFFER_VIEW& GetVertexBuffer();
	D3D12_INDEX_BUFFER_VIEW& GetIndexBuffer();

protected:
	//========================================================================*/
	//	protected Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	UINT rootParameterIndex = 0;

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	ComPtr<ID3D12Resource> resource_;
	T* mappedData_ = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_;

	//========================================================================*/
	//* functions

	ComPtr<ID3D12Resource> CreateVertexResource(ID3D12Device* device, size_t sizeInBytes);
	ComPtr<ID3D12Resource> CreateUavVertexResource(ID3D12Device* device, size_t sizeInBytes);

};

//============================================================================*/
//	DXConstBuffer classMethods
//============================================================================*/

template<typename T>
inline void DXConstBuffer<T>::CreateConstBuffer(ID3D12Device* device) {

	HRESULT hr;

	// 定数バッファーのリソース作成
	D3D12_HEAP_PROPERTIES heapProps{};
	heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = sizeof(T);
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	hr = device->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&resource_)
	);
	assert(SUCCEEDED(hr));

	// マッピング
	hr = resource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedData_));
	assert(SUCCEEDED(hr));
}

template<typename T>
void DXConstBuffer<T>::CreateVertexBuffer(ID3D12Device* device, UINT vertexCount) {

	HRESULT hr;

	if (vertexCount > 0) {

		// 頂点データサイズ
		UINT sizeVB = static_cast<UINT>(sizeof(T) * vertexCount);

		// 定数バッファーのリソース作成
		resource_ = CreateVertexResource(device, sizeVB);

		// 頂点バッファビューの作成
		vertexBufferView_.BufferLocation = resource_->GetGPUVirtualAddress();
		vertexBufferView_.SizeInBytes = sizeVB;
		vertexBufferView_.StrideInBytes = sizeof(T);

		// マッピング
		hr = resource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedData_));
		assert(SUCCEEDED(hr));
	}
}

template<typename T>
void DXConstBuffer<T>::CreateUavVertexBuffer(ID3D12Device* device, UINT vertexCount) {

	if (vertexCount > 0) {

		// 頂点データサイズ
		UINT sizeVB = static_cast<UINT>(sizeof(T) * vertexCount);

		// 定数バッファーのリソース作成
		resource_ = CreateUavVertexResource(device, sizeVB);

		// 頂点バッファビューの作成
		vertexBufferView_.BufferLocation = resource_->GetGPUVirtualAddress();
		vertexBufferView_.SizeInBytes = sizeVB;
		vertexBufferView_.StrideInBytes = sizeof(T);
	}
}

template<typename T>
void DXConstBuffer<T>::CreateStructuredBuffer(ID3D12Device* device, UINT instanceCount) {

	HRESULT hr;

	// 定数バッファーのリソース作成
	D3D12_HEAP_PROPERTIES heapProps{};
	heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = sizeof(T) * instanceCount;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	hr = device->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&resource_)
	);
	assert(SUCCEEDED(hr));

	// マッピング
	hr = resource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedData_));
	assert(SUCCEEDED(hr));
}

template<typename T>
void DXConstBuffer<T>::CreateIndexBuffer(ID3D12Device* device, UINT indexCount) {

	HRESULT hr;

	if (indexCount > 0) {

		// インデックスデータのサイズ
		UINT sizeIB = static_cast<UINT>(sizeof(T) * indexCount);

		// 定数バッファーのリソース作成
		resource_ = CreateVertexResource(device, sizeIB);

		indexBufferView_.BufferLocation = resource_->GetGPUVirtualAddress();
		indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
		indexBufferView_.SizeInBytes = sizeIB;

		// マッピング
		hr = resource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedData_));
		assert(SUCCEEDED(hr));
	}
}

template<typename T>
void DXConstBuffer<T>::TransferData(const T& data) {

	if (mappedData_) {

		std::memcpy(mappedData_, &data, sizeof(T));
	}
}

template<typename T>
void DXConstBuffer<T>::TransferVectorData(const std::vector<T>& data) {

	if (mappedData_) {

		std::memcpy(mappedData_, data.data(), sizeof(T) * data.size());
	}
}

template<typename T>
inline void DXConstBuffer<T>::SetCommand(ID3D12GraphicsCommandList* commandList) {

	commandList->SetGraphicsRootConstantBufferView(rootParameterIndex, resource_.Get()->GetGPUVirtualAddress());
}

template<typename T>
ComPtr<ID3D12Resource> DXConstBuffer<T>::CreateVertexResource(ID3D12Device* device, size_t sizeInBytes) {

	HRESULT hr;

	// 頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	// 頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	// バッファリソース。テクスチャの場合はまた別の設定をする
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	// リソースのサイズ
	vertexResourceDesc.Width = sizeInBytes;
	// バッファの場合はこれらは1にする決まり
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	// バッファの場合はこれにする決まり
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 実際に頂点リソースを作る
	ComPtr<ID3D12Resource> bufferResource = nullptr;
	hr = device->CreateCommittedResource(
		&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &vertexResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&bufferResource));
	assert(SUCCEEDED(hr));

	return bufferResource;
}

template<typename T>
ComPtr<ID3D12Resource> DXConstBuffer<T>::CreateUavVertexResource(ID3D12Device* device, size_t sizeInBytes) {

	HRESULT hr;

	// 頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	// 頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	// バッファリソース。テクスチャの場合はまた別の設定をする
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	// リソースのサイズ
	vertexResourceDesc.Width = sizeInBytes;
	// バッファの場合はこれらは1にする決まり
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	// バッファの場合はこれにする決まり
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	vertexResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	// 実際に頂点リソースを作る
	ComPtr<ID3D12Resource> bufferResource = nullptr;
	hr = device->CreateCommittedResource(
		&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &vertexResourceDesc,
		D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&bufferResource));
	assert(SUCCEEDED(hr));

	return bufferResource;
}

template<typename T>
ID3D12Resource* DXConstBuffer<T>::GetResource() const {

	return resource_.Get();
}

template<typename T>
D3D12_VERTEX_BUFFER_VIEW& DXConstBuffer<T>::GetVertexBuffer() {

	return vertexBufferView_;
}

template<typename T>
D3D12_INDEX_BUFFER_VIEW& DXConstBuffer<T>::GetIndexBuffer() {

	return indexBufferView_;
}