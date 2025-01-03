#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/2D/Base/BaseSprite.h>

// imgui
#include <imgui.h>

// c++
#include <vector>
#include <unordered_map>

//============================================================================*/
//	SpriteRenderer class
//============================================================================*/
class SpriteRenderer {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	SpriteRenderer() = default;
	~SpriteRenderer() = default;

	static void Render();

	static void SetSprite(BaseSprite* sprite);

	static void EraseSprite(BaseSprite* sprite);

	static void Clear();

	//* imgui *//

	static void SelectSprite();

	static void SelectedImGui();

	//* getter *//

	static BaseSprite* GetSelectedSprite() { return selectedSprite_; };

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//===================================================================*/
	///* variables

	static std::vector<BaseSprite*> sprites_;

	static BaseSprite* selectedSprite_;

	static int currentSpriteIndex_;

};