#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/2D/Base/BaseSprite.h>

//============================================================================*/
//	PlayerOperatre class
//============================================================================*/
class PlayerOperatre
	:public BaseSprite {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	PlayerOperatre() = default;
	~PlayerOperatre() = default;

	void Init();

};