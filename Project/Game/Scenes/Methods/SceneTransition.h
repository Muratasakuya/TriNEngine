#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/2D/Base/BaseSprite.h>

//============================================================================*/
//	SceneTransition class
//============================================================================*/
// Fadeのみ実装
class SceneTransition :
	public BaseSprite {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	SceneTransition() = default;
	~SceneTransition() = default;

	void Init();

	void Update();

	//* setter *//

	void SetTransition() { isTransition_ = true; }
	void SetResetBeginTransition() { isBeginTransitionFinished_ = false; }

	//* getter *//

	bool IsTransition() const { return isTransition_; }

	bool IsBeginTransitionFinished() const { return isBeginTransitionFinished_; }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//===================================================================*/
	///* enum class structure

	// 遷移状態
	enum class TransitionState {

		Begin,
		Wait,
		End
	};

	// 時間
	struct Timer {

		float current;
		float target;
	};

	//===================================================================*/
	///* variables

	bool isTransition_; //* 遷移フラグ

	bool isBeginTransitionFinished_; //* 次のSceneに進めるフラグ

	TransitionState state_; //* 遷移状態

	//* フェードに使うタイマー
	Timer fadeOutTimer_;
	Timer fadeInTimer_;

	Timer waitTimer_; //* 待機時間に使うタイマー

	//===================================================================*/
	///* functions

	void FadeOut(); //* start

	void Wait();    //* wait

	void FadeIn();  //* end

};