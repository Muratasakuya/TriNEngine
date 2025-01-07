#include "TitleSprite.h"

//============================================================================*/
//	TitleSprite classMethod
//============================================================================*/

void TitleSprite::Init() {

	BaseSprite::Init("Title");
	BaseSprite::SetSpriteRenderer("titleSprite");

	BaseSprite::SetCenterPos();

}