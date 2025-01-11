#include "TimeLimit.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Game/Objects/Enemy/Manager/EnemyManager.h>
#include <Game/Utility/GameTimer.h>
#include "Lib/Adapter/JsonAdapter.h"

//============================================================================*/
//	TimeNumber classMethods
//============================================================================*/

void TimeNumber::Init(uint32_t index) {

	BaseSprite::Init("timeNumber");
	BaseSprite::SetSpriteRenderer("timeNumber" + std::to_string(index));
}

//============================================================================*/
//	TimeCoron class
//============================================================================*/

void TimeCoron::Init() {

	BaseSprite::Init("coron");
	BaseSprite::SetSpriteRenderer("coron");
}

//============================================================================*/
//	TimeLimit classMethods
//============================================================================*/

void TimeLimit::Init(EnemyManager* enemyManager) {

	enemyManager_ = enemyManager;

	BaseEditor::SetEditor("TimeLimit");

	const size_t timeNum = 3;
	timeNumbers_.resize(timeNum);
	timeNumberPositions_.resize(timeNum);
	for (uint32_t index = 0; index < timeNum; ++index) {

		timeNumbers_[index] = std::make_unique<TimeNumber>();
		timeNumbers_[index]->Init(index);
	}

	timeCoron_ = std::make_unique<TimeCoron>();
	timeCoron_->Init();

	accumulatedTime_ = 0.0f;

	isFinish_ = false;
	isCountStart_ = false;

	waitTimer_ = 0.0f;
	waitTime_ = 4.0f;

	ApplyJson();

}

void TimeLimit::Update() {

	if (isFinish_) {
		return;
	}

	if (!isCountStart_ && enemyManager_->IsStart()) {

		waitTimer_ += GameTimer::GetDeltaTime();
		if (waitTimer_ >= waitTime_) {

			isCountStart_ = true;
		}
	}

	if (isCountStart_) {
		accumulatedTime_ += GameTimer::GetDeltaTime();
		if (accumulatedTime_ >= 1.0f) {

			++time_;
			accumulatedTime_ = 0.0f;
		}
	}

	// 1文字分のテクスチャサイズ
	const float texWidth = 544.0f / 10.0f; // 画像の幅544px..10で割って1文字分を求める
	const float texHeight = 64.0f;         // 画像の高さ64px

	// 分と秒を計算
	int minutes = (timeLimit_ - time_) / 60;
	int seconds = (timeLimit_ - time_) % 60;

	// 分
	Vector2 texBaseMin = { texWidth * minutes, 0.0f };
	timeNumbers_[0]->SetTextureLeftTop(texBaseMin);

	// 秒の10の位
	int tens = seconds / 10;
	Vector2 texBaseTens = { texWidth * tens, 0.0f };
	timeNumbers_[1]->SetTextureLeftTop(texBaseTens);

	// 秒の1の位
	int ones = seconds % 10;
	Vector2 texBaseOnes = { texWidth * ones, 0.0f };
	timeNumbers_[2]->SetTextureLeftTop(texBaseOnes);

	for (const auto& timeNumber : timeNumbers_) {

		// サイズ設定
		timeNumber->SetSize(Vector2(texWidth, texHeight) / 2.0f);
		timeNumber->SetTextureSize(Vector2(texWidth, texHeight));

		// 更新
		timeNumber->Update();
	}

	timeCoron_->SetSize(timeCoron_->GetTextureSize() / 2.0f);
	timeCoron_->Update();

	if (time_ == timeLimit_) {
		isFinish_ = true;
	}

}

void TimeLimit::ImGui() {

	if (ImGui::Button("Save")) {
		SaveJson();
	}

	ImGui::DragInt("timeLimit", &timeLimit_);
	ImGui::Text("time: %d", time_);

	int index = 0;
	for (auto& pos : timeNumberPositions_) {

		std::string posLebel = "timePos" + std::to_string(index);
		if (ImGui::DragFloat2(posLebel.c_str(), &pos.x)) {

			timeNumbers_[index]->SetPos(pos);
		}
		++index;
	}
	if (ImGui::DragFloat2("timeCoronPos", &timeCoronPos_.x)) {

		timeCoron_->SetPos(timeCoronPos_);
	}

}

void TimeLimit::ApplyJson() {

	Json data = JsonAdapter::Load("HUD/TimeParameter.json");

	timeLimit_ = data["timeLimit"];
	timeNumberPositions_[0] = JsonAdapter::ToVector2(data["timePos_0"]);
	timeNumberPositions_[1] = JsonAdapter::ToVector2(data["timePos_1"]);
	timeNumberPositions_[2] = JsonAdapter::ToVector2(data["timePos_2"]);
	timeCoronPos_ = JsonAdapter::ToVector2(data["timeCoronPos"]);

	int index = 0;
	for (auto& pos : timeNumberPositions_) {

		timeNumbers_[index]->SetPos(pos);
		++index;
	}
	timeCoron_->SetPos(timeCoronPos_);

}

void TimeLimit::SaveJson() {

	Json data;

	data["timeLimit"] = timeLimit_;
	data["timePos_0"] = JsonAdapter::FromVector2(timeNumberPositions_[0]);
	data["timePos_1"] = JsonAdapter::FromVector2(timeNumberPositions_[1]);
	data["timePos_2"] = JsonAdapter::FromVector2(timeNumberPositions_[2]);
	data["timeCoronPos"] = JsonAdapter::FromVector2(timeCoronPos_);

	JsonAdapter::Save("HUD/TimeParameter.json", data);

}