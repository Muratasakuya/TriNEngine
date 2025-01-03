#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/Pipeline/PipelineTypes.h>
#include <Engine/CBuffer/VertexBuffer.h>
#include <Engine/CBuffer/MaterialBuffer.h>
#include <Engine/CBuffer/Transform.h>
#include <Lib/MathUtils/Vector2.h>

// c++
#include <string>

// 頂点数 Vertex
const constexpr UINT kSpriteVertexNum = 4;
// 頂点数 Index
const constexpr UINT kSpriteIndexNum = 6;

//============================================================================*/
//	Sprite class
//============================================================================*/
class Sprite {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	Sprite(const std::string& textureName, Transform2D& transform);
	~Sprite() = default;

	void Update(const Transform2D& transform, const Color& color);

	void Draw(BlendMode blendMode = BlendMode::kBlendModeNormal);

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	std::string textureName_;

	//* buffer *//

	VertexBuffer<SpriteVertexData> vertexBuffer_;
	IndexBuffer indexBuffer_;

	SpriteMaterialBuffer materialBuffer_;
	SpriteTransformBuffer transformBuffer_;

	//========================================================================*/
	//* function

	void SetMetaDataTextureSize(Transform2D& transform);
	void VertexUpdate(const Transform2D& transform);

};