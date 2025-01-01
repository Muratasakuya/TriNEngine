#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Object/BaseGameObject.h>
#include <Game/3D/Collision/Collider.h>

//============================================================================*/
//	Enemy class
//============================================================================*/
class Enemy :
	public BaseGameObject, Collider {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	Enemy() = default;
	~Enemy() = default;

	void Init(uint32_t index);

	void Update();

	void Draw(RendererPipelineType pipeline) override;

	//* collision *//

	void OnCollisionEnter([[maybe_unused]] Collider* other) override;
	
	//* getter *//

	bool IsAlive() const { return isAlive_; }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	bool isAlive_;

};