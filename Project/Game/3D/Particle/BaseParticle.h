#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Particle/ParticleTraits.h>
#include <Game/3D/Particle/IBaseParticle.h>
#include <Game/3D/Particle/ParticleSystem.h>
#include <Game/Utility/GameTimer.h>

// c++
#include <memory>

// external
#include <imgui.h>

//============================================================================*/
//	BaseParticle class
//============================================================================*/
template <ParticleType particleType>
class BaseParticle
	: public IBaseParticle {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	using ParameterType = typename ParticleTraits<particleType>::ParameterType;

	BaseParticle() = default;
	virtual ~BaseParticle() = default;

	virtual void Init(const std::string& particleName, const std::string modelName,
		const std::string textureName, ParticleRenderTarget renderTarget) override;
	virtual void Update() override;

	virtual void Create(const std::string& modelName, ParticleRenderTarget renderTarget);

	virtual void Draw() override;

	virtual void ImGui() override;
	virtual void DerivedImGui() {};

	//* particleEmitMethods *//

	// 任意のタイミングで呼び出して発生
	virtual void Emit();
	// 一定時間置きに発生させる
	virtual void UpdateFrequencyEmit();

protected:
	//========================================================================*/
	//	protected Methods
	//========================================================================*/

	//========================================================================*/
	///* variables

	ParameterType parameter_;
	ParticleType particleType_ = particleType;

	std::unique_ptr<ParticleSystem> particleSystem_;

	BlendMode blendMode_ = BlendMode::kBlendModeNormal;

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	///* variables

	bool frequencyEmitEnable_ = false;

};

//============================================================================*/
//	BaseParticle classMethods
//============================================================================*/

template<ParticleType particleType>
inline void BaseParticle<particleType>::Init(const std::string& particleName, const std::string modelName,
	const std::string textureName, ParticleRenderTarget renderTarget) {

	SetParticleRenderer(particleName, renderTarget);
	Create(modelName, renderTarget);

	particleSystem_->SetTexture(GetName(), textureName);
}

template<ParticleType particleType>
inline void BaseParticle<particleType>::Update() {

	if (frequencyEmitEnable_) {
		UpdateFrequencyEmit();
	}

	particleSystem_->Update();
}

template<ParticleType particleType>
inline void BaseParticle<particleType>::Create(const std::string& modelName, ParticleRenderTarget renderTarget) {

	particleSystem_ = std::make_unique<ParticleSystem>();
	particleSystem_->CreateParticle(modelName, IBaseParticle::GetName(), particleType_, parameter_, renderTarget);

}

template<ParticleType particleType>
inline void BaseParticle<particleType>::Draw() {

	particleSystem_->Draw(IBaseParticle::GetName(), blendMode_);
}

template<ParticleType particleType>
inline void BaseParticle<particleType>::Emit() {

	particleSystem_->EmitParticle(IBaseParticle::GetName(), particleType_, parameter_);
}

template<ParticleType particleType>
inline void BaseParticle<particleType>::UpdateFrequencyEmit() {

	float deltaTime;
	if (parameter_.isUseScaledDeltaTime) {
		deltaTime = GameTimer::GetScaledDeltaTime();
	} else {
		deltaTime = GameTimer::GetDeltaTime();
	}
	parameter_.frequencyTime += deltaTime;

	if (parameter_.frequency <= parameter_.frequencyTime) {

		particleSystem_->EmitParticle(IBaseParticle::GetName(), particleType_, parameter_);
		parameter_.frequencyTime -= parameter_.frequency;
	}
}

