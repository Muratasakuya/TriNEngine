#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Model/Base/BaseModel.h>
#include <Engine/CBuffer/Transform.h>
#include <Engine/CBuffer/MaterialBuffer.h>
#include <Engine/DXClass/Pipeline/PipelineTypes.h>

// c++
#include <vector>

//============================================================================*/
//	Model class
//============================================================================*/
class Model :
	public BaseModel {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	Model() = default;
	~Model() = default;

	void Draw(WorldTransform transform, std::vector<MaterialBuffer>& materials, RendererPipelineType pipeline);

	void DrawShadowDepth(WorldTransform transform);

	//* getter *//

	size_t GetMeshNum() const { return meshNum_; }

	//* setter *//

	void SetTexture(const std::string& textureName);

};