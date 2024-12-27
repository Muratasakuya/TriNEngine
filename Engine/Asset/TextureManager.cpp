#include "TextureManager.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>

//============================================================================*/
//	TextureManager classMethods
//============================================================================*/

void TextureManager::Load(const std::string& textureName) {

	// 読みこみ済みなら早期リターン
	if (textures_.contains(textureName)) {
		return;
	}

	std::string baseDirectory = "./Resources/Textures/";
	std::filesystem::path filePath;

	bool found = false;
	for (const auto& entry : std::filesystem::recursive_directory_iterator(baseDirectory)) {
		if (entry.is_regular_file() && entry.path().stem().string() == textureName) {

			filePath = entry.path();
			found = true;
			break;
		}
	}
	assert(found && "Error: Texture not found in directory or its subdirectories.");

	std::string identifier = filePath.stem().string();
	TextureData& texture = textures_[identifier];

	DirectX::ScratchImage mipImages = GenerateMipMaps(filePath.string());
	texture.metadata = mipImages.GetMetadata();
	texture.resource = CreateTextureResource(GraphicsEngine::GetDevice()->Get(), texture.metadata);
	ComPtr<ID3D12Resource> intermediateResource = UploadTextureData(texture.resource.Get(), mipImages);

	// GPU実行の完了を待つ
	GraphicsEngine::GetCommand()->WaitForGPU();
	intermediateResource.Reset();

	// SRV確保
	texture.srvIndex = GraphicsEngine::SRV()->Allocate();
	texture.cpuHandle = GraphicsEngine::SRV()->GetCPUHandle(texture.srvIndex);
	texture.gpuHandle = GraphicsEngine::SRV()->GetGPUHandle(texture.srvIndex);
	GraphicsEngine::SRV()->CreateSRVForTexture2D(
		texture.srvIndex, texture.resource.Get(), texture.metadata.format, UINT(texture.metadata.mipLevels));

	std::wstring resourceName = std::wstring(identifier.begin(), identifier.end());
	texture.resource->SetName(resourceName.c_str());

}

DirectX::ScratchImage TextureManager::GenerateMipMaps(const std::string& filePath) {

	// テクスチャファイルを呼んでプログラムを扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	// ミップマップの作成 → 元画像よりも小さなテクスチャ群
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(
		image.GetImages(), image.GetImageCount(), image.GetMetadata(),
		DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	// ミップマップ付きのデータを返す
	return mipImages;
}

ComPtr<ID3D12Resource> TextureManager::CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata) {

	HRESULT hr;

	// metadataを元にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width);                             // Textureの幅
	resourceDesc.Height = UINT(metadata.height);                           // Textureの高さ
	resourceDesc.MipLevels = UINT16(metadata.mipLevels);                   // mipmapの数
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);            // 奥行き　or 配列textureの配列数
	resourceDesc.Format = metadata.format;                                 // TextureのFormat
	resourceDesc.SampleDesc.Count = 1;                                     // サンプリングカウント。1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension); // Textureの次元数、普段は2次元

	// 利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	// Resourceの作成
	ComPtr<ID3D12Resource> resource = nullptr;
	hr =
		device->CreateCommittedResource(
			&heapProperties,                   // Heapの設定
			D3D12_HEAP_FLAG_NONE,              // Heapの特殊な設定
			&resourceDesc,                     // Resourceの設定
			D3D12_RESOURCE_STATE_COPY_DEST,    // 初回のResourceState、Textureは基本読むだけ
			nullptr,                           // Clear最適値、使わない。
			IID_PPV_ARGS(&resource)            // 作成するResourceポインタへのポインタ
		);
	assert(SUCCEEDED(hr));

	return resource;
}

[[nodiscard]]
ComPtr<ID3D12Resource> TextureManager::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages) {

	std::vector<D3D12_SUBRESOURCE_DATA> subresources;
	ID3D12GraphicsCommandList* commandlist = GraphicsEngine::GetCommand()->GetCommandList();

	DirectX::PrepareUpload(GraphicsEngine::GetDevice()->Get(), mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subresources);
	uint64_t intermediateSize = GetRequiredIntermediateSize(texture, 0, static_cast<UINT>(subresources.size()));
	ComPtr<ID3D12Resource> intermediateResource = CreateBufferResource(GraphicsEngine::GetDevice()->Get(), intermediateSize);
	UpdateSubresources(commandlist, texture, intermediateResource.Get(), 0, 0, static_cast<UINT>(subresources.size()), subresources.data());

	// Textureへの転送後は利用できるようにする
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;

	commandlist->ResourceBarrier(1, &barrier);

	return intermediateResource;
}

ComPtr<ID3D12Resource> TextureManager::CreateBufferResource(ID3D12Device* device, size_t sizeInBytes) {

	HRESULT hr;

	// 頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	// 頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	// バッファリソース。テクスチャの場合はまた別の設定をする
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	// リソースのサイズ、今回はVector4を3頂点分
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

std::wstring TextureManager::ConvertString(const std::string& str) {

	if (str.empty()) {
		return std::wstring();
	}

	auto sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), NULL, 0);
	if (sizeNeeded == 0) {
		return std::wstring();
	}
	std::wstring result(sizeNeeded, 0);
	MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), &result[0], sizeNeeded);
	return result;
}

D3D12_GPU_DESCRIPTOR_HANDLE& TextureManager::GetTextureGPUHandle(const std::string textureName) {

	auto it = textures_.find(textureName);
	if (it == textures_.end()) {

		throw std::runtime_error("Texture not found: " + textureName);
	}

	TextureData& textureData = it->second;

	return textureData.gpuHandle;
}

const DirectX::TexMetadata& TextureManager::GetMetaData(const std::string textureName) {

	auto it = textures_.find(textureName);
	if (it == textures_.end()) {

		throw std::runtime_error("Texture not found: " + textureName);
	}

	TextureData& textureData = it->second;

	return textureData.metadata;
}