template<ParticleType particleType>
inline void BaseParticle<particleType>::ImGui() {

	// Emit
	if (ImGui::Button("Emit")) {
		this->Emit();
	}

	// Transform
	if (ImGui::TreeNode("Transform")) {

		ImGui::PushItemWidth(144.0f);
		if (parameter_.isUniform) {

			ImGui::DragFloat3("scale", &parameter_.scale.uniform.x, 0.01f);
		} else {

			ImGui::DragFloat3("scaleMin", &parameter_.scale.min.x, 0.01f);
			ImGui::DragFloat3("scaleMax", &parameter_.scale.max.x, 0.01f);
		}
		ImGui::DragFloat3("translate", &parameter_.aabb.center.x, 0.01f);
		ImGui::DragFloat3("extent", &parameter_.aabb.extent.x, 0.01f);
		ImGui::PopItemWidth();

		ImGui::TreePop();
	}

	// Flags
	if (ImGui::TreeNode("Flags")) {

		ImGui::Checkbox("frequencyEmit", &frequencyEmitEnable_);
		ImGui::Checkbox("uniform", &parameter_.isUniform);
		ImGui::Checkbox("useScaledDeltaTime", &parameter_.isUseScaledDeltaTime);
		ImGui::Checkbox("useBillboard", &parameter_.isUseBillboard);
		ImGui::Checkbox("moveToDirection", &parameter_.moveToDirection);

		ImGui::TreePop();
	}

	// Parameters
	if (ImGui::TreeNode("Parameters")) {

		// Count
		ImGui::PushItemWidth(128.0f);
		ImGui::DragInt("emitCount", reinterpret_cast<int*>(&parameter_.count), 0, 128);
		// Frequency
		ImGui::DragFloat("frequency", &parameter_.frequency, 0.1f, 0.0f, 10.0f);
		ImGui::PopItemWidth();

		bool hasColor = parameter_.color.has_value();
		if (ImGui::Checkbox("hasColor", &hasColor)) {
			if (hasColor) {
				parameter_.color = Color(1.0f, 1.0f, 1.0f, 1.0f);
			} else {
				parameter_.color = std::nullopt;
			}
		}
		if (parameter_.color.has_value()) {
			ImGui::ColorEdit4("color", &parameter_.color.value().r);
		}

		bool haslifeTime = parameter_.lifeTime.has_value();
		if (ImGui::Checkbox("hasLifeTime", &haslifeTime)) {
			if (haslifeTime) {
				parameter_.lifeTime = 2.4f;
			} else {
				parameter_.lifeTime = std::nullopt;
			}
		}
		if (parameter_.lifeTime.has_value()) {
			ImGui::DragFloat("lifeTime", &parameter_.lifeTime.value(), 0.1f, 0.0f, 10.0f);
		}

		bool hasSpeed = parameter_.speed.has_value();
		if (ImGui::Checkbox("hasSpeed", &hasSpeed)) {
			if (hasSpeed) {
				parameter_.speed = 1.0f;
			} else {
				parameter_.speed = std::nullopt;
			}
		}
		if (parameter_.speed.has_value()) {
			ImGui::DragFloat("speed", &parameter_.speed.value(), 0.1f, 0.0f, 10.0f);
		}

		bool hasEasingType = parameter_.easingType.has_value();
		if (ImGui::Checkbox("hasEasing", &hasEasingType)) {
			if (hasEasingType) {
				parameter_.easingType = EasingType::EaseInSine;
			} else {
				parameter_.easingType = std::nullopt;
			}
		}

		// BlendMode
		const char* blendModeOptions[] = {
			"None","Normal","Add","Subtract","Multiply","Screen"
		};
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f)); // 内部の余白調整
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 4.0f));  // アイテム間隔調整

		int blendModeIndex = static_cast<int>(blendMode_);

		if (ImGui::BeginCombo("BlendMode", blendModeOptions[blendModeIndex])) {
			for (int index = 0; index < IM_ARRAYSIZE(blendModeOptions); index++) {

				const bool isSelected = (blendModeIndex == index);
				if (ImGui::Selectable(blendModeOptions[index], isSelected)) {

					blendMode_ = static_cast<BlendMode>(index);
					blendModeIndex = index;
				}
				if (isSelected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		ImGui::PopStyleVar(2);

		const char* easingOptions[] = {
			"EaseInSine", "EaseInQuad", "EaseInCubic", "EaseInQuart", "EaseInQuint", "EaseInExpo", "EaseInCirc", "EaseInBack", "EaseInBounce",
			"EaseOutSine", "EaseOutQuad", "EaseOutCubic", "EaseOutQuart", "EaseOutQuint", "EaseOutExpo", "EaseOutCirc", "EaseOutBack", "EaseOutBounce",
			"EaseInOutSine", "EaseInOutQuad", "EaseInOutCubic", "EaseInOutQuart", "EaseInOutQuint", "EaseInOutExpo", "EaseInOutCirc", "EaseInOutBounce"
		};

		// Enum values
		const char* easeInOptions[] = {
			"EaseInSine", "EaseInQuad", "EaseInCubic", "EaseInQuart", "EaseInQuint", "EaseInExpo", "EaseInCirc", "EaseInBack", "EaseInBounce"
		};
		const char* easeOutOptions[] = {
			"EaseOutSine", "EaseOutQuad", "EaseOutCubic", "EaseOutQuart", "EaseOutQuint", "EaseOutExpo", "EaseOutCirc", "EaseOutBack", "EaseOutBounce"
		};
		const char* easeInOutOptions[] = {
			"EaseInOutSine", "EaseInOutQuad", "EaseInOutCubic", "EaseInOutQuart", "EaseInOutQuint", "EaseInOutExpo", "EaseInOutCirc", "EaseInOutBounce"
		};

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f)); // 内部の余白調整
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 4.0f));  // アイテム間隔調整
		ImGui::PushItemWidth(128.0f);
		if (hasEasingType) {
			int easingIndex = static_cast<int>(*parameter_.easingType);

			if (parameter_.easingType.has_value()) {
				if (ImGui::BeginCombo("EasingType", easingOptions[easingIndex])) {

					// EaseIn
					if (ImGui::BeginCombo("EaseIn", "")) {
						for (int i = 0; i < IM_ARRAYSIZE(easeInOptions); i++) {
							const bool isSelected = (easingIndex == i);
							if (ImGui::Selectable(easeInOptions[i], isSelected)) {
								parameter_.easingType = static_cast<EasingType>(i);
								easingIndex = i;
							}
							if (isSelected) ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}

					// EaseOut
					if (ImGui::BeginCombo("EaseOut", "")) {
						for (int i = 0; i < IM_ARRAYSIZE(easeOutOptions); i++) {
							const bool isSelected = (easingIndex == i + IM_ARRAYSIZE(easeInOptions)); // adjust index
							if (ImGui::Selectable(easeOutOptions[i], isSelected)) {
								parameter_.easingType = static_cast<EasingType>(i + IM_ARRAYSIZE(easeInOptions));
								easingIndex = i + IM_ARRAYSIZE(easeInOptions);
							}
							if (isSelected) ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}

					// EaseInOut
					if (ImGui::BeginCombo("EaseInOut", "")) {
						for (int i = 0; i < IM_ARRAYSIZE(easeInOutOptions); i++) {
							const bool isSelected = (easingIndex == i + IM_ARRAYSIZE(easeInOptions) + IM_ARRAYSIZE(easeOutOptions)); // adjust index
							if (ImGui::Selectable(easeInOutOptions[i], isSelected)) {
								parameter_.easingType = static_cast<EasingType>(i + IM_ARRAYSIZE(easeInOptions) + IM_ARRAYSIZE(easeOutOptions));
								easingIndex = i + IM_ARRAYSIZE(easeInOptions) + IM_ARRAYSIZE(easeOutOptions);
							}
							if (isSelected) ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}

					ImGui::EndCombo();
				}
			}
		}
		ImGui::PopStyleVar(2);
		ImGui::PopItemWidth();

		ImGui::TreePop();
	}

}