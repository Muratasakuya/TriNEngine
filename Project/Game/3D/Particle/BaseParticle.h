#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Particle/ParticleTraits.h>
#include <Game/3D/Particle/IBaseParticle.h>
#include <Game/3D/Particle/ParticleSystem.h>

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

	virtual void Init() override = 0;
	virtual void Update() override = 0;

	virtual void Create(const std::string& modelName);

	virtual void Draw() override;

	virtual void ImGui() override;
	virtual void DerivedImGui() {};

	//* particleEmitMethods *//

	// 任意のタイミングで呼び出して発生
	virtual void Emit();
	// 一定時間置きに発生させる
	virtual void UpdateFrequencyEmit(float deltaTime);

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

};

//============================================================================*/
//	BaseParticle classMethods
//============================================================================*/

template<ParticleType particleType>
inline void BaseParticle<particleType>::Create(const std::string& modelName) {

	particleSystem_ = std::make_unique<ParticleSystem>();
	particleSystem_->CreateParticle(modelName, IBaseParticle::GetName(), particleType_, parameter_);

}

template<ParticleType particleType>
inline void BaseParticle<particleType>::Draw() {

	particleSystem_->Draw(IBaseParticle::GetName(), blendMode_);

}

template<ParticleType particleType>
inline void BaseParticle<particleType>::ImGui() {

	uintptr_t ptrAddress = reinterpret_cast<uintptr_t>(this);

	// Emit
	if (ImGui::Button(("Emit##" + std::to_string(ptrAddress)).c_str())) {
		this->Emit();
	}

	// Transform
	if (ImGui::CollapsingHeader("Transform")) {

		if (parameter_.isUniform) {

			ImGui::DragFloat3(("Scale##" + std::to_string(ptrAddress)).c_str(), &parameter_.scale.uniform.x, 0.01f);
		} else {

			ImGui::DragFloat3(("ScaleMin##" + std::to_string(ptrAddress)).c_str(), &parameter_.scale.min.x, 0.01f);
			if (parameter_.scale.min.x >= parameter_.scale.max.x) {
				parameter_.scale.min.x = parameter_.scale.max.x - 0.01f;
			}
			if (parameter_.scale.min.y >= parameter_.scale.max.y) {
				parameter_.scale.min.y = parameter_.scale.max.y - 0.01f;
			}
			if (parameter_.scale.min.z >= parameter_.scale.max.z) {
				parameter_.scale.min.z = parameter_.scale.max.z - 0.01f;
			}

			ImGui::DragFloat3(("ScaleMax##" + std::to_string(ptrAddress)).c_str(), &parameter_.scale.max.x, 0.01f);
		}
	}

	// Particle Settings
	if (ImGui::CollapsingHeader("Particle Settings")) {

		// Uniform
		ImGui::Checkbox(("Uniform##" + std::to_string(ptrAddress)).c_str(), &parameter_.isUniform);

		// LifeTime (optional)
		bool haslifeTime = parameter_.lifeTime.has_value();
		if (ImGui::Checkbox(("Has LifeTime##" + std::to_string(ptrAddress)).c_str(), &haslifeTime)) {
			if (haslifeTime) {
				parameter_.lifeTime = 1.0f;
			} else {
				parameter_.lifeTime = std::nullopt;
			}
		}
		if (parameter_.lifeTime.has_value()) {
			ImGui::DragFloat(("LifeTime##" + std::to_string(ptrAddress)).c_str(), &parameter_.lifeTime.value(), 0.1f, 0.0f, 10.0f);
		}

		// Speed (optional)
		bool hasSpeed = parameter_.speed.has_value();
		if (ImGui::Checkbox(("Has Speed##" + std::to_string(ptrAddress)).c_str(), &hasSpeed)) {
			if (hasSpeed) {
				parameter_.speed = 1.0f;
			} else {
				parameter_.speed = std::nullopt;
			}
		}
		if (parameter_.speed.has_value()) {
			ImGui::DragFloat(("Speed##" + std::to_string(ptrAddress)).c_str(), &parameter_.speed.value(), 0.1f, 0.0f, 10.0f);
		}

		// Color (optional)
		bool hasColor = parameter_.color.has_value();
		if (ImGui::Checkbox(("Has Color##" + std::to_string(ptrAddress)).c_str(), &hasColor)) {
			if (hasColor) {
				parameter_.color = Color(1.0f, 1.0f, 1.0f, 1.0f);
			} else {
				parameter_.color = std::nullopt;
			}
		}
		if (parameter_.color.has_value()) {
			ImGui::ColorEdit4(("Color##" + std::to_string(ptrAddress)).c_str(), &parameter_.color.value().r);
		}

		// Count
		ImGui::DragInt(("Count##" + std::to_string(ptrAddress)).c_str(), reinterpret_cast<int*>(&parameter_.count), 0, 128);

		// Frequency
		ImGui::DragFloat(("Frequency##" + std::to_string(ptrAddress)).c_str(), &parameter_.frequency, 0.1f, 0.0f, 10.0f);

		// Easing Type
		bool hasEasingType = parameter_.easingType.has_value();
		if (ImGui::Checkbox(("Has Easing##" + std::to_string(ptrAddress)).c_str(), &hasEasingType)) {
			if (hasEasingType) {
				parameter_.easingType = EasingType::EaseInSine;
			} else {
				parameter_.easingType = std::nullopt;
			}
		}

		// Easing Type Selection
		if (parameter_.easingType.has_value()) {
			int easingIndex = static_cast<int>(*parameter_.easingType);

			const char* easingOptions[] = {
				"EaseInSine", "EaseInQuad", "EaseInCubic", "EaseInQuart", "EaseInQuint", "EaseInExpo", "EaseInCirc",
				"EaseOutSine", "EaseOutQuad", "EaseOutCubic", "EaseOutQuart", "EaseOutQuint", "EaseOutExpo", "EaseOutCirc",
				"EaseInOutSine", "EaseInOutQuad", "EaseInOutCubic", "EaseInOutQuart", "EaseInOutQuint", "EaseInOutExpo", "EaseInOutCirc"
			};

			if (ImGui::CollapsingHeader("Easing Type")) {
				if (ImGui::BeginCombo(("Easing Type##" + std::to_string(ptrAddress)).c_str(), easingOptions[easingIndex])) {
					for (int i = 0; i < IM_ARRAYSIZE(easingOptions); i++) {
						const bool isSelected = (easingIndex == i);
						if (ImGui::Selectable(easingOptions[i], isSelected)) {
							parameter_.easingType = static_cast<EasingType>(i);
							easingIndex = i;
						}
						if (isSelected) ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
			}
		}
	}

	// Additional Parameters
	if (ImGui::CollapsingHeader("Additional Parameters")) {
		this->DerivedImGui();
	}

}

template<ParticleType particleType>
inline void BaseParticle<particleType>::Emit() {

	particleSystem_->EmitParticle(IBaseParticle::GetName(), particleType_, parameter_);
}

template<ParticleType particleType>
inline void BaseParticle<particleType>::UpdateFrequencyEmit(float deltaTime) {

	parameter_.frequencyTime += deltaTime;

	if (parameter_.frequency <= parameter_.frequencyTime) {

		particleSystem_->EmitParticle(IBaseParticle::GetName(), particleType_, parameter_);
		parameter_.frequencyTime -= parameter_.frequency;
	}
}