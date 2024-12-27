#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/Scenes/Methods/IScene.h>

//============================================================================*/
//	TitleScene class
//============================================================================*/
class TitleScene :
	public IScene {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	TitleScene() = default;
	~TitleScene() = default;

	void Init() override;

	void Update() override;

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

};