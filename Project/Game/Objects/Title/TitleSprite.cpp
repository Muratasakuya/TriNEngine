#include "TitleSprite.h"

//============================================================================*/
//	TitleSprite classMethod
//============================================================================*/

void TitleSprite::Init() {

	BaseSprite::Init("title");
	BaseSprite::SetSpriteRenderer("titleSprite");

	BaseSprite::SetCenterPos();

}