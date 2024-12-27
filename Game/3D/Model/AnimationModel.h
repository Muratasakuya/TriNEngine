#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Model/Base/BaseModel.h>
#include <Engine/CBuffer/Transform.h>
#include <Engine/CBuffer/MaterialBuffer.h>
#include <Engine/DXClass/Pipeline/PipelineTypes.h>

//============================================================================*/
//	AnimationModel class
//============================================================================*/
class AnimationModel :
	public BaseModel {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	AnimationModel() = default;
	~AnimationModel() = default;

	void Init(const std::string& modelName, const std::string& animationName);

	void Draw(AnimationTransform transform, MaterialBuffer material, RendererPipelineType pipeline);
	void DrawShadowDepth(AnimationTransform transform);

	//* setter *//

	void SetAnimationName(const std::string& animationName) { animationName_ = animationName; }

	void SetTexture(const std::string& textureName);

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	std::string animationName_;

	//* compute *//

	InputVertexBuffer inputVertices_;
	OutputVertexBuffer outputVertices_;
	// SkinningInfo
	SkinningInfoData skinningInfoDates_;

	//========================================================================*/
	//* functions

	void SetComputeCommands(const std::string& animationName);

};