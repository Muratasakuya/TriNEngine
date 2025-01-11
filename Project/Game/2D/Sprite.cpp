#include "Sprite.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>
#include <Engine/Asset/Asset.h>

//============================================================================*/
//	Sprite classMethods
//============================================================================*/

Sprite::Sprite(const std::string& textureName, Transform2D& transform) {

	textureName_ = textureName;

	vertexBuffer_.data.resize(kSpriteVertexNum);
	vertexBuffer_.CreateVertexBuffer(GraphicsEngine::GetDevice()->Get(), kSpriteVertexNum);

	indexBuffer_.data.resize(kSpriteIndexNum);
	indexBuffer_.CreateIndexBuffer(GraphicsEngine::GetDevice()->Get(), kSpriteIndexNum);

	indexBuffer_.data[0] = 0;
	indexBuffer_.data[1] = 1;
	indexBuffer_.data[2] = 2;
	indexBuffer_.data[3] = 1;
	indexBuffer_.data[4] = 3;
	indexBuffer_.data[5] = 2;

	materialBuffer_.Init();
	transformBuffer_.Init();

	// 本来のテクスチャサイズに合わせる
	SetMetaDataTextureSize(transform);

}

void Sprite::Update(const Transform2D& transform, const Color& color) {

	VertexUpdate(transform);

	vertexBuffer_.Update();
	indexBuffer_.Update();

	materialBuffer_.properties.color = color;
	materialBuffer_.Update();
	transformBuffer_.Update(transform);

}

void Sprite::Draw(BlendMode blendMode) {

	auto commandList = GraphicsEngine::GetCommand()->GetCommandList();

	GraphicsEngine::GetPipeline()->SetRendererPipeline(commandList, Object2D, blendMode);
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->IASetVertexBuffers(0, 1, &vertexBuffer_.GetVertexBuffer());
	commandList->IASetIndexBuffer(&indexBuffer_.GetIndexBuffer());
	materialBuffer_.SetCommand(commandList);
	transformBuffer_.SetCommand(commandList);
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = Asset::GetTexture()->GetTextureGPUHandle(textureName_);
	commandList->SetGraphicsRootDescriptorTable(2, gpuHandle);
	commandList->DrawIndexedInstanced(kSpriteIndexNum, 1, 0, 0, 0);

}

void Sprite::SetMetaDataTextureSize(Transform2D& transform) {

	transform.Init();

	float left = 0.0f - transform.anchorPoint.x;
	float right = 1.0f - transform.anchorPoint.x;
	float top = 0.0f - transform.anchorPoint.y;
	float bottom = 1.0f - transform.anchorPoint.y;

	const DirectX::TexMetadata& metadata = Asset::GetTexture()->GetMetaData(textureName_);

	transform.textureSize = { static_cast<float>(metadata.width) ,static_cast<float>(metadata.height) };
	transform.size = transform.textureSize;

	// 横
	float texLeft = transform.textureLeftTop.x / static_cast<float>(metadata.width);
	float texRight = (transform.textureLeftTop.x + transform.textureSize.x) / static_cast<float>(metadata.width);
	// 縦
	float texTop = transform.textureLeftTop.y / static_cast<float>(metadata.height);
	float texBottom = (transform.textureLeftTop.y + transform.textureSize.y) / static_cast<float>(metadata.height);

	// 左下
	vertexBuffer_.data[0].pos = { left,bottom };
	vertexBuffer_.data[0].texcoord = { texLeft,texBottom };
	// 左上
	vertexBuffer_.data[1].pos = { left,top };
	vertexBuffer_.data[1].texcoord = { texLeft,texTop };
	// 右下
	vertexBuffer_.data[2].pos = { right,bottom };
	vertexBuffer_.data[2].texcoord = { texRight,texBottom };
	// 右上
	vertexBuffer_.data[3].pos = { right,top };
	vertexBuffer_.data[3].texcoord = { texRight,texTop };

}

void Sprite::VertexUpdate(const Transform2D& transform) {

	float left = 0.0f - transform.anchorPoint.x;
	float right = 1.0f - transform.anchorPoint.x;
	float top = 0.0f - transform.anchorPoint.y;
	float bottom = 1.0f - transform.anchorPoint.y;

	const DirectX::TexMetadata& metadata = Asset::GetTexture()->GetMetaData(textureName_);

	// 横
	float texLeft = transform.textureLeftTop.x / static_cast<float>(metadata.width);
	float texRight = (transform.textureLeftTop.x + transform.textureSize.x) / static_cast<float>(metadata.width);
	// 縦
	float texTop = transform.textureLeftTop.y / static_cast<float>(metadata.height);
	float texBottom = (transform.textureLeftTop.y + transform.textureSize.y) / static_cast<float>(metadata.height);

	// 左下
	vertexBuffer_.data[0].pos = { left,bottom };
	vertexBuffer_.data[0].texcoord = { texLeft,texBottom };
	// 左上
	vertexBuffer_.data[1].pos = { left,top };
	vertexBuffer_.data[1].texcoord = { texLeft,texTop };
	// 右下
	vertexBuffer_.data[2].pos = { right,bottom };
	vertexBuffer_.data[2].texcoord = { texRight,texBottom };
	// 右上
	vertexBuffer_.data[3].pos = { right,top };
	vertexBuffer_.data[3].texcoord = { texRight,texTop };

}