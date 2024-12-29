#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Object/IBaseGameObject.h>

// c++
#include <memory>

//============================================================================*/
//	BaseAnimationObject class
//============================================================================*/
class BaseAnimationObject :
	public IBaseGameObject {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	BaseAnimationObject() = default;
	virtual ~BaseAnimationObject() = default;

	virtual void Init(const std::string& modelName, const std::string& animationName);

	virtual void Update();

	virtual void Draw(RendererPipelineType pipeline) override;
	virtual void DrawShadowDepth() override;

	void SwitchAnimation(const std::string& animationKey,
		bool loopAnimation,
		float transitionDuration);

	//* imgui *//

	void TransformImGui() override;
	virtual void DerivedImGui() override {}

	//* utility *//

	virtual void ApplyJson() override {};
	virtual void SaveJson() override {};

	//* setter *//

	void SetWorldTransform(const AnimationTransform& transform);

	//* getter *//

	const AnimationTransform& GetWorldTransform() const { return transform_; };

	Vector3 GetWorldPos() const { return transform_.GetWorldPos(); };

protected:
	//===================================================================*/
	//							protected Methods
	//===================================================================*/

	//===================================================================*/
	///* variables

	std::unique_ptr<AnimationModel> model_;

	AnimationTransform transform_;

private:
	//===================================================================*/
	//							private Methods
	//===================================================================*/

	//===================================================================*/
	///* variables

	std::string currentAnimationKey_;
	std::string nextAnimationKey_;

	bool isTransitioning_ = false;

};