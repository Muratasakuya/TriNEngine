#include "PlayerOperatre.h"

//============================================================================*/
//	PlayerOperatre classMethods
//============================================================================*/

void PlayerOperatre::Init() {

	BaseSprite::Init("operation");
	BaseSprite::SetSpriteRenderer("operation");

	transform.size = Vector2(160.0f, 160.0f);
	transform.pos = Vector2(1142.0f, 620.0f);

}