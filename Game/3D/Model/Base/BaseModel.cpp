#include "BaseModel.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Asset/Asset.h>

//============================================================================*/
//	BaseModel classMethod
//============================================================================*/

void BaseModel::Init(const std::string& modelName) {

	modelData_ = Asset::GetModel()->GetModelData(modelName);
	meshNum_ = modelData_.meshes.size();

	for (uint32_t index = 0; index < meshNum_; ++index) {

		inputAssembler_.Init(modelData_.meshes[index]);
	}
}