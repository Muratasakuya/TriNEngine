#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/2D/Sprite.h>

// c++
#include <memory>
#include <string>

//============================================================================*/
//	BaseSprite class
//============================================================================*/
class BaseSprite {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	BaseSprite() = default;
	~BaseSprite();

	void Init(const std::string& textureName);

	void Update();
		
	void Draw();

	virtual void ImGui();
	virtual void DerivedImGui() {};

	//* setter *//

	void SetSpriteRenderer(const std::string& name);

	void SetPos(const Vector2& pos) { transform.pos = pos; }

	void SetSize(const Vector2& size) { transform.size = size; }
	void SetTextureSize(const Vector2& textureSize) { transform.textureSize = textureSize; }
	void SetTextureLeftTop(const Vector2& textureLeftTop) { transform.textureLeftTop = textureLeftTop; }

	//* getter *//

	std::string GetName() const { return name_; }

	Vector2 GetTextureSize() const;

protected:
	//===================================================================*/
	//							protected Methods
	//===================================================================*/

	//===================================================================*/
	///* variables

	Transform2D transform;

	Color color;

private:
	//===================================================================*/
	//							  private Methods
	//===================================================================*/

	std::unique_ptr<Sprite> sprite;

	std::string name_ = "sprite";
	std::string textureName_;

};