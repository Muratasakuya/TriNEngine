#include "SceneTransition.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Game/Utility/GameTimer.h>
#include <Lib/Adapter/Easing.h>

//============================================================================*/
//	SceneTransition classMethods
//============================================================================*/

void SceneTransition::Init() {

	BaseSprite::Init("transitionWhite");
	BaseSprite::SetSpriteRenderer("sceneTransition");

	transform.anchorPoint = Vector2(0.0f, 0.0f);
	color = Color(0.12f, 0.12f, 0.12f, 0.0f);

	drawType_ = SpriteDrawType::Transition;

	isTransition_ = false;
	isBeginTransitionFinished_ = false;

	state_ = TransitionState::Begin;

	// Out
	fadeOutTimer_.current = 0.0f;
	fadeOutTimer_.target = 2.0f;
	// In
	fadeInTimer_.current = 0.0f;
	fadeInTimer_.target = 4.0f;

	// Wait
	waitTimer_.current = 0.0f;
	waitTimer_.target = 1.0f;

}

void SceneTransition::Update() {

	BaseSprite::Update();

	if (!isTransition_) {
		return;
	}

	switch (state_) {
	case SceneTransition::TransitionState::Begin: {

		FadeOut();

		break;
	}
	case SceneTransition::TransitionState::Wait: {

		Wait();

		break;
	}
	case SceneTransition::TransitionState::End: {

		FadeIn();

		break;
	}
	}

}

void SceneTransition::FadeOut() {

	fadeOutTimer_.current += GameTimer::GetDeltaTime();
	float t = fadeOutTimer_.current / fadeOutTimer_.target;
	float easedT = EaseOutExpo(t);

	// 0.0f -> 1.0f
	color.a = std::lerp(0.0f, 1.0f, easedT);
	color.a = std::clamp(color.a, 0.0f, 1.0f);

	if (t > 1.0f) {

		// 次に進める
		state_ = TransitionState::Wait;
		fadeOutTimer_.current = 0.0f;
	}

}

void SceneTransition::Wait() {

	waitTimer_.current += GameTimer::GetDeltaTime();

	if (waitTimer_.current > waitTimer_.target) {

		// 次に進める
		state_ = TransitionState::End;
		waitTimer_.current = 0.0F;

		// SceneManagerに次に進めるフラグを通達
		isBeginTransitionFinished_ = true;
	}

}

void SceneTransition::FadeIn() {

	fadeInTimer_.current += GameTimer::GetDeltaTime();
	float t = fadeInTimer_.current / fadeInTimer_.target;
	float easedT = EaseInCirc(t);

	// 1.0f -> 0.0f
	color.a = std::lerp(1.0f, 0.0f, easedT);
	color.a = std::clamp(color.a, 0.0f, 1.0f);

	if (t > 1.0f) {

		// 遷移終了
		state_ = TransitionState::Begin;
		fadeInTimer_.current = 0.0f;

		isTransition_ = false;
		color.a = 0.0f;
	}
}