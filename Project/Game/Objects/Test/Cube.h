#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Object/BaseGameObject.h>

//============================================================================*/
//	Cube class
//============================================================================*/
class Cube :
	public BaseGameObject {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	Cube() = default;
	~Cube() = default;

	void Init(uint32_t index);

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

};