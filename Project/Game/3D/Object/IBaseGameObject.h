#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Model/Model.h>
#include <Game/3D/Model/AnimationModel.h>
#include <Engine/DXClass/Pipeline/PipelineTypes.h>

//============================================================================*/
//	Structure
//============================================================================*/
struct UVTransform {

	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

//============================================================================*/
//	IBaseGameObject class
//============================================================================*/
class IBaseGameObject {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	IBaseGameObject() = default;
	virtual ~IBaseGameObject();

	virtual void Draw(RendererPipelineType pipeline) = 0;
	virtual void DrawShadowDepth() = 0;

	//* imgui *//

	virtual void ImGui();
	virtual void TransformImGui() = 0;
	virtual void DerivedImGui() = 0;

	//* utility *//

	virtual void ApplyJsonForColor();
	virtual void SaveJsonForColor();

	virtual void ApplyJsonForTransform(BaseTransform& transform, const std::optional<std::string>& name = std::nullopt);
	virtual void SaveJsonForTransform(const BaseTransform& transform);

	//* Json *//

	virtual void ApplyJson() = 0;
	virtual void SaveJson() = 0;

	//* setter *//

	void SetMeshRenderer(const std::string& name, uint32_t index = 0);

	void SetUVTransform(const UVTransform& uvTransform);

	void SetLightingEnable(bool enable);
	void SetBlinnPhongLightingEnable(bool enable);

	//* getter *//

	std::string GetName() const { return name_; };

	bool GetDrawEnable() const { return drawEnable_; };

	bool GetDrawShadowEnable() const { return drawDepthShadowEnable_; };

protected:
	//===================================================================*/
	//							protected Methods
	//===================================================================*/

	//===================================================================*/
	///* variables

	std::optional<std::string> parentFolderName_;

	std::vector<MaterialBuffer> materials_;

	bool drawEnable_ = true;            //* 描画を行うかどうか
	bool drawDepthShadowEnable_ = true; //* 深度に書き込むかどうか

private:
	//===================================================================*/
	//							  private Methods
	//===================================================================*/

	std::string name_ = "object";

};