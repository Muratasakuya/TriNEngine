#include "Cube.h"

//============================================================================*/
//	Cube classMethod
//============================================================================*/

void Cube::Init(uint32_t index) {

	BaseGameObject::Init("cube.obj");

	BaseGameObject::SetMeshRenderer("cube", index);

}