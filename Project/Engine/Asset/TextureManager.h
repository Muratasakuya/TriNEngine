#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/ComPtr.h>

// directX
#include <d3d12.h>
#include "Externals/DirectXTex/DirectXTex.h"
#include "Externals/DirectXTex/d3dx12.h"

// c++
#include <string>
#include <unordered_map>
#include <filesystem>

//============================================================================*/
//	TextureManager class
//============================================================================*/
class TextureManager {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	TextureManager() = default;
	~TextureManager() = default;

	void Load(const std::string& textureName);

	//* getter *//

	D3D12_GPU_DESCRIPTOR_HANDLE& GetTextureGPUHandle(const std::string textureName);

	const DirectX::TexMetadata& GetMetaData(const std::string textureName);

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* structure

	struct TextureData {

		ComPtr<ID3D12Resource> resource;
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
		DirectX::TexMetadata metadata;
		uint32_t srvIndex;
	};

	//========================================================================*/
	//* variables

	std::unordered_map<std::string, TextureData> textures_;

	//========================================================================*/
	//* functions

	DirectX::ScratchImage GenerateMipMaps(const std::string& filePath);

	ComPtr<ID3D12Resource> CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);
	ComPtr<ID3D12Resource> UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

	ComPtr<ID3D12Resource> CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);
	std::wstring ConvertString(const std::string& str);

};