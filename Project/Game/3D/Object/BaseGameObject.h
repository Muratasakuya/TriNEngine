#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Object/IBaseGameObject.h>

// c++
#include <cstdint>
#include <memory>
#include <numbers>

//============================================================================*/
//	BaseGameObject class
//============================================================================*/
class BaseGameObject :
	public IBaseGameObject {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	BaseGameObject() = default;
	virtual ~BaseGameObject() = default;

	virtual void Init(const std::string& modelName);

	virtual void Update();

	virtual void Draw(RendererPipelineType pipeline) override;
	virtual void DrawShadowDepth() override;

	//* imgui *//

	void TransformImGui() override;
	virtual void DerivedImGui() override {}

	void KeyRotate(); //* QuaternionがGuiで操作できないのでこれだけKeyで行う

	//* utility *//

	virtual void ApplyJson() override {};
	virtual void SaveJson() override {};

	//* getter *//

	const WorldTransform& GetWorldTransform() const { return transform_; };

	Vector3 GetWorldPos() const { return transform_.GetWorldPos(); };

	Vector3 GetTranslation() const { return transform_.translation; };
	Quaternion GetRotation() const { return transform_.rotation; };

	//* setter *//

	void SetWorldTransform(const WorldTransform& transform);
	void SetParent(const WorldTransform& transform);

	void SetTranslate(const Vector3& translate);
	void SetRotate(const Quaternion& rotate);

protected:
	//===================================================================*/
	//							protected Methods
	//===================================================================*/

	//===================================================================*/
	///* variables

	std::unique_ptr<Model> model_;

	WorldTransform transform_;

	//* uv *//

	UVTransform uvTransform;

